#ifndef OPENCLPARAMS_H
#define OPENCLPARAMS_H


#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 200
#define CL_TARGET_OPENCL_VERSION 200

#ifdef __APPLE__
    #include <OpenCL/opencl.hpp>
#else
    #include <CL/opencl.hpp>
#endif // __APPLE__

using namespace cl;

#endif //OPENCLPARAMS
