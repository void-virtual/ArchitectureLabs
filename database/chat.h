#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Chat{
        private:
            long _id;
            std::string _name;
            std::string _creator_id;

        public:

            static Chat fromJSON(const std::string & str);

            long get_id() const;
            const std::string &get_name() const;
            const std::string& get_creator_id() const;

            long& id();
            std::string &name();
            std::string &creator_id();

            static void init();
            static std::optional<Chat> read_by_id(long id);
            static std::vector<Chat> search(std::string name);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif