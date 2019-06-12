
cmake_minimum_required(VERSION 2.6)
project(nekohack)

set(CMAKE_CXX_STANDARD 17)
file(GLOB_RECURSE NEKOHACK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(nekohack STATIC ${NEKOHACK_SOURCES})
target_include_directories(nekohack PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/break")
target_include_directories(nekohack PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
