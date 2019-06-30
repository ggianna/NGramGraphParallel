/*
 * This program creates DocumentClass objects (class graphs) for various topics and counts the number of their edges.
 * It works as follows:
 * Firstly, it reads all the subdirectories in the TOPICS_ROOT directory, considering them as different topics.
 * Then, it iterates over each subdirectory/topic it found above and builds it's DocumentClass object by iterating
 * over the text files contained in it and calling the update method for each of them.
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <sys/types.h>
#include <dirent.h>

#include "NGramGraph.hpp"
#include "ProximityApproach.hpp"
#include "DocumentClass.hpp"

#define TOPICS_ROOT "./topics/"


/*
 * Gets thenames of the files contained in a directory.
 * \param name The pathname of the directory under search
 * \param v Reference to the vector to store the file names
 */
void read_directory(const std::string& name, std::vector<std::string>& v)
{
    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
	if (dp->d_name[0] != '.')
            v.push_back(dp->d_name);
    }
    closedir(dirp);
}


/*
 * Reads the contents of a file to a string.
 * \param fileName The name of the file to read
 * \return A string containing the contents of the file
 */
std::string read_file_to_string(const std::string& fileName) {
	ifstream ifs(fileName.c_str(), ios::in | ios::binary | ios::ate);
	ifstream::pos_type fileSize = ifs.tellg();
	ifs.seekg(0, ios::beg);

	vector<char> bytes(fileSize);
	ifs.read(&bytes[0], fileSize);

	return std::string(&bytes[0], fileSize);
}


int main() {
	std::vector<std::string> topics;
	std::vector<std::string> topic_files;
	std::vector<DocumentClass *> class_graphs; // A vector containing pointers to the resulting DocumentClass objects
	NGramGraph * sample_ngg;
	StringSplitter ssplitter;
	StringPayload spayload; // A single StringPayload object to carry the contents of the files, one at a time
	ProximityApproach *approach = new SymmetricApproach();
	int cnt = 0;

	// Read all subdirectories/topics and place them in the topics vector
	read_directory(TOPICS_ROOT, topics);

	class_graphs.resize(topics.size());

	// Iterate over every different topic/subdirectory
	for (const auto& topic : topics) {
		class_graphs[cnt] = new DocumentClass();

		topic_files.clear();
		read_directory(TOPICS_ROOT + topic, topic_files);
		// Iterate over every file of the current topic/subdirectory and update the current DocumentClass object with it
		for (const auto& file : topic_files) {

			// Get the contents of the file into a string and set the spayload with it.
			spayload.setPayload(read_file_to_string(TOPICS_ROOT + topic + "/" + file));

			// Create a temporary NGramGraph object representing the current file
			sample_ngg = new NGramGraph(nullptr, &ssplitter, &spayload, 4, approach);
			sample_ngg->createGraph();

			// Update the current DocumentClass object with the above NGramGraph object
			class_graphs[cnt]->update(sample_ngg);

			delete sample_ngg;

		}

		cout << "Created class graph for topic '" << topics[cnt]
		     << "' with " << class_graphs[cnt]->numberOfEdges() << " edges.\n";
		++cnt;
	}

	return 0;
}
