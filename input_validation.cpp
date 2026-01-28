
#include "utils.hpp"

void print_err(std::string err_msg, std::string additional_info) {
    std::cout << "Error: " << err_msg << " ";
    if (!additional_info.empty())
        std::cout << additional_info << std::endl;
    std::cout << std::endl;
    return ;
}

bool validate_port(const char * port) {

    std::regex port_regex("^[0-9]{1,5}$");
    if (!std::regex_match(port, port_regex))
        return (print_err("Port number has to be between 1 and 5 DIGITS only: ", port), false);

    int port_num = atoi(port);
    if (port_num < 1024 || port_num > 65535)
        return (print_err("Port number has to be: 1024 >= port <= 65535: ", port), false);

    return (true);
}

bool	validate_password(const std::string& pass)
{
	std::regex pattern("^[^\\x00-\\x1F\\s]{4,64}$");

	if (!std::regex_match(pass, pattern))
        return(print_err("Password has to be between 4-64 characters, no spaces, "
            "no control characters!",""), false);
    return true;
}