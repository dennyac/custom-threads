cmake_minimum_required(VERSION 3.4.1)

add_executable(test_stack stack.cpp)
target_link_libraries(test_stack stack)
add_custom_command( TARGET test_stack POST_BUILD COMMAND test_stack)
