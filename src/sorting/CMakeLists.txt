cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)
project("Sorting-Algorithms" C CXX)

set(CMAKE_CXX_STANDARD 11)

# OpenMP 4.0 is needed which needs at least gcc 4.9
if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
    message(FATAL_ERROR "Requires at least gcc-4.9")
endif()


# Compiler Options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall -Werror -o3 -Wl,--no-as-needed")

find_package(Threads)





# Files
list(APPEND SourceList "sorting.cpp" )

set(inFiles selectionsort bubblesort quicksort countingsort)

foreach(inFileName ${inFiles})


#=======================================================================
# Sequential
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/seq_${inFileName}.cpp)
  add_executable(seq_${inFileName} seq_${inFileName}.cpp ${SourceList})
  target_link_libraries(seq_${inFileName} util)
endif()


#=======================================================================
# C11
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/c11_${inFileName}.cpp)
  add_executable(c11_${inFileName} c11_${inFileName}.cpp ${SourceList})
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
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/omp_${inFileName}.cpp)
  add_executable(omp_${inFileName} omp_${inFileName}.cpp ${SourceList})
  target_link_libraries(omp_${inFileName} util)
endif()





endforeach(inFileName)
