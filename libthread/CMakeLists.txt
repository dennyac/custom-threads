cmake_minimum_required(VERSION 3.4.1)

add_library(thread SHARED TBC.cpp q.cpp thread.cpp)
target_link_libraries(thread stack timer)
add_subdirectory(tests)
