cmake_minimum_required(VERSION 3.26.0)

project(Child)

add_library(MergeSort STATIC MergeSort.cpp)

add_executable(Child child.cpp)

target_link_libraries(Child MergeSort)

set_target_properties(Child PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/target")