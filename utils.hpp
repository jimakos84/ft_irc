
#pragma once

#include <iostream>
#include <regex>
#include <string>

bool validate_port(const char * port);
void print_err(std::string err_msg, std::string additional_info);