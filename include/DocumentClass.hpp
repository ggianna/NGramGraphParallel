#ifndef DOCUMENT_CLASS_H
#define DOCUMENT_CLASS_H

#include <CL/cl.hpp>

#include "FileUtils.hpp"
#include "NGramGraph.hpp"
#include "NGGUpdateOperator.hpp"
#include "DocumentClassComponent.hpp"
#include "OpenclUpdateComputation.hpp"

using namespace cl;

/* 
 * \class An NGramGraph-derived class that represents a class of documents.
 */
class DocumentClass : public NGramGraph {
protected:

	/*
	 * The number of documents that constitue the class.
	 */
	unsigned int numberOfConstituents;


	/*
	 * An NGGUpdateOperator to come in handy when the document class is updated.
	 */
	NGGUpdateOperator updateOp;


	/*
	 * Computes and returns the learning factor based on the current number of constituents.
	 */
	float computeLearningFactor();

public:
	/*
	 * Constructor.
	 * Payload, Splitter and ProximityEvaluator member fields are all set to NULL and never used.
	 * OpenCL-related fields are left uninitialized.
	 */
	DocumentClass();


	/*
	 * Fills the document class with the edges contained in the lines of the given file.
	 * The first entry of each line is the edge's weight, the second entry is the edge's label.
	 * \param filename The name of the file containing the edges' info (weight and label)
	 */
	void restoreFromFile(std::string filename);


	/*
	 * Caches the edges of the document class in a file, so they can be restored in a later execution,
	 * without recomputing it.
	 * \param dir The directory in which to save the file
	 * \param fname The name of the file
	 */
	void cache(std::string dir, std::string fname);


	/*
	 * Builds the document class, by doing consecutive updates with it's constituents.
	 * \param dir A directory containing the text documents that constitute the class
	 */
	void build(std::string dir);

	/*
	 * Updates the document class with a new document. A precise update, iterates over the edges of both the 
	 * class graph and the document graph, while a non precise one iterates over the edges of the document graph only.
	 * The non precise operation is faster, but not strictly correct because the edges of the class graph that are not
	 * present on the document graph are left untouched, while their weights should be reduced.
	 * \param newDoc Pointer to the new document of the class.
	 * \param precise Determines whether it is going to be a precise update operation or not.
	 */
	void update(NGramGraph *newDoc, bool precise = true);


	/*
	 * Updates the document class with a new document, using a specific learning factor.
	 * \param newDoc Pointer to the new document of the class.
	 * \param l The learning factor to be used.
	 * \param precise Determines whether it is going to be a precise update operation or not.
	 */
	void update(NGramGraph *newDoc, double l, bool precise = false);
};

#endif // DOCUMENT_CLASS_H
