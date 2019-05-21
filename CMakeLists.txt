cmake_minimum_required (VERSION 3.10)

project (SC)

include_directories (${PROJECT_SOURCE_DIR}/src)

add_subdirectory (src)

# aux_source_directory (./test/, testsrc)

# add_executable (main ${testsrc})

add_executable (main test.cpp)

target_link_libraries (main sc)
