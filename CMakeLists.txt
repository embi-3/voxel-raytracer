# ------------------------------------------------------------ #
# -------------- DO NOT TOUCH BELOW THIS LINE ---------------- #
# ------------------------------------------------------------ #

cmake_minimum_required(VERSION 3.20)

project(VOXEL_RAYTRACER LANGUAGES CXX)

# we are using C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  # compiler flags for MSVC
  add_compile_options(
    /W4      # highest warning level
    /WX      # treat warnings as errors
    /permissive-   # stricter C++ standard conformance
  )
else()
  # compiler flags for gcc/clang
  add_compile_options(
    -Wall
    -Wextra
    -Werror
    -pedantic-errors
    -Wconversion
    -Wsign-conversion
    -Wdouble-promotion
    -Wcast-align
    -Wformat=2
    -Wuninitialized
    -Wnull-dereference
    -Wnon-virtual-dtor
    -Woverloaded-virtual
    -Wdeprecated-copy-dtor
    -Wold-style-cast
    -Wzero-as-null-pointer-constant
    -Wsuggest-override
    -fstack-protector-strong
  )
endif()

# sanitisers for gcc/clang
if(NOT MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG_INIT "-fsanitize=address,undefined")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-fsanitize=address,undefined")
  set(CMAKE_CXX_EXE_LINKER_FLAGS_DEBUG_INIT "-fsanitize=address,undefined")
  set(CMAKE_CXX_EXE_LINKER_FLAGS_RELWITHDEBINFO_INIT "-fsanitize=address,undefined")
endif()

# ------------------------------------------------------------ #
# -------------- DO NOT MODIFY ABOVE THIS LINE --------------- #
# ------------------------------------------------------------ #

add_executable(voxel-raytracer src/voxel-raytracer.cpp)
