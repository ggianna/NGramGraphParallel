#include "OclUpdatableClass.hpp"
#include "OpenclUpdateComputation.hpp"


OpenclUpdateComputation::OpenclUpdateComputation(Context *c, CommandQueue *q, Program *p,
		OclUpdatableClass *dc, DocumentClassComponent *dcc) :
	OpenclComputation(c, q, p, 0), docClass(dc), component(dcc) {
		kernel_name = "update_ngg";
		table_size = docClass->getOCLTableSize();
	}

OpenclUpdateComputation::OpenclUpdateComputation() : OpenclComputation(nullptr, nullptr, nullptr, 0),
	docClass(nullptr), component(nullptr) {
		kernel_name = "update_ngg";
	}

void OpenclUpdateComputation::allocateBuffers() {
	if (buffers.size() == 0) {
		buffers.push_back(Buffer(*context, CL_MEM_READ_WRITE, sizeof(EDGE_WEIGHT_TYPE) * table_size));
		buffers.push_back(Buffer(*context, CL_MEM_READ_ONLY, sizeof(EDGE_WEIGHT_TYPE) * table_size));
		buffers.push_back(Buffer(*context, CL_MEM_WRITE_ONLY, sizeof(unsigned char) * table_size));
	}
}


void OpenclUpdateComputation::writeBuffers() {
	if (currentUpdatesNo == 0) {
		queue->enqueueWriteBuffer(buffers[0], CL_TRUE, 0, sizeof(EDGE_WEIGHT_TYPE) * table_size, docClass->getOCLTable());
	}
	queue->enqueueWriteBuffer(buffers[1], CL_TRUE, 0, sizeof(EDGE_WEIGHT_TYPE) * table_size, component->getHashTable());
	queue->enqueueWriteBuffer(buffers[2], CL_TRUE, 0, sizeof(unsigned char) * table_size, component->getFlags());
}


void OpenclUpdateComputation::setKernelArguments() {
	kernel->setArg(0, buffers[0]);
	kernel->setArg(1, buffers[1]);
	kernel->setArg(2, buffers[2]);
	kernel->setArg(3, table_size);
	kernel->setArg(4, learning_factor);
}


void OpenclUpdateComputation::submitKernel() {
	queue->enqueueNDRangeKernel(*kernel, NullRange, NDRange(256), NDRange(32));
	queue->finish();
	++currentUpdatesNo;
}


void OpenclUpdateComputation::readBuffers() {
	if (currentUpdatesNo == totalUpdatesNo) {
		queue->enqueueReadBuffer(buffers[0], CL_TRUE, 0, sizeof(EDGE_WEIGHT_TYPE) * table_size, docClass->getOCLTable());
	}
	queue->enqueueReadBuffer(buffers[2], CL_TRUE, 0, sizeof(unsigned char) * table_size, component->getFlags());
}


void OpenclUpdateComputation::computeResult() {
}
