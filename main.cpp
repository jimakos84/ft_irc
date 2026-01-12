
#include "utils.hpp"
#include "Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
        return (print_err("Usage: ./ircserv <port number> <password>", ""), 1);
    if (validate_port(argv[1]) == false)
        return 1;
    if (validate_password(argv[2]) == false)
        return 1;
    try {
        Server server(std::atoi(argv[1]), argv[2]);
        server.start();
        server.run();
    }
    catch (const std::exception& e) {
        print_err(e.what(), "");
        return 1;
    }
    return 0;
}
