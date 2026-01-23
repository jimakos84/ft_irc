
#pragma once

#include <string>
#include <sstream>
#include <vector>


class Parser {
public:
	Parser();
	~Parser();

	void		splitIrcLine(const std::string& line, std::string& cmd,
					std::vector<std::string>& params);
};