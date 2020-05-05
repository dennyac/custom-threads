cmake_minimum_required(VERSION 3.4.1)

include_directories(include)
add_library(stack SHARED direction.cpp new.cpp)
add_subdirectory(tests)
