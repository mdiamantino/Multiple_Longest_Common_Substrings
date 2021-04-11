cmake_minimum_required(VERSION 3.16)
project(longest_common_substring)

set(GCC_COVERAGE_COMPILE_FLAGS "-masm=intel -fconcepts -mlong-double-128 -ggdb3 -Wpedantic -Wall -Wextra -Wconversion -Wsign-conversion -Weffc++ -Wstrict-null-sentinel -Wold-style-cast -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wsign-promo -Wzero-as-null-pointer-constant -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override -lquadmath")
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS}")
add_executable(longest_common_substring main.cpp LongestCommonPrefix.h SlidingWindow.h MultipleLongestCommonSubstring.h FileLCSWrapper.h SuffixArray.h)