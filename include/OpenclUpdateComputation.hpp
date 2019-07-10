#ifndef OPENCL_UPDATE_COMPUTATION
#define OPENCL_UDPATE_COMPUTATION

#include "OpenclComputation.hpp"

/*
 * An OpenclComputation-derived class. It defines the execution enviroment for the OpenCL kernel
 * that implements the update operation of n-gram graphs.
 */
class OpenclUpdateComputation : public OpenclComputation<void> {
private:
	/* Hash table of the document class */
	EDGE_WEIGHT_TYPE *class_table;

	/* Hash table of the text sample */
	EDGE_WEIGHT_TYPE *sample_table;

	/* Flag array where 1s indicate new edges that the sample brings to class */
	unsigned char *flags;

	/* Hash table size */
	std::size_t table_size;

	/* Learning factor of the update operation */
	float learning_factor;

	/* Implementation of the concrete steps defined by the base class */
	void allocateBuffers() override;
	void writeBuffers() override;
	void setKernelArguments() override;
	void submitKernel() override;
	void readBuffers() override;
	void computeResult() override;


public:
	/* Constructor */
	OpenclUpdateComputation(Context *c, CommandQueue *q, Program *p, EDGE_WEIGHT_TYPE *ct,
			EDGE_WEIGHT_TYPE *st, unsigned char *fl, std::size_t ts, float lf);
};

#endif // OPENCL_UPDATE_COMPUTATION
