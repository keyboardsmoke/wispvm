#pragma once

#include "platform.h"

typedef signed char int8;
typedef unsigned char uint8;

#if WISP_COMPILER == WISP_COMPILER_VS
typedef signed __int16 int16;
typedef unsigned __int16 uint16;
typedef signed __int32 int32;
typedef unsigned __int32 uint32;
typedef signed __int64 int64;
typedef unsigned __int64 uint64;
#else
typedef short int int16;
typedef unsigned short int uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long int int64;
typedef unsigned long int uint64;
#endif

#include <vector>
#include <string>
#include <functional>
#include <random>
#include <limits>