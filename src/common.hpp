// #elifdef is a C++23 preprocessor directive, and may not be compatible with older versions.
// We probably won't need to use integer precision anyways.

// #ifdef USE_INTEGER_PRECISION
// using num = int;
// #elifdef USE_SINGLE_PRECISION

#ifndef COMMON_H
#define COMMON_H

#include <limits>
#include <string>
#include <sstream>

#ifdef USE_SINGLE_PRECISION
using num = float;
#else
using num = double;
#endif

using StringStream = std::ostringstream;

extern bool debug;
static constexpr num infinity = std::numeric_limits<num>::infinity();
static constexpr num epsilon = 1.E-10;

#endif // COMMON_H