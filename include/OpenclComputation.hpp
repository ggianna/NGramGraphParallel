#ifndef OPENCL_COMPUTATION
#define OPENCL_COMPUTATION

#include <string>
#ifdef __APPLE__
    #include <OpenCL/cl.hpp>
#else
    #include <CL/cl.hpp>
#endif // __APPLE__

using namespace cl;


#define EDGE_WEIGHT_TYPE float


/*
 * An abstract class to represent an opencl computation. Following the template method
 * design pattern, it defines the steps a computation with opencl must follow,
 * and leaves the implementation of these steps to the derived class.
 */
template <typename ReturnType>
class OpenclComputation {
protected:
	Context *context;
	CommandQueue *queue;
	Program *program;
	Kernel *kernel;
	std::string kernel_name;
	std::vector<Buffer> buffers;

	/* Loads/creates the computation kernel from program */
	virtual void loadKernel() {
		kernel = new Kernel(*program, kernel_name.c_str());
	}

	/* Allocate buffers on GPU memory */
	virtual void allocateBuffers() = 0;

	/* Write data on the buffers of GPU memory */
	virtual void writeBuffers() = 0;

	virtual void setKernelArguments() = 0;

	virtual void submitKernel() = 0;

	/* Read buffers that contain useful data to CPU ram */
	virtual void readBuffers() = 0;


	/* Compute the final result and return it */
	virtual ReturnType computeResult() = 0;


	/*
	 * Constructor
	 * \param c The context within the kernel is executed
	 * \param q The CommandQueue object to submit the kernel
	 * \param p The Program object that contains the kernel of this computation
	 */
	OpenclComputation(Context *c, CommandQueue *q, Program *p, int buf_no) : context(c), queue(q), program(p), buffers(buf_no) {}


public:
	/*
	 * Executes the computation.
	 * \return a value of type ReturnType as a result
	 */
	ReturnType apply() {
		loadKernel();
		allocateBuffers();
		writeBuffers();
		setKernelArguments();
		submitKernel();
		readBuffers();
		return computeResult();
	}
};

#endif // OPENCL_OPERATION
