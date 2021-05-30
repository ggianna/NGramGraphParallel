#ifndef C_INTERFACE_HPP
#define C_INTERFACE_HPP
#ifdef __cplusplus
extern "C" {
#endif
#include "C_HandleGraphRequest.hpp"
#include "string.h"
int Request(const char* msg);
#ifdef __cplusplus
}
#endif
#endif
