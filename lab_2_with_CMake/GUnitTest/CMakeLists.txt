cmake_minimum_required(VERSION 3.26.0)

project(GUnitTest)

include_directories(${CMAKE_SOURCE_DIR}/Child)
add_subdirectory(GoogleTest)

add_executable(GUnitTest test.cpp)
target_link_libraries(GUnitTest PRIVATE MergeSort gtest gtest_main)

enable_testing()

add_test(NAME GUnitTest COMMAND GUnitTest)