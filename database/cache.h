#ifndef CACHE_H
#define CACHE_H

#include <string>
#include <iostream>
#include <memory>


namespace database
{
    class Cache
    {
    private:
        std::shared_ptr<std::iostream> _stream;

        Cache();
    public:
        static Cache get();
        void put(long id, const std::string& val);
        bool get(long id, std::string& val);

    };
}

#endif