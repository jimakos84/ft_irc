
#include "utils.hpp"


int main(int argc, char **argv)
{
    if (argc != 3)
        return (print_err("Usage: ./ircserv <port number> <password>", ""), 1);
    
    if (validate_port(argv[1]) == false)
        return (1);
    
    return 0;
}