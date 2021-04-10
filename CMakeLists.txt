cmake_minimum_required(VERSION 3.17)
project(logest_common_substring)

set(CMAKE_CXX_STANDARD 20)

add_executable(logest_common_substring main.cpp sais.h MultipleLongestCommonSubstring.h longestCommonPrefix.h SlidingWindow.h)