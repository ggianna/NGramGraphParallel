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
	 * These are OpenCL-related fields.
	 */
	std::size_t hash_table_size;
	EDGE_WEIGHT_TYPE *hash_table;
	vector<std::string> edge_labels;
	vector<unsigned long> hash_values;

	/*
	 * An OpenclUpdateCompuation object to come in handy when updating the class with OpenCL.
	 */
	OpenclUpdateComputation oclUpdateComp;

	/*
	 * Computes and returns the learning factor based on the current number of constituents.
	 */
	float computeLearningFactor();

	/*
	 * Gets new edges from a component and adds them to the OpenCL-related fields of the class.
	 * This function is called after an OpenclUpdateComputation has been executed with this specific component.
	 * \param c Pointer to the DocumentClassComponent object that has been used to update the class
	 */
	void getNewEdges(DocumentClassComponent *c);

	/*
	 * Fills the graph-related data structures inherited from the ProximityGraph class, using the results
	 * of the constructWithOpenCL() function.
	 */
	void buildGraph();


public:
	/*
	 * Constructor.
	 * Payload, Splitter and ProximityEvaluator member fields are all set to NULL and never used.
	 * OpenCL-related fields are left uninitialized.
	 */
	DocumentClass();

	/*
	 * Constructor.
	 * Payload, Splitter and ProximityEvaluator member fields are all set to NULL and never used.
	 * OpenCL-related fields are allocated.
	 * \param table_size The size of the hash table used for opencl operations.
	 */
	DocumentClass(std::size_t ts);


	/*
	 * Updates the document class with a new document. A precise update, iterates over the edges of both the 
	 * class graph and the document graph, while a non precise one iterates over the edges of the document graph only.
	 * The non precise operation is faster, but not strictly correct because the edges of the class graph that are not
	 * present on the document graph are left untouched, while their weights should be reduced.
	 * \param newDoc Pointer to the new document of the class.
	 * \param precise Determines whether it is going to be a precise update operation or not.
	 */
	void update(NGramGraph *newDoc, bool precise = false);


	/*
	 * Updates the document class with a new document, using a specific learning factor.
	 * \param newDoc Pointer to the new document of the class.
	 * \param l The learning factor to be used.
	 * \param precise Determines whether it is going to be a precise update operation or not.
	 */
	void update(NGramGraph *newDoc, double l, bool precise = false);


	/*
	 * Updates the document class with a new document, using an opencl kernel that executes on the GPU.
	 * \param component Pointer to the DocumentClassComponent object that represents the new document of the class
	 * \param context Pointer to the openCL context
	 * \param queue Pointer to the opencl CommandQueue to sumbit the kernel
	 * \param program Pointer to the opencl Program that contains the update kernel
	 */
	void updateWithOpenCL(DocumentClassComponent *component, Context *context, CommandQueue *queue, Program *program);


	/*
	 * Constructs the class graph from scratch given the directory of it's components(text files).
	 * It does so by calling continuously the update opencl kernel.
	 * \param componentsDir The directory containing the text files that constitute the class
	 * \param context Pointer to the OpenCL Context object
	 * \param queue Pointer to the OpenCL CommandQueue object to submit the update kernel
	 * \param program Pointer to the OpenCL Program object that contains the udpate kernel
	 */
	void constructWithOpenCL(std::string componentsDir, Context *context, CommandQueue *queue, Program *program);


	/*
	 * Returns a pointer to hash_table, to be used by OpenCL update operations.
	 */
	EDGE_WEIGHT_TYPE * getOCLTable() { return hash_table; }

	std::size_t getOCLTableSize() { return hash_table_size; }

};

#endif // DOCUMENT_CLASS_H
