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

<<<<<<< HEAD
# stb_image_write library
add_library(stb_image_write STATIC lib/stb_image_write.cpp)
target_include_directories(stb_image_write PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Suppress warnings for stb_image_write
target_compile_options(stb_image_write PRIVATE
    -Wno-old-style-cast
    -Wno-sign-conversion
    -Wno-implicit-int-conversion
    -Wno-missing-field-initializers
    -Wno-zero-as-null-pointer-constant
    -Wno-conversion
)

add_executable(static_renderer src/static_renderer.cpp src/classes/vec3.cpp)

# Link the stb_image_write library
target_link_libraries(static_renderer PRIVATE stb_image_write)

# Ensure the include path for stb_image_write is visible
target_include_directories(static_renderer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
=======
add_executable(static_renderer src/static_renderer.cpp)
>>>>>>> d260ed6 (Simple ray caster (#2))
