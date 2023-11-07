cmake_minimum_required(VERSION 3.2)

project(hl_server C CXX)

SET (MESSANGER_API_BINARY "messanger_api")
SET (AUTH_SERVICE "auth_api")

find_package(OpenSSL)
find_package(Threads)
find_package(ZLIB)
find_package(Poco REQUIRED COMPONENTS Foundation Net XML JSON Crypto NetSSL Util)


if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})
include_directories("/usr/include/mysql")
include_directories("/usr/local/include/mysql")
link_directories("/usr/local/lib")

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${MESSANGER_API_BINARY} messanger_api_service/main.cpp 
                                 config/config.cpp 
                                 database/database.cpp
                                 database/user.cpp
                                 database/message.cpp
                                 database/chat.cpp
                                 database/user_to_chat.cpp)


target_include_directories(${MESSANGER_API_BINARY} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${MESSANGER_API_BINARY} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${MESSANGER_API_BINARY} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${MESSANGER_API_BINARY} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${MESSANGER_API_BINARY} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)


add_executable(${AUTH_SERVICE} auth_service/auth_main.cpp database/user.cpp database/database.cpp config/config.cpp)
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
                             ZLIB::ZLIB)





