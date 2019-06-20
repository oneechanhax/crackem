
cmake_minimum_required(VERSION 2.6)
project(nekohack)

set(CMAKE_CXX_STANDARD 17)
file(GLOB_RECURSE NEKOHACK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(nekohack STATIC ${NEKOHACK_SOURCES})
target_include_directories(nekohack PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/hack")
target_include_directories(nekohack PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(nekohack PUBLIC dl)

add_library(nekohack-dummy SHARED "${CMAKE_CURRENT_SOURCE_DIR}/example/dummy.cpp")
add_executable(nekohack-example "${CMAKE_CURRENT_SOURCE_DIR}/example/main.cpp")
target_include_directories(nekohack-example PRIVATE nekohack)
target_link_libraries(nekohack-example PRIVATE nekohack nekohack-dummy)
