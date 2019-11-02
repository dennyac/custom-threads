cmake_minimum_required(VERSION 3.4.1)

add_executable(test_thread thread.cpp)
target_link_libraries(test_thread thread)
add_custom_command( TARGET test_thread POST_BUILD COMMAND test_thread)
