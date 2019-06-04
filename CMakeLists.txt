cmake_minimum_required (VERSION 3.10)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
add_compile_options(-std=c++11)
add_compile_options(-g)

project (SC)

include_directories (${PROJECT_SOURCE_DIR}/src)

add_subdirectory (src)

# aux_source_directory (./test/, testsrc)

# add_executable (main ${testsrc})

add_executable (main test_dense.cpp)

target_link_libraries (main sc)
