#include <algorithm>
#include "InputParser.hpp"

InputParser::InputParser(int& argc, char **argv) {
	for (int i=1; i<argc; i++) {
		tokens.push_back(std::string(argv[i]));
	}
}


const std::string& InputParser::getCmdOption(const std::string& option) {
	std::vector<std::string>::const_iterator itr;
	itr = std::find(tokens.begin(), tokens.end(), option);
	if (itr != tokens.end() && ++itr != tokens.end()) {
		return *itr;
	}
	static const std::string empty_str("");
	return empty_str;
}


bool InputParser::cmdOptionExists(const std::string& option) {
	return std::find(tokens.begin(), tokens.end(), option) != tokens.end();
}
