// #elifdef is a C++23 preprocessor directive, and may not be compatible with older versions.
// We probably won't need to use integer precision anyways.

// #ifdef USE_INTEGER_PRECISION
//     using num = int;
// #elifdef USE_SINGLE_PRECISION

#ifdef USE_SINGLE_PRECISION
    using num = float;
#else
    using num = double;
#endif