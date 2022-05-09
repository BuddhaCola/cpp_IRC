#include <iostream>
#include <string>
#include "../includes/Server.hpp"


int main(int argc, char *argv[]) {
    std::cout << "Start server..." << std::endl;
    if (argc != 3) {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
        return 0;
    }
    try {
        creat_socket(atoi(argv[1]), atoi(argv[2]));

    }
    catch (std::exception &e) {
        std::cout << "Error during initialization: " << e.what() << std::endl;
    }
    return 0;
}
