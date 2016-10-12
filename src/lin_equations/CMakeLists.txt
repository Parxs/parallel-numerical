cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)
project("Solving Linear Equations" C CXX)

set(CMAKE_CXX_STANDARD 11)


# Compiler Options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -std=c++11 -pthread -Wall -Werror -g -Wl,--no-as-needed")

find_package(Threads)

set(CMAKE_RUNETIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


# Files
list(APPEND SourceList "lin_equations.cpp")

list(APPEND LibrariesList "util")

set(inFiles gaussian jacobi)

foreach(inFileName ${inFiles})
#=======================================================================
# Sequential
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/seq_${inFileName}.cpp)
  add_executable(seq_${inFileName} seq_${inFileName}.cpp ${SourceList})
  target_link_libraries(seq_${inFileName} ${LibrariesList})
endif()

#=======================================================================
# C11
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/c11_${inFileName}.cpp)
  add_executable(c11_${inFileName} c11_${inFileName}.cpp ${SourceList})
  target_link_libraries(c11_${inFileName} ${LibrariesList})
endif()

#=======================================================================
# OPENMP
include(FindOpenMP)
if(OPENMP_FOUND)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/omp_${inFileName}.cpp)
  add_executable(omp_${inFileName} omp_${inFileName}.cpp ${SourceList})
  target_link_libraries(omp_${inFileName} ${LibrariesList})
endif()

endforeach(inFileName)
