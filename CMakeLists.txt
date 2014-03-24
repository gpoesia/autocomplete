cmake_minimum_required(VERSION 2.6)

project(Autocompleter)

add_subdirectory(gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include)

add_executable(unit_tests unit_tests.cpp)
target_link_libraries(unit_tests gtest_main)
add_test(unit_tests test)
