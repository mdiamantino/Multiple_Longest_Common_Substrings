cmake_minimum_required(VERSION 3.16)
project(longest_common_substring)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
add_executable(longest_common_substring main.cpp LongestCommonPrefix.h SlidingWindow.h MultipleLongestCommonSubstring.h FileLCSWrapper.h SuffixArray.h)