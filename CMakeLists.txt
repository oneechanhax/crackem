
cmake_minimum_required(VERSION 3.14)
project(crackem)

set(CRACKEM_BUILD_SHARED OFF CACHE BOOL "Build the shared library")
set(CRACKEM_BUILD_STATIC ON CACHE BOOL "Build the static library")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/")
