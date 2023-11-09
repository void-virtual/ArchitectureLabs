#include "message.h"
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

    void Message::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Message` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`chat_id` INT NOT NULL,"
                        << "`user_id` VARCHAR(256) NOT NULL,"
                        << "`message` VARCHAR(1024) NOT NULL,"
                        << "PRIMARY KEY (`id`))-- sharding:0",
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
        std::cout << "Entity Message initiated\n";
    }

    Poco::JSON::Object::Ptr Message::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("chat_id", _chat_id);
        root->set("user_id", _user_id);
        root->set("message", _message);

        return root;
    }

    Message Message::fromJSON(const std::string &str)
    {
        Message message;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        message.id() = object->getValue<long>("id");
        message.chat_id() = object->getValue<long>("chat_id");
        message.user_id() = object->getValue<std::string>("user_id");
        message.message() = object->getValue<std::string>("message");

        return message;
    }

    std::optional<Message> Message::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Message a;
            select << "SELECT id, chat_id, user_id, message FROM Message where id=?-- sharding:0",
                into(a._id),
                into(a._chat_id),
                into(a._user_id),
                into(a._message),
                use(id),
                range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return a;
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

    std::optional<Message> Message::read_by_user_id(std::string user_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Message a;
            select << "SELECT id, chat_id, user_id, message FROM Message where user_id=?-- sharding:0",
                    into(a._id),
                    into(a._chat_id),
                    into(a._user_id),
                    into(a._message),
                    use(user_id),
                    range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return a;
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

    void Message::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Message (chat_id, user_id, message) VALUES(?, ?, ?)-- sharding:0",
                use(_chat_id),
                use(_user_id),
                use(_message);

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()-- sharding:0",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
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

    long Message::get_id() const
    {
        return _id;
    }

    long Message::get_chat_id() const
    {
        return _chat_id;
    }

    const std::string& Message::get_user_id() const
    {
        return _user_id;
    }

    const std::string &Message::get_message() const
    {
        return _message;
    }

    long &Message::id()
    {
        return _id;
    }

    long &Message::chat_id()
    {
        return _chat_id;
    }

    std::string& Message::user_id()
    {
        return _user_id;
    }

    std::string &Message::message()
    {
        return _message;
    }

    
    std::vector<Message> Message::read_by_chat_id(long chat_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            std::vector<Message> result;
            Message a;

            select << "SELECT id, chat_id, user_id, message FROM Message WHERE Message.chat_id=?-- sharding:0",
                into(a.id()),
                into(a.chat_id()),
                into(a.user_id()),
                into(a.message()),
                use(chat_id),
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

}