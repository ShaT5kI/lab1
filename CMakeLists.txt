cmake_minimum_required(VERSION 3.17)
project(lab1)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(lib/googletest)
include_directories(lib/googletest/googletest/include)

add_executable(lab1 main.cpp trit.cpp trit.h tritSet.cpp tritSet.h tests.cpp)
target_link_libraries(lab1 gtest gtest_main)