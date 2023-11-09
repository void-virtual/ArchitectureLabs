cmake_minimum_required(VERSION 3.2)

project(hl_server C CXX)

SET(MESSENGER_API_BINARY "messenger_api")
SET(AUTH_SERVICE "auth_api")
SET(KAFKA_WRITER "writer")
SET(REDISCPP_FLAGS "-DREDISCPP_HEADER_ONLY=ON")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall ${REDISCPP_FLAGS}")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -DNDEBUG -Wall ${REDISCPP_FLAGS}")
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL)
find_package(Threads)
find_package(ZLIB)
find_package(CppKafka REQUIRED)
find_package(Poco REQUIRED COMPONENTS Foundation Net XML JSON Crypto NetSSL Util)


if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})
include_directories("/usr/include/mysql")
include_directories("/usr/local/include/redis-cpp")
include_directories("/usr/local/include/cppkafka")
include_directories("/usr/local/include/mysql")
link_directories("/usr/local/lib")

add_executable(${MESSENGER_API_BINARY} messenger_api_service/main.cpp
                                 config/config.cpp 
                                 database/database.cpp
                                 database/user.cpp
                                 database/message.cpp
                                 database/chat.cpp
                                 database/user_to_chat.cpp 
                                 database/cache.cpp)


target_include_directories(${MESSENGER_API_BINARY} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${MESSENGER_API_BINARY} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${MESSENGER_API_BINARY} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${MESSENGER_API_BINARY} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${MESSENGER_API_BINARY} PRIVATE
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "rdkafka++"
                             CppKafka::cppkafka
                             "mysqlclient"
                             ZLIB::ZLIB)


add_executable(${AUTH_SERVICE} auth_service/auth_main.cpp 
                               database/user.cpp 
                               database/database.cpp
                               database/cache.cpp 
                               config/config.cpp)
target_include_directories(${AUTH_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${AUTH_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${AUTH_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${AUTH_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${AUTH_SERVICE} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB
                             "rdkafka++"
                             CppKafka::cppkafka)

add_executable(${KAFKA_WRITER} kafka/main.cpp
        database/user.cpp
        database/database.cpp
        database/cache.cpp
        config/config.cpp)
target_include_directories(${KAFKA_WRITER} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${KAFKA_WRITER} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${KAFKA_WRITER} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${KAFKA_WRITER} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${KAFKA_WRITER} PRIVATE
        ${CMAKE_THREAD_LIBS_INIT}
        ${Poco_LIBRARIES}
        "PocoData"
        "PocoDataMySQL"
        "mysqlclient"
        ZLIB::ZLIB
        "rdkafka++"
        CppKafka::cppkafka)









