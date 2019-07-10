#include "OpenclUpdateComputation.hpp"

OpenclUpdateComputation::OpenclUpdateComputation(Context *c, CommandQueue *q, Program *p,
		EDGE_WEIGHT_TYPE *ct, EDGE_WEIGHT_TYPE *st, unsigned char *fl,
		std::size_t ts, float lf) :
	OpenclComputation(c, q, p, 3), class_table(ct), sample_table(st), flags(fl),
	table_size(ts), learning_factor(lf) {
		kernel_name = "update_ngg";
	}


void OpenclUpdateComputation::allocateBuffers() {
	buffers[0] = new Buffer(*context, CL_MEM_READ_WRITE, sizeof(EDGE_WEIGHT_TYPE) * table_size);
	buffers[1] = new Buffer(*context, CL_MEM_READ_ONLY, sizeof(EDGE_WEIGHT_TYPE) * table_size);
	buffers[2] = new Buffer(*context, CL_MEM_WRITE_ONLY, sizeof(unsigned char) * table_size);
}


void OpenclUpdateComputation::writeBuffers() {
	queue->enqueueWriteBuffer(*(buffers[0]), CL_TRUE, 0, sizeof(EDGE_WEIGHT_TYPE) * table_size, class_table);
	queue->enqueueWriteBuffer(*(buffers[1]), CL_TRUE, 0, sizeof(EDGE_WEIGHT_TYPE) * table_size, sample_table);
	// OpenCL buffers are not guaranteed to initialize to 0.
	queue->enqueueWriteBuffer(*(buffers[2]), CL_TRUE, 0, sizeof(unsigned char) * table_size, flags);
}


void OpenclUpdateComputation::setKernelArguments() {
	kernel->setArg(0, *(buffers[0]));
	kernel->setArg(1, *(buffers[1]));
	kernel->setArg(2, *(buffers[2]));
	kernel->setArg(3, table_size);
	kernel->setArg(4, learning_factor);
}


void OpenclUpdateComputation::submitKernel() {
	queue->enqueueNDRangeKernel(*kernel, NullRange, NDRange(256), NDRange(32));
}


void OpenclUpdateComputation::readBuffers() {
	queue->enqueueReadBuffer(*(buffers[0]), CL_TRUE, 0, sizeof(EDGE_WEIGHT_TYPE) * table_size, class_table);
	queue->enqueueReadBuffer(*(buffers[2]), CL_TRUE, 0, sizeof(unsigned char) * table_size, flags);
}


void OpenclUpdateComputation::computeResult() {}
