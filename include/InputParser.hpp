#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <string>
#include <vector>

/*
 * A simple class to operate as a command line input parser.
 */
class InputParser {
public:
	/*
	 * Constructor.
	 */
	InputParser(int& argc, char **argv);

	/*
	 * Checks if an options has been enabled and returns it's value (if there is one).
	 * \param option The option name
	 * \return The value of the option
	 */
	const std::string& getCmdOption(const std::string& option);

	/*
	 * Checks if an option has been enabled.
	 * \param option The option name
	 * \return True if the option is enabled, false otherwise
	 */
	bool cmdOptionExists(const std::string& option);


private:
	std::vector<std::string> tokens;
};

#endif //INPUT_PARSER_H
