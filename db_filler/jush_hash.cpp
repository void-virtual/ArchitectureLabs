#include <functional>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    std::cout << std::hash<std::string>{}(std::string(argv[1])) << std::endl;
    return 0;
}