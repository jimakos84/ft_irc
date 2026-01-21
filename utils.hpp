
#pragma once

#include <iostream>
#include <regex>
#include <string>

bool						validate_port(const char * port);
void						print_err(std::string err_msg, std::string additional_info);
bool						validate_password(const std::string& pass);
std::vector<std::string>	splitJoinLine(const std::string &line, char delim);
