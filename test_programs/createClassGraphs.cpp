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

#include "NGramGraph.hpp"
#include "ProximityApproach.hpp"
#include "DocumentClass.hpp"
#include "FileUtils.hpp"

#define TOPICS_ROOT "./topics/"




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
	FileUtils::read_directory(TOPICS_ROOT, topics);

	class_graphs.resize(topics.size());

	// Iterate over every different topic/subdirectory
	for (const auto& topic : topics) {
		class_graphs[cnt] = new DocumentClass();

		topic_files.clear();
		FileUtils::read_directory(TOPICS_ROOT + topic, topic_files);
		// Iterate over every file of the current topic/subdirectory and update the current DocumentClass object with it
		for (const auto& file : topic_files) {
			// Get the contents of the file into a string and set the spayload with it.
			spayload.setPayload(FileUtils::read_file_to_string(TOPICS_ROOT + topic + "/" + file));

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
