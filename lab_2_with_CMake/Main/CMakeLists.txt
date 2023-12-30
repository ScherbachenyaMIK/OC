cmake_minimum_required (VERSION 3.26.0)

project (Main)

add_executable(Main main.cpp)

set_target_properties(Main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/target")