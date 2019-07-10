#ifndef FILE_UTILS
#define FILE_UTILS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/types.h>


/*
 * A class to implement and provide through static functions some common file-specific opereations.
 */
class FileUtils {
public:
	/*
	 * Gets the names of the files contained in a directory.
	 * \param name The pathname of the directory under search
	 * \param v Reference to the vector to store the file names
	 */


	static void read_directory(const std::string& name, std::vector<std::string>& v);
	/*
	 * Reads the contents of a file to a string.
	 * \param fileName The name of the file to read
	 * \return A string containing the contents of the file
	 */
	static std::string read_file_to_string(const std::string& fileName);


private:
	FileUtils();
};

#endif // FILE_UTILS
