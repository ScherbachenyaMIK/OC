cmake_minimum_required(VERSION 3.26.0)

project(Parent)

add_executable(Parent parent.cpp)

set_target_properties(Parent PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/target")