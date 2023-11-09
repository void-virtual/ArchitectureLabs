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

    void Chat::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Chat` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`name` VARCHAR(1024) NOT NULL,"
                        << "`creator_id` VARCHAR(256) NOT NULL,"
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
        std::cout << "Entity Chat initiated\n";
    }

    Poco::JSON::Object::Ptr Chat::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("name", _name);
        root->set("creator_id", _creator_id);

        return root;
    }

    Chat Chat::fromJSON(const std::string &str)
    {
        Chat chat;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        chat.id() = object->getValue<long>("id");
        chat.name() = object->getValue<std::string>("name");

        return chat;
    }

    std::optional<Chat> Chat::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Chat a;
            select << "SELECT id, name, creator_id FROM Chat where id=?-- sharding:0",
                into(a._id),
                into(a._name),
                into(a._creator_id),
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

    void Chat::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Chat (name, creator_id) VALUES(?, ?)-- sharding:0",
                use(_name),
                use(_creator_id);

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

    long Chat::get_id() const
    {
        return _id;
    }

    const std::string &Chat::get_name() const
    {
        return _name;
    }

    long &Chat::id()
    {
        return _id;
    }

    std::string &Chat::name()
    {
        return _name;
    }
    
    const std::string& Chat::get_creator_id() const {
        return _creator_id;
    }
    std::string& Chat::creator_id() {
        return _creator_id;
    }
}