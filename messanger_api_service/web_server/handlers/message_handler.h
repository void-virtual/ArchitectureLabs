#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

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
#include <iostream>
#include <iostream>
#include <fstream>

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

#include "../../../database/message.h"
#include "../../../helper.h"
#include "../clients/auth_service_client.h"

class MessageHandler : public HTTPRequestHandler
{
public:
    MessageHandler()
    {
    }
    AuthServiceClient _auth_service_client;

    AuthServiceClient& authServiceClient() {
        return _auth_service_client;
    }

    void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    {
        
        long user_id = authServiceClient().checkAccess(request);

        if (user_id == AuthServiceClient::NOT_AUTHORIZED) {
            unauthorized(response);
            return;
        }

        HTMLForm form(request, request.stream());
        try
        {
            // получить сообщения
            if (isGet(request)) {
                if(!form.has("chatId")) {
                    badRequest(response);
                    return;
                }
                long chat_id = atol(form.get("chatId").c_str());
                auto messages = database::Message::read_by_chat_id(chat_id);
                Poco::JSON::Object::Ptr content = new Poco::JSON::Object();
                Poco::JSON::Array::Ptr arr = new Poco::JSON::Array();
                for (auto s : messages) {
                    arr->add(s.toJSON());
                }
                content->set("messages", arr);
                content->set("chatId", chat_id);
                ok(response, content);
                return;
            }
            // отправить сообщние
            if (isPost(request)) {
                if(!form.has("chatId") ||
                   !form.has("text")) {
                    badRequest(response);
                    return;
                }
                long chat_id = atol(form.get("chatId").c_str());
                std::string text = std::string(form.get("text").c_str());

                database::Message message;
                message.chat_id() = chat_id;
                message.message() = text;
                message.user_id() = user_id;
                message.save_to_mysql();

                Poco::JSON::Object::Ptr content = new Poco::JSON::Object();
                content->set("id", message.id());
                ok(response, content);
                return;
            } 

        }
        catch (Poco::Exception& ex) 
        {
            internalServerError(response, ex.what());
        }

        notFound(response, "/message");
    }
};
#endif