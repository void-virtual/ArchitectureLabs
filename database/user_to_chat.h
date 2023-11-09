#ifndef USER_TO_CHAT_H
#define USER_TO_CHAT_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include "chat.h"
#include <optional>

namespace database
{
    class UserToChat{
        private:
            long _chat_id;
            std::string _user_id;

        public:

            static UserToChat fromJSON(const std::string & str);

            long get_chat_id() const;
            std::string get_user_id() const;

            long& chat_id();
            std::string& user_id();

            static void init();
            static std::vector<Chat> read_chats_by_user_id(std::string user_id);
            static std::optional<long> auth(std::string &login, std::string &password);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif