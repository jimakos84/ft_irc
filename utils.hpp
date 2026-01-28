
#pragma once

#include <iostream>
#include <regex>
#include <string>

class Channel;

bool						validate_port(const char * port);
void						print_err(std::string err_msg, std::string additional_info);
bool						validate_password(const std::string& pass);
std::vector<std::string>	splitLine(const std::string &line, char delim);
bool                        isChannelEmpty(Channel &chan);
void		                splitIrcLine(const std::string& line, std::string& cmd,
					        std::vector<std::string>& params);
