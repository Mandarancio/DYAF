cmake_minimum_required(VERSION 2.8)

project(DYAF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra")

add_executable(DYAF src/main.cpp src/dyaf.cpp src/scidown.cpp)
