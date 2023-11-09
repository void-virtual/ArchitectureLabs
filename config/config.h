#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class  Config{
    private:
        Config();
        std::string _host;
        std::string _port;
        std::string _login;
        std::string _password;
        std::string _database;
        std::string _cache_servers;
        std::string _queue_host;
        std::string _queue_topic;
        std::string _queue_group_id;


    public:
        static Config& get();

        std::string& port();
        std::string& host();
        std::string& login();
        std::string& password();
        std::string& database();
        std::string& cache_servers();
        std::string& queue_host();
        std::string& queue_topic();
        std::string& queue_group_id();

        const std::string& get_port() const ;
        const std::string& get_host() const ;
        const std::string& get_login() const ;
        const std::string& get_password() const ;
        const std::string& get_database() const ;
        const std::string& get_cache_servers() const ;
        const std::string& get_queue_host() const;
        const std::string& get_queue_topic() const ;
        const std::string& get_queue_group_id() const ;
};

#endif