cmake_minimum_required (VERSION 2.6)

project(PROG3 C CXX)
include_directories(BEFORE /Rational/include)
link_directories(/Rational/lib)
#set(CMAKE_CXX_COMPILER "/Rational/bin/g++")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")


add_subdirectory(src)
