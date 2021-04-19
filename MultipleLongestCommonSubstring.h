#ifndef LOGEST_COMMON_SUBSTRING_MULTIPLELONGESTCOMMONSUBSTRING_H
#define LOGEST_COMMON_SUBSTRING_MULTIPLELONGESTCOMMONSUBSTRING_H


#include <utility>
#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include <cassert>
#include <iostream>
#include <map>
#include "LongestCommonPrefix.h"
#include "SlidingWindow.h"
#include "SuffixArray.h"

/**
 * Performs the research of longest common substrings
 * in multiple strings (vectors of elements of type T).
 * Complexity: O(n)
 * Based on paper : "Computing Longest Common Substrings Via Suffix Arrays"
 */
template<typename T>
class MultipleLongestCommonSubstr final {

private:
    std::vector<std::vector<T>> _files_arrays;
    int _alphabet_length = 256;
    int _n_sentinels{}; // A sentinel is a special symbol palced at the end of each string
    int _text_length{};
    int _shift{};
    int _k = 2;
    std::vector<int> _sarray{};
    std::vector<int> _combined_text;
    std::unordered_map<int, int> _type_from_char_pos;
    std::vector<std::tuple<int, int, int>> _longest_comon_substrings_indices;
    std::vector<int> _lcp_array;

    /**
     * Compute values required to start the research of substirngs:
     * - _n_sentinels
     * - _text_length
     * - _alphabet_length (required to improve SA-IS performances)
     * - _shift (will be used to shift int values representing each char so that the first sentil values starts at one)
     */
    void InitializeTextProperties() {
        std::set<T> char_map;
        _n_sentinels = _files_arrays.size();
        for (std::size_t i = 0, char_type = 0; i < _n_sentinels; ++i) {
            std::vector<T> curr_txt = _files_arrays[i];
            int curr_txt_size = curr_txt.size();
            _text_length += curr_txt_size;
            for (std::size_t c = 0; c < curr_txt_size; ++c) {
                char_map.insert(curr_txt[c]);
                _type_from_char_pos[char_type++] = i;
            }
            _type_from_char_pos[char_type++] = i;
        }
        _text_length += _n_sentinels; // Total text length also include sentinels (Eg. {ab,er,zer} -> ab$er!zer&)
        _alphabet_length = char_map.size() + _n_sentinels;
        int _min_symbol_value = *char_map.begin();
        _shift = _n_sentinels - _min_symbol_value;
    }

    /**
     * Each string (trated in the search of the substrings)
     * is combined in a single string using separators (called sentinels)
     * AND each char - int value - is shifted so that the sentinels values start at 0
     * Example: {"ABC", "BCD", "ABBD"}  -> "ABC&BCD!ABBD§"
     * where '&', '!', '§' are sentinels and have values [0,1,2]
     * and characters are shifted (for instance A : int(65) could became 65 - 3)
     */
    void BuildCombinedText() {
        for (std::size_t i = 0; i < _n_sentinels; ++i) {
            for (auto &chr : _files_arrays[i]) {
                _combined_text.push_back(chr + _shift);
            }
            _combined_text.push_back(i);
        }
    }


    /**
     * Converts an index in the combined text, to an index in the original string.
     * Example:
     *      INPUT: "ABC&BCD!ABBD§", string_num=1 index_in_combined_text=5 (indicating letter C in BCD)
     *      OUTPUT: 1 because C is at position 1 in BCD
     * @param string_num Type (numberd from 0 to #original strings)
     * @param index_in_combined_text Index of first char of substring (in combined text)
     * @return Index in the string
     */
    int ConvertPositionInCombinedTextToPosInString(int string_num, int index_in_combined_text) {
        int i = 0;
        while (_type_from_char_pos[i] != string_num) {
            ++i;
        }
        assert(index_in_combined_text >= i);
        return index_in_combined_text - i;
    }

    /**
     * Verifies that the window contains at least K suffixes
     * of different types (meaning of originated by K different original string)
     * Example:
     * Text: "ABC&BCD!ABBD§",
     * window : { "BCD!ABBD§", "ABBD§"} contains K=2 different types
     * window : { "BCD!ABBD§", "D!ABBD§"} contains K=1 type
     */
    bool IsKDifferentTypesInWindow(std::set<int> &types_in_current_window, int lowest_index, int highest_index) {
        types_in_current_window.clear();
        for (auto i = lowest_index; i <= highest_index; ++i) {
            types_in_current_window.insert(_type_from_char_pos[_sarray[i]]);
        }
        return types_in_current_window.size() >= _k;
    }

    /**
     * From the array of longest common prefixes and the suffixes array, finds the longest substring
     * Complexity : O(n)
     * Description of the algorithm:
     *      1. Build a window that covers the first suffix
     *      2. Advance or shrink the window so to have at least K suffixes of different type
     *      3. Asks the the SlidingWindow for the suffix with minimum LCP value,
     *         this corresponds to a substring of the suffixes in the window.
     *         NOTE: First suffix in the window not considered !=
     *      4. Only keeps the longest.
     */
    void RunSlidingWindow() {
        std::set<int> types_in_current_window;
        int lowest_index = _n_sentinels, highest_index = lowest_index;
        SlidingWindow sliding_window(_lcp_array, lowest_index + 1, highest_index + 1);
        int current_longest_length = INT32_MIN;
        while (true) {
            bool shrinkWindow =
                    highest_index == _text_length - 1 ||
                    IsKDifferentTypesInWindow(types_in_current_window, lowest_index, highest_index);
            if (shrinkWindow) {
                sliding_window.ShrinkWindow();
                lowest_index++;
            } else {
                sliding_window.AdvanceWindow();
                highest_index++;
            }
            if (lowest_index == _text_length - 1) break; // Because we always look for the minimum from the next suffix
            if (lowest_index != highest_index ||
                IsKDifferentTypesInWindow(types_in_current_window, lowest_index, highest_index)) {
                int min_lcp = sliding_window.ExtractMin();
                if (min_lcp > 0) { // Only interested if is a substring of another suffix
                    if (min_lcp > current_longest_length) {
                        // A new longest string, hence we do not need previous anymore !
                        current_longest_length = min_lcp;
                        _longest_comon_substrings_indices.clear();
                    }
                    if (min_lcp == current_longest_length) {
                        // Another substring of same length of the previous longest substring is found
                        _longest_comon_substrings_indices.emplace_back(lowest_index, highest_index, min_lcp);
                    }
                }
            }
        }
    }

    /**
     * Main method
     * Complexity : O(n)
     */
    void FindMostCommonSubStrings() {
        InitializeTextProperties();
        BuildCombinedText();
        _sarray = ComputeSuffixArray(_combined_text, _alphabet_length); // O(n)
        _lcp_array = ComputeLongestCommonPrefix(_combined_text, _sarray); // O(n)
        RunSlidingWindow(); // O(n)
    }

public:
    explicit MultipleLongestCommonSubstr(const std::vector<std::vector<T>> &v) : _files_arrays(std::move(v)) {
        FindMostCommonSubStrings();
    }

    /**
     * Results are converted in a better form : tuple(length, vector[ res1, res2, ... ])
     * where res is of the form:
     *      hashmap[(file1_containing_substring, offset_in_file1), (file2_containing_substring, offset_in_file2), ...]
     * Note that the length is the same - if there are multiple results.
     */
    std::tuple<int, std::vector<std::map<int, int>>> GetResultsStats() {
        // For each longest substring
        std::tuple<int, std::vector<std::map<int, int>>> results;
        std::vector<std::map<int, int>> results_string_type_offset;
        int length;
        for (std::tuple<int, int, int> indices : _longest_comon_substrings_indices) {
            int lowest_index = std::get<0>(indices);
            int highest_index = std::get<1>(indices);
            length = std::get<2>(indices);
            std::map<int, int> map_file_offset;
            for (size_t i = lowest_index; i <= highest_index; ++i) {
                int type = _type_from_char_pos[_sarray[i]];
                if (map_file_offset.find(type) == map_file_offset.end()) {
                    int offset = ConvertPositionInCombinedTextToPosInString(type, _sarray[i]);
                    map_file_offset[type] = offset;
                }
            }
            results_string_type_offset.push_back(map_file_offset);
        }
        results = std::make_tuple(length, results_string_type_offset);
        return results;
    }

    /**
     * Displays found longest substrings.
     */
    [[maybe_unused]] void PrintResults() {
        for (std::tuple<int, int, int> indices : _longest_comon_substrings_indices) {
            int lowest_index = std::get<0>(indices);
            int length = std::get<2>(indices);
            std::vector<char> word;
            word.reserve(length);
            int i = _sarray[lowest_index];
            std::cout << length << std::endl;
            for (std::size_t j = 0; j < length; ++j) {
                word.push_back((_combined_text[i + j] - _shift));
            }
            std::string string(word.begin(), word.end());
            std::cout << string << std::endl;
        }
        if (_longest_comon_substrings_indices.empty()) {
            std::cout << "No longst common substring found !" << std::endl;
        }
    }
};

#endif //LOGEST_COMMON_SUBSTRING_MULTIPLELONGESTCOMMONSUBSTRING_H
