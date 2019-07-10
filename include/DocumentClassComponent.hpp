#ifndef DOCUMENT_CLASS_COMPONENT_H
#define DOCUMENT_CLASS_COMPONENT_H

#include <vector>
#include <string>
#include <tuple>

#include "Atom.hpp"
#include "StringSplitter.hpp"
#include "StringPayload.hpp"
#include "HashFunction.hpp"

#define EDGE_WEIGHT_TYPE float
#define EDGE_WEIGHT_UNIT_VALUE 1.0

/*
 * A class that represents a text document and is useful only when updating a class graph with a new document using OpenCL.
 * It's key element is a hash table, than can be easily compared in parallel against the hash table of the class graph.
 */

class DocumentClassComponent {
private:
	std::size_t hash_table_size;

	unsigned int correlation_window;

	/* Pointer to the hash table */
	EDGE_WEIGHT_TYPE *hash_table;

	/* This array is used to indicate new edges added from the component to the class graph */
	unsigned char *flags;

	/* Holds the labels of the edges */
	vector<std::string> edge_labels;

	/* Holds the hash values of the labels */
	vector<unsigned long> hash_values;

	/* Used to split the text document in n-grams */
	StringSplitter *splitter;

	/* Used to hold the text document in memory */
	StringPayload  *payload;

	/* Hash function to apply on the edges' labels */
	HashFunction<std::string, unsigned long> *hash_function;


	/*
	 * Applies the hash function to an edge label.
	 * \param key The string of the label that is hashed
	 * \return A pair of the computed hash value and it's corresponding index in the hash table
	 */
	std::pair<unsigned long, unsigned int> applyHashFunction(std::string key) {
		unsigned long hash;
		unsigned int index;

		hash_function->setArgument(key);
		hash = hash_function->apply();
		index = hash % hash_table_size;

		return {hash, index};
	}


	/*
	 * Inserts an new edge in the tables.
	 * \param index The index of the new edge in the tables
	 * \param label The label of the new edge
	 * \param hash The hash value of the label
	 */
	void insertToTable(unsigned int index, std::string label, unsigned long hash) {
		// std::cout << "DocumentClassComponent::insertToTable(): Inserting edge with label '" << label << "'" << std::endl;
		if (hash_table[index] == 0) {
			edge_labels[index] = label;
			hash_values[index] = hash;
			hash_table[index] = EDGE_WEIGHT_UNIT_VALUE;
		}
		else if (label != edge_labels[index]) {
			// This is a colllision. Don't update the table in this case. The new edge label won't be inserted.
			// TODO Apply a different policy to resolve collisions.
			/*
			std::cout << "DocumentClassComponent::insertToTable(): Collision for edge "
				  << "labels '" << label << "', '" << edge_labels[index] << "' with hash "
				  << "value " << index << ". Edge with label '" << label << "' won't be "
				  << "inserted in the tables." << std::endl;
			*/
		}
		else {
			hash_table[index] += EDGE_WEIGHT_UNIT_VALUE;
		}
	}


	/*
	 * Creates both direction edges between an atom and every other atom in a vector.
	 * \param source The atom that participates in all edges
	 * \param targets A vector containing the atoms to be connected with source
	 */
	void createEdgesToPrecedingAtoms(Atom<std::string> source, vector<Atom<std::string>> targets) {
		std::string source_name = source.toString();
		std::string label_name, target_name;
		unsigned long hash;
		unsigned int index;

		for (auto& target : targets) {
			target_name = target.toString();

			label_name = source_name + "___" + target_name;
			std::tie(hash, index) = applyHashFunction(label_name);
			insertToTable(index, label_name, hash);

			label_name = target_name + "___" + source_name;
			std::tie(hash, index) = applyHashFunction(label_name);
			insertToTable(index, label_name, hash);
		}
	}

public:
	/*
	 * Constructor.
	 */
	DocumentClassComponent(std::size_t size, unsigned int win, StringSplitter *split,
			       StringPayload *payl, HashFunction<std::string, unsigned long> *hf) : 
		hash_table_size(size), correlation_window(win),
		edge_labels(size), hash_values(size),
		splitter(split), payload(payl),
		hash_function(hf)
	{
		// () initializes all elements to 0
		hash_table = new EDGE_WEIGHT_TYPE[size]();
		flags = new unsigned char[size]();

	}


	/*
	 * Breaks the payload into atoms (n-grams) and stores the edges between them in the tables. It uses
	 * the symmetric proximity approach.
	 */
	void fillTables() {
		vector<Atom<std::string>> atoms, preceding_atoms;
		atoms = splitter->splitPayloadToAtoms(payload);
		unsigned int half_win = correlation_window / 2;

		for (const auto& atom : atoms) {
			createEdgesToPrecedingAtoms(atom, preceding_atoms);

			preceding_atoms.push_back(atom);
			if (preceding_atoms.size() > half_win) {
				preceding_atoms.erase(preceding_atoms.begin());
			}
		}
	
	}


	/*
	 * Resets the tables so that the same object can be used to represent another document.
	 */
	void reset() {
		for (std::size_t i=0; i<hash_table_size; i++) {
			hash_table[i] = 0.0;
			flags[i] = (unsigned char)0;
		}
	}


	/*
	 * Getter functions.
	 */
	std::size_t getHashTableSize() {
		return hash_table_size;
	}

	EDGE_WEIGHT_TYPE * getHashTable() {
		return hash_table;
	}

	unsigned char * getFlags() {
		return flags;
	}

	vector<std::string>& getEdgeLabels() {
		return edge_labels;
	}

	vector<unsigned long>& getHashValues() {
		return hash_values;
	}

};

#endif // DOCUMENT_CLASS_COMPONENT_H
