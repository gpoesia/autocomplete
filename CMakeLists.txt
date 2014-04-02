cmake_minimum_required(VERSION 2.6)

project(Autocompleter)

set(CMAKE_CXX_FLAGS "-Wall -Weffc++ -g -std=c++11")

add_subdirectory(gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include)

add_executable(unit_tests
               stringlist.cpp
               tests/test_string_list.cpp)

target_link_libraries(unit_tests gtest_main)
add_test(unit_tests test)
