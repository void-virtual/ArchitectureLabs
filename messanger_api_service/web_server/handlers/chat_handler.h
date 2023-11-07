#ifndef CHATHANDLER_H
#define CHATHANDLER_H

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

#include "../../../database/chat.h"
#include "../../../database/user_to_chat.h"
#include "../../../helper.h"
#include "../clients/auth_service_client.h"

class ChatHandler : public HTTPRequestHandler
{
private:
    AuthServiceClient _auth_service_client;
public:
    ChatHandler()
    {
    }

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
            // получить чаты по user_id и получить чат по chat_id
            // GET /chat?chat_id={chat_id}
            // GET /chat?user_id={user_id}
            if (isGet(request)) {
                if (contains(request.getURI(), "searchByChatId")) {
                    if (!form.has("chatId")) {
                        badRequest(response, "chatId not exist's");
                    }
                    auto chatId = atol(form.get("chatId").c_str());
                    auto chat = database::Chat::read_by_id(chatId);
                    if (chat) {
                        auto jsonChat = chat->toJSON();
                        ok(response, jsonChat);
                    } else {
                        notFound(response, "cannot find chat with id " + std::to_string(chatId));
                    }
                    return;
                }

                if (contains(request.getURI(), "searchByUserId")) {
                    if (!form.has("userId")) {
                        badRequest(response, "userId not exist's");
                        return;
                    }
                    long query_param_id = atol(form.get("userId").c_str());

                    auto users_to_chats = database::UserToChat::read_chats_by_user_id(query_param_id);
                    Poco::JSON::Object::Ptr content = new Poco::JSON::Object();
                    Poco::JSON::Array::Ptr arr = new Poco::JSON::Array();
                    for (auto s : users_to_chats) {
                        arr->add(s.toJSON());
                    }
                    content->set("chats", arr);
                    ok(response, content);
                    return;
                }
            }
            
            if (isPost(request)) { // создать чат
                if (contains(request.getURI(), "addMember")) {
                    if (!form.has("chatId") || !form.has("userId")) {
                        badRequest(response, "do not have valid parameters chatId and userId");
                        return;
                    }
                    long chatId = atol(form.get("chatId").c_str());
                    long userId = atol(form.get("userId").c_str());
                    database::UserToChat userToChat;
                    userToChat.chat_id() = chatId;
                    userToChat.user_id() = userId;

                    try {
                        userToChat.save_to_mysql();
                        Poco::JSON::Object::Ptr content = new Poco::JSON::Object();
                        ok(response, content);
                    } catch (Poco::Exception& ex) {
                        std::cout << "Exception: " << ex.what() << std::endl;
                        unprocessableEntity(response, std::string(ex.what()));
                    }
                    return;
                }
                if(form.has("chatName")) {
                    try {
                        database::Chat chat;
                        database::UserToChat userToChat;
                        chat.name() = form.get("chatName");
                        chat.creator_id() = user_id;
                        std::string message;
                        std::string reason;
                        Poco::JSON::Object::Ptr content = new Poco::JSON::Object();
                        chat.save_to_mysql();
                        content->set("chat_id", chat.get_id());
                        userToChat.chat_id() = chat.get_id();
                        userToChat.user_id() = user_id;
                        userToChat.save_to_mysql();
                        ok(response, content);
                    } catch (const Poco::Exception& ex) {
                        std::cout << "Exception: " << ex.what() << std::endl;
                        internalServerError(response, std::string(ex.what()));
                    }
                    return;
                }
            }
        }
        catch (...)
        {
            std::cout << "Unexpected error";
            internalServerError(response);
            return;
        }

        notFound(response, "/chat");
    }
};
#endif