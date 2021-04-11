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

template<typename T>
class MultipleLongestCommonSubstr {

private:
    std::vector<std::vector<T>> _files_arrays;
    int _alphabet_length = 256;
    int _n_sentinels{};
    int _text_length{};
    int _min_symbol_value{};
    int _shift{};
    int _k = 2;
    std::vector<int> _sarray{};
    std::vector<int> _combined_text;
    std::unordered_map<int, int> _type_from_char_pos;
    std::vector<std::tuple<int, int, int>> _longest_comon_substrings_indices;

    void InitializeTextProperties() {
        std::set<T> char_map;
        _n_sentinels = _files_arrays.size();
        for (int i = 0, char_type = 0; i < _n_sentinels; i++) {
            std::vector<T> curr_txt = _files_arrays[i];
            int curr_txt_size = curr_txt.size();
            _text_length += curr_txt_size;
            for (int c = 0; c < curr_txt_size; c++) {
                char_map.insert(curr_txt[c]);
                _type_from_char_pos[char_type++] = i;
            }
            _type_from_char_pos[char_type++] = i;
        }
        _text_length += _n_sentinels; // Total text length also include sentinels (Eg. {ab,er,zer} -> ab$er!zer&)
        _alphabet_length = (int) char_map.size() + _n_sentinels;
        _min_symbol_value = *char_map.begin();
        _shift = _n_sentinels - _min_symbol_value;
    }

    void BuildCombinedText() {
        int i;
        for (i = 0; i < _n_sentinels; i++) {
            for (auto &chr : _files_arrays[i]) {
                _combined_text.push_back(chr + _shift);
            }
            _combined_text.push_back(i);
        }
    }

    bool IsKDifferentTypesInWindow(std::set<int> &types_in_current_window, int lowest_index, int highest_index) {
        types_in_current_window.clear();
        for (int i = lowest_index; i <= highest_index; i++) {
            types_in_current_window.insert(_type_from_char_pos[_sarray[i]]);
        }
        return types_in_current_window.size() >= _k;
    }

    void FindMostCommonSubStrings() {
        InitializeTextProperties();
        BuildCombinedText();
        _sarray = ComputeSuffixArray(_combined_text, _alphabet_length);
        std::vector<int> lcp_array = ComputeLongestCommonPrefix(_combined_text, _sarray);
        RunSlidingWindow(lcp_array);
    }

    int ConvertPositionInCombinedTextToPosInString(int string_num, int index_in_combined_text) {
        int i = 0;
        while (_type_from_char_pos[i] != string_num) {
            i++;
        }
        assert(index_in_combined_text >= i);
        return index_in_combined_text - i;
    }

    void RunSlidingWindow(const std::vector<int> &lcp_array) {
        std::set<int> types_in_current_window;
        int lowest_index = _n_sentinels, highest_index = lowest_index;
        SlidingWindow sliding_window(lcp_array, lowest_index + 1, highest_index + 1);
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
            if (lowest_index == _text_length - 1) break;
            if (lowest_index != highest_index ||
                IsKDifferentTypesInWindow(types_in_current_window, lowest_index, highest_index)) {
                int min_lcp = sliding_window.ExtractMin();
                if (min_lcp > 0) {
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

public:
    explicit MultipleLongestCommonSubstr(const std::vector<std::vector<T>> &v) : _files_arrays(std::move(v)) {
        FindMostCommonSubStrings();
    }

    std::tuple<int, std::vector<std::map<int,int>>> ComputeResultsStats() {
        // For each longest substring
        std::tuple<int, std::vector<std::map<int,int>>> results;
        std::vector<std::map<int,int>> results_string_type_offset;
        int length;
        for (std::tuple<int, int, int> indices : _longest_comon_substrings_indices) {
            int lowest_index = std::get<0>(indices);
            int highest_index = std::get<1>(indices);
            length = std::get<2>(indices);
            std::map<int, int> map_file_offset;
            for (int i = lowest_index; i <= highest_index; i++) {
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

    void PrintResults() {
        for (std::tuple<int, int, int> indices : _longest_comon_substrings_indices) {
            int lowest_index = std::get<0>(indices);
            int length = std::get<2>(indices);
            std::vector<char> word;
            word.reserve(length);
            int i = _sarray[lowest_index];
            std::cout << length << std::endl;
            for (int j = 0; j < length; j++) {
                word.push_back((char) (_combined_text[i + j] - _shift));
            }
            std::string string(word.begin(), word.end());
            std::cout << string << std::endl;
        }
    }
};

#endif //LOGEST_COMMON_SUBSTRING_MULTIPLELONGESTCOMMONSUBSTRING_H
