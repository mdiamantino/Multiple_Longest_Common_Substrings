cmake_minimum_required(VERSION 3.17)
project(logest_common_substring)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
add_executable(logest_common_substring main.cpp LongestCommonPrefix.h SlidingWindow.h SuffixArray.cpp SuffixArray.h MultipleLongestCommonSubstring.h FileLCSWrapper.h SAIS2.h)