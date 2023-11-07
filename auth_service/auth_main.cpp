
#include <optional>
#include "http_auth_server.h"

int main(int argc, char*argv[]) 
{
    std::cout << "Started";
    HTTPAuthWebServer app;
    return app.run(argc, argv);
}