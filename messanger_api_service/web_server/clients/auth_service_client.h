#ifndef AUTHSERVICECLIENT_H
#define AUTHSERVICECLIENT_H

#include <iostream>
#include <string>
#include <optional>
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NetException.h"
#include "Poco/URI.h"
#include <Poco/JSON/Parser.h>

using Poco::Net::HTTPServerRequest;

class AuthServiceClient {
    private:
        std::string _auth_service_url = "http://" + std::string(std::getenv("AUTH_API_HOST")) + ":8081/user/auth";
    public:
    static const int NOT_AUTHORIZED = -1;
    int checkAccess(HTTPServerRequest &request) {
        try {
            std::string scheme;
            std::string info;
            request.getCredentials(scheme, info);
            
            std::cout << "scheme: " << scheme << " identity: " << info << std::endl;

            std::string login, password;
            std::string string_result;
            if (scheme == "Basic")
            {
                    Poco::URI uri(_auth_service_url);
                    uri.addQueryParameter("token", info);
                    Poco::Net::HTTPClientSession s(uri.getHost(), uri.getPort());
                    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri.toString());
                    request.setVersion(Poco::Net::HTTPMessage::HTTP_1_1);
                    
                    s.sendRequest(request);
                    Poco::Net::HTTPResponse response;
                    std::istream &rs = s.receiveResponse(response);
                    while (rs)
                    {
                        char c{};
                        rs.read(&c, 1);
                        if (rs)
                            string_result += c;
                    }

                    if (response.getStatus() != 200) {
                        std::cout << "Gotten " << response.getStatus() << std::endl;
                        return NOT_AUTHORIZED;
                    }
                    Poco::JSON::Parser parser;
                    auto result = parser.parse(string_result);
                    return result.extract<Poco::JSON::Object::Ptr>()->getValue<long>("user_id");
                
            }
            return NOT_AUTHORIZED;
        }
        catch (Poco::Exception &ex)
        {
            std::cout << "exception:" << ex.what() << std::endl;
            return NOT_AUTHORIZED;
        }
        catch (...)
        {
            std::cout << "Unexpected exception" << std::endl;
            return NOT_AUTHORIZED;
        }
    }
};

#endif