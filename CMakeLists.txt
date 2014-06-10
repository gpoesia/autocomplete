cmake_minimum_required(VERSION 2.6)

project(Autocompleter)

set(CMAKE_CXX_FLAGS "-Wall -Weffc++ -g -std=c++11 -pthread")

add_subdirectory(gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include)

add_executable(unit_tests
               stringlist.cpp
               autocompleteprotocol.cpp
               connectionpeer.cpp
               autocompleteclient.cpp
               autocompleteserver.cpp
               loadbalancer.cpp
               tests/test_string_list.cpp
               tests/test_autocomplete_protocol.cpp
               tests/test_connection_peer.cpp)

add_executable(client_simulator
               clientsimulator.cpp
               stringlist.cpp
               autocompleteprotocol.cpp
               connectionpeer.cpp
               autocompleteclient.cpp)

add_executable(loadbalancer
               loadbalancingserver.cpp
               stringlist.cpp
               autocompleteprotocol.cpp
               connectionpeer.cpp
               loadbalancer.cpp)

add_executable(server
               server.cpp
               stringlist.cpp
               autocompleteprotocol.cpp
               connectionpeer.cpp
               autocompleteserver.cpp)

target_link_libraries(unit_tests gtest_main)
add_test(unit_tests test)
