#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <boost/scoped_array.hpp>

#define TEXT_1_RELATIVE_PATH "topics/motorcycles/101725"
#define TEXT_1_TOPIC "motorcycles"
#define TEXT_2_RELATIVE_PATH "topics/mideast/75364"
#define TEXT_2_TOPIC "mideast"
#define TEXT_3_RELATIVE_PATH "topics/guns/53293"
#define TEXT_3_TOPIC "guns"
#define TEXT_4_RELATIVE_PATH "topics/forsale/70337"
#define TEXT_4_TOPIC "forsale"
#define DEMO_INPUT_FILENAME "demo_input.txt"

std::string get_program_path() {
	std::string path;
	size_t size = 1024;
	const ssize_t err = static_cast<ssize_t>(-1);
	while (true) {
		boost::scoped_array<char> buffer(new char[size]);
		ssize_t res = readlink("/proc/self/exe", buffer.get(), size);
		if (res == err) {
			if (errno == ENAMETOOLONG)
				size *= 2; // try again
			else
				throw std::runtime_error("unexpected error during readlink");
		}
		else {
			path.assign(buffer.get(), res);
			break;
		}
	}

	return path;
}


int main() {
	std::string demos_path = get_program_path();
	demos_path = demos_path.substr(0, demos_path.find_last_of("/"));
	std::string project_path = demos_path.substr(0, demos_path.find_last_of("/")+1);
	demos_path += "/";
	//std::ofstream ofs;

	//ofs.open(demos_path + DEMO_INPUT_FILENAME, std::ofstream::out | std::ofstream::trunc);
	//ofs << project_path + TEXT_1_RELATIVE_PATH << std::endl;
	//ofs << project_path + TEXT_2_RELATIVE_PATH << std::endl;
	//ofs << project_path + TEXT_3_RELATIVE_PATH << std::endl;
	//ofs << project_path + TEXT_4_RELATIVE_PATH << std::endl;
	//ofs.close();
	std::cout << project_path + TEXT_1_RELATIVE_PATH << " " << TEXT_1_TOPIC << std::endl;
	std::cout << project_path + TEXT_2_RELATIVE_PATH << " " << TEXT_2_TOPIC << std::endl;
	std::cout << project_path + TEXT_3_RELATIVE_PATH << " " << TEXT_3_TOPIC << std::endl;
	std::cout << project_path + TEXT_4_RELATIVE_PATH << " " << TEXT_4_TOPIC << std::endl;

	return 0;
}


