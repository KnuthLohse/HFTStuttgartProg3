cmake_minimum_required (VERSION 2.6)

project (OMPIMON C CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")


add_subdirectory(src)
