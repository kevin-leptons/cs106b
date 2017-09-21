cmake_minimum_required(VERSION 3.0)
project(cs106b-project)

# flags
set(CMAKE_C_FLAGS "-std=gnu99 -Wall -g3 -lm -pthread")

# setup build directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# setup header files
include_directories(include)

# setup sub directory
add_subdirectory(src)
