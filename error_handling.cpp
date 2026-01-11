
#include "utils.hpp"

void print_err(std::string err_msg, std::string additional_info) {
    std::cout << "Error: " << err_msg << " ";
    if (!additional_info.empty())
        std::cout << additional_info << std::endl;
    std::cout << std::endl;
    return ;
}