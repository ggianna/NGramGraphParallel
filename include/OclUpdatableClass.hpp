#ifndef OCL_UPDATABLE_CLASS
#define OCL_UPDATABLE_CLASS

#include <vector>
#include <string>
#ifdef __APPLE__
    #include <OpenCL/opencl.hpp>
#else
    #include <CL/opencl.hpp>
#endif

#include "DocumentClass.hpp"
#include "DocumentClassComponent.hpp"
#include "OpenclUpdateComputation.hpp"


class OclUpdatableClass : public DocumentClass {
private:
	/*
	 * EDGE_WEIGHT_TYPE array that serves as hash table and it's size.
	 */
	std::size_t hash_table_size;
	EDGE_WEIGHT_TYPE *hash_table;


	/*
	 * A vector to hold edge labels as they are added by components.
	 */
	vector<std::string> edge_labels;


	/*
	 * A vector to hold hash values of edge labels.
	 */
	vector<unsigned long> hash_values;


	/*
	 * An OpenclUpdateCompuation object to come in handy when updating the class with OpenCL.
	 */
	OpenclUpdateComputation oclUpdateComp;


	/*
	 * Gets new edges from a component and updates the related fields of the class.
	 * This function is called after an OpenclUpdateComputation has completed it's execution.
	 * \param c Pointer to the DocumentClassComponent object that has been used to update the class
	 */
	void getNewEdges(DocumentClassComponent *c);


	/*
	 * Fills the graph-related data structures inherited from the ProximityGraph class, using the results
	 * produced by the buildClass() function.
	 */
	void buildGraph();


	/*
	 * TODO Make a different class for this purpose.
	 * Measures elapsed time between two time instances. Used to profile the buildClass() function.
	 * \param start The beginning of the time interval
	 * \param end The end of the time interval
	 */
	double compute_elapsed_time(struct timespec *start, struct timespec *end);


public:
	/*
	 * Constructor.
	 * Payload, Splitter and ProximityEvaluator member fields are all set to NULL and never used.
	 * OpenCL-related fields are allocated.
	 * \param table_size The size of the hash table used by OpenCL.
	 */
	OclUpdatableClass(std::size_t ts);


	/*
	 * Updates the document class with a new document, using an opencl kernel that executes on the GPU.
	 * \param component Pointer to the DocumentClassComponent object that represents the new document of the class
	 * \param context Pointer to the openCL context
	 * \param queue Pointer to the opencl CommandQueue to sumbit the kernel
	 * \param program Pointer to the opencl Program that contains the update kernel
	 */
	void singleUpdate(DocumentClassComponent *component, Context *context, CommandQueue *queue, Program *program);


	/*
	 * Constructs the class graph from scratch given the directory of it's components(text files).
	 * \param componentsDir The directory containing the text files that constitute the class
	 * \param context Pointer to the OpenCL Context object
	 * \param queue Pointer to the OpenCL CommandQueue object to submit the update kernel
	 * \param program Pointer to the OpenCL Program object that contains the udpate kernel
	 */
	void buildClass(std::string componentsDir, Context *context, CommandQueue *queue, Program *program);


	/*
	 * Profiles the buildClass() function.
	 * \return A map containing the execution time of the various tasks performed by the buildClass() function
	 */
	std::map<std::string, double> profileClassBuilding(std::string dir, Context *con, CommandQueue *queue, Program *prog);


	/*
	 * Returns a pointer to hash_table, to be used by OclUpdateComputation.
	 */
	EDGE_WEIGHT_TYPE * getOCLTable() { return hash_table; }


	/*
	 * Returns the hash table size.
	 */
	std::size_t getOCLTableSize() { return hash_table_size; }
};

#endif // OCL_UPDATABLE_CLASS
