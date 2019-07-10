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
	 * These are OpenCL-related fields
	 */
	std::size_t hash_table_size;
	EDGE_WEIGHT_TYPE *hash_table;
	vector<std::string> edge_labels;
	vector<unsigned long> hash_values;

	/*
	 * Computes and returns the learning factor based on the current number of constituents
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
	 * Constructor.
	 * Payload, Splitter and ProximityEvaluator member fields are all set to NULL and never used.
	 * OpenCL-related fields are allocated.
	 * \param table_size The size of the hash table used for opencl operations.
	 */
	DocumentClass(std::size_t ts);


	/*
	 * Updates the document class with a new document.
	 * \param newDoc Pointer to the new document of the class.
	 */
	void update(NGramGraph *newDoc);


	/*
	 * Updates the document class with a new document, using a specific learning factor.
	 * \param newDoc Pointer to the new document of the class.
	 * \param l The learning factor to be used.
	 */
	void update(NGramGraph *newDoc, double l);


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
};

#endif // DOCUMENT_CLASS_H
