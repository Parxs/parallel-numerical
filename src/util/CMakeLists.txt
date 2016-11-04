cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)
project("Util Library" C CXX)

#specify C++11
set(CMAKE_CXX_STANDARD 11)

#set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall -Werror -o3 -Wl,--no-as-needed")

find_package(Boost)

find_path(BOOST_FILESYSTEM_INCLUDE_DIRS boost/optional.hpp)

add_library(util SHARED util.cpp)
