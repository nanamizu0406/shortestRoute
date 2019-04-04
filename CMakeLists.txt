cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions("-I/foo/dir1")
add_definitions("-DMACRO1=bar")

file(GLOB_RECURSE SRC_FILES "*.cpp") 
add_executable(cmake_test ${SRC_FILES})
target_link_libraries(cmake_test pthread)
