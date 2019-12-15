cmake_minimum_required(VERSION 2.8.9)
project (161044004)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -std=c++14 -std=c++17 -pthread")
add_executable(main main.cpp gtu.cpp)
