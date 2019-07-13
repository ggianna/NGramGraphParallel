#ifndef OPENCL_UPDATE_COMPUTATION
#define OPENCL_UPDATE_COMPUTATION

#include "OpenclComputation.hpp"
#include "DocumentClassComponent.hpp"

// Forward declaration is needed, because DocumentClass and OpenclUpdateComputation refer to each other.
class DocumentClass;

/*
 * An OpenclComputation-derived class. It defines the execution enviroment for the OpenCL kernel
 * that implements the update operation of n-gram graphs.
 */
class OpenclUpdateComputation : public OpenclComputation<void> {
private:
	/* The DocumentClass object that is updated by the computation */
	DocumentClass *docClass;

	/* The DocumentClassComponent object that 'joins' the class */
	DocumentClassComponent *component;

	/* Size of tables */
	std::size_t table_size;

	/* Learning factor of the update operation */
	float learning_factor;

	/* Auxiliary fields that help to avoid useless buffer reads/writes when doing consecutive updates on a class */
	unsigned int currentUpdatesNo;
	unsigned int totalUpdatesNo;


	/* Implementation of the concrete steps defined by the base class */
	void allocateBuffers() override;
	void writeBuffers() override;
	void setKernelArguments() override;
	void submitKernel() override;
	void readBuffers() override;
	void computeResult() override;


public:
	/* Default constructor */
	OpenclUpdateComputation();

	/* Constructor */
	OpenclUpdateComputation(Context *c, CommandQueue *q, Program *p, DocumentClass *dc, DocumentClassComponent *dcc);

	void setOpenclFields(Context *c, CommandQueue *q, Program *p) {
		context = c;
		queue = q;
		program = p;
	}

	void setLearningFactor(float l) { learning_factor = l; }

	void setComponent(DocumentClassComponent *c) { component = c; }

	void setCurrentUpdatesNo(unsigned int c) { currentUpdatesNo = c; }

	void setTotalUpdatesNo(unsigned int t) { totalUpdatesNo = t; }

};

#endif // OPENCL_UPDATE_COMPUTATION
