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

add_executable (maindirect test_direct.cpp)
add_executable (maingemm test_gemm.cpp)
add_executable (mainconv test_conv.cpp)

target_link_libraries (maindirect sc)
target_link_libraries (maingemm sc)
target_link_libraries (mainconv sc)
