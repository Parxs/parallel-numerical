cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)
project("MatrixMultiplication" C CXX)

set(CMAKE_CXX_STANDARD 11)


# Compiler Options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall -Werror -O3 -Wl,--no-as-needed")

find_package(Threads)


set(CMAKE_RUNETIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


# Files
list(APPEND SourceList "matrix_mult.cpp")

set(inFiles matrix_mult)

foreach(inFileName ${inFiles})
#=======================================================================
# Sequential
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/seq_helper.cpp)
  add_executable(seq_${inFileName} seq_helper.cpp ${SourceList})
  target_link_libraries(seq_${inFileName} util)
endif()

#=======================================================================
# C11
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/c11_helper.cpp)
  add_executable(c11_${inFileName} c11_helper.cpp ${SourceList})
  target_link_libraries(c11_${inFileName} util)
endif()

#=======================================================================
# OPENMP
include(FindOpenMP)
if(OPENMP_FOUND)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/omp_helper.cpp)
  add_executable(omp_${inFileName} omp_helper.cpp ${SourceList})
  target_link_libraries(omp_${inFileName} util)
endif()

endforeach(inFileName)
