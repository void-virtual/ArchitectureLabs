#include "user_to_chat.h"
#include "user.h"
#include "chat.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void UserToChat::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `UserToChat` (`chat_id` INT NOT NULL,"
                        << "`user_id` VARCHAR(256) NOT NULL,"
                        << "PRIMARY KEY(chat_id,user_id))-- sharding:0",
                now;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }

        std::cout << "Entity UserToChat initiated\n";
    }

    Poco::JSON::Object::Ptr UserToChat::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("chat_id", _chat_id);
        root->set("user_id", _user_id);

        return root;
    }

    UserToChat UserToChat::fromJSON(const std::string &str)
    {
        UserToChat user_to_chat;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        user_to_chat.chat_id() = object->getValue<long>("chat_id");
        user_to_chat.user_id() = object->getValue<std::string>("user_id");

        return user_to_chat;
    }


    std::vector<Chat> UserToChat::read_chats_by_user_id(std::string user_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            std::vector<Chat> result;
            Chat a;
            select << "SELECT DISTINCT Chat.id, Chat.name FROM Chat INNER JOIN UserToChat ON Chat.id = UserToChat.chat_id WHERE UserToChat.user_id=?-- sharding:0",
                into(a.id()),
                into(a.name()),
                use(user_id),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            
        }
        return {};
    }

    void UserToChat::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO UserToChat (chat_id, user_id) VALUES(?, ?)-- sharding:0",
                use(_chat_id),
                use(_user_id);

            insert.execute();

            std::cout << "inserted: ";
            Poco::JSON::Stringifier::stringify(toJSON(), std::cout);
            std::cout << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    long UserToChat::get_chat_id() const
    {
        return _chat_id;
    }

    std::string UserToChat::get_user_id() const
    {
        return _user_id;
    }

    long &UserToChat::chat_id()
    {
        return _chat_id;
    }

    std::string &UserToChat::user_id()
    {
        return _user_id;
    }
}