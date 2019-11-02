cmake_minimum_required(VERSION 3.4.1)
include_directories(libstack/include)
add_subdirectory(libstack)

include_directories(libtimer/include)
add_subdirectory(libtimer)

include_directories(libthread/include)
add_subdirectory(libthread)