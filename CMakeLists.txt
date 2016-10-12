cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)
#set(CMAKE_BUILD_TYPE "Release)

project("Parallel Numerical Receipts" C CXX)

#specify C++11
set(CMAKE_CXX_STANDARD 11)

# OpenMP 4.0 is needed which needs at least gcc 4.9
if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
    message(FATAL_ERROR "Requires at least gcc-4.9")
endif()

#set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall -Werror -g -Wl,--no-as-needed")


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/util)

#specify directory for shared libs
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
#specify directory for binaries
set(CMAKE_RUNETIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


add_subdirectory(src/util)
add_subdirectory(src/matrix)
add_subdirectory(src/sorting)
add_subdirectory(src/search)
add_subdirectory(src/lin_equations)
add_subdirectory(src/eigenvalue)

#add_subdirectory(src/polation)
#can't really be used for benchmarking as too fast/not enough things to parallelize
