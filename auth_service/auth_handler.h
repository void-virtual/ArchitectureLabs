#ifndef AUTHHANDLER_H
#define AUTHHANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Base64Encoder.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include "../database/user.h"
#include "../helper.h"

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

class AuthHandler : public HTTPRequestHandler
{

private:
bool check_name(const std::string &name, std::string &reason)
    {
        if (name.length() < 3)
        {
            reason = "Name must be at leas 3 signs";
            return false;
        }

        if (name.find(' ') != std::string::npos)
        {
            reason = "Name can't contain spaces";
            return false;
        }

        if (name.find('\t') != std::string::npos)
        {
            reason = "Name can't contain spaces";
            return false;
        }

        return true;
    };

    bool check_email(const std::string &email, std::string &reason)
    {
        if (email.find('@') == std::string::npos)
        {
            reason = "Email must contain @";
            return false;
        }

        if (email.find(' ') != std::string::npos)
        {
            reason = "EMail can't contain spaces";
            return false;
        }

        if (email.find('\t') != std::string::npos)
        {
            reason = "EMail can't contain spaces";
            return false;
        }

        return true;
    };
    

    Poco::JSON::Object::Ptr remove_password(Poco::JSON::Object::Ptr src)
    {
        if (src->has("password"))
            src->set("password", "*******");
        return src;
    }

public:
    AuthHandler()
    {
    }

    void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
    {
        try {
            HTMLForm form(request, request.stream());
            if (isGet(request) && form.has("all_users")) {

            }

            if (isGet(request) && form.has("user")) {
                std::string uuid = form.get("uuid");
                bool use_cache = !form.has("no_cache");

                std::optional<database::User> result = database::User::read_by_id(uuid, use_cache);
                if (result) {
                    if (use_cache) {
                        result->save_to_cache();
                    }
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream& ostr = response.send();
                    Poco::JSON::Stringifier::stringify(remove_password(result->toJSON()),
                                                       ostr);
                    return;
                } else {
                    response.setStatus(
                            Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("type", "/errors/not_found");
                    root->set("title", "User not found");
                    root->set("status", "404");
                    root->set("detail", "User not found");
                    root->set("instance", "/user");
                    std::ostream& ostr = response.send();
                    Poco::JSON::Stringifier::stringify(root, ostr);
                    return;
                }
            }
            if (isGet(request) && contains(request.getURI(), "/user/auth")) {
                if (!form.has("token")) {
                    badRequest(response);
                    return;
                }
                std::string token = form.get("token");
                std::string login, password;
                try {
                    get_identity(token, login, password);
                } catch (...) {
                    unauthorized(response);
                }
                if (auto id = database::User::auth(login, password)) {
                    Poco::JSON::Object::Ptr content = new Poco::JSON::Object();
                    content->set("user_id", id.value());
                    ok(response, content);
                } else {
                    unauthorized(response);
                }
                return;
            }

            if (isGet(request) && contains(request.getURI(), "/user/search"))
            {
                if (!form.has("first_name") || !form.has("last_name")) {
                    badRequest(response);
                    return;
                }
                std::string fn = form.get("first_name");
                std::string ln = form.get("last_name");
                auto results = database::User::search(fn, ln);
                Poco::JSON::Array arr;
                for (auto s : results)
                    arr.add(remove_password(s.toJSON()));
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(arr, ostr);

                return;
            }

            if (isPut(request) && contains(request.getURI(), "/user/login")) {
                if (!form.has("login") || !form.has("password")) {
                    badRequest(response);
                    return;
                }
                std::string login = form.get("login"), password = form.get("password");
                std::cout << 1 << std::endl;
                if (auto id = database::User::auth(login, password)) {
                    auto user = database::User::read_by_id(id.value()).value();
                    std::string token = user.login() + ':' + user.password();
                    std::ostringstream os;
                    Poco::Base64Encoder b64in(os);
                    b64in << token;
                    b64in.close();
                    Poco::JSON::Object::Ptr content = new Poco::JSON::Object();
                    content->set("token", os.str());
                    ok(response, content);
                } else {
                    unauthorized(response);
                }
                return;

            }

            if (isPost(request) && contains(request.getURI(), "/user")) {
                if (!form.has("first_name") || !form.has("last_name") || !form.has("email") || !form.has("title") || !form.has("login") || !form.has("password"))
                {
                    badRequest(response);
                    return;
                }
                database::User user;
                user.first_name() = form.get("first_name");
                user.last_name() = form.get("last_name");
                user.email() = form.get("email");
                user.title() = form.get("title");
                user.login() = form.get("login");
                user.password() = form.get("password");

                bool check_result = true;
                std::string message;
                std::string reason;

                if (!check_name(user.get_first_name(), reason))
                {
                    check_result = false;
                    message += reason;
                    message += "<br>";
                }

                if (!check_name(user.get_last_name(), reason))
                {
                    check_result = false;
                    message += reason;
                    message += "<br>";
                }

                if (!check_email(user.get_email(), reason))
                {
                    check_result = false;
                    message += reason;
                    message += "<br>";
                }

                if (check_result)
                {
                    user.uuid() = database::User::generate_uuid(user.get_login());
                    bool exists = user.exists_in_mysql();
                    if (!exists) {
                        user.save_to_mysql();
                    }
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    if (exists) {
                        ostr << "Already exists";
                    } else {
                        ostr << user.uuid();
                    }

                    return;
                }
                else
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                    std::ostream &ostr = response.send();
                    ostr << message;
                    response.send();
                    return;
                }
            }
        } catch (...) {
            internalServerError(response);
            return;
        }
        notFound(response);
        return;
    }
};

#endif