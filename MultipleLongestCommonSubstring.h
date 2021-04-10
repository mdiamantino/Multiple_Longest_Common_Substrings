#ifndef LOGEST_COMMON_SUBSTRING_MULTIPLELONGESTCOMMONSUBSTRING_H
#define LOGEST_COMMON_SUBSTRING_MULTIPLELONGESTCOMMONSUBSTRING_H


#include <utility>
#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include <iostream>
#include "SuffixArray.h"
#include "longestCommonPrefix.h"
#include "SlidingWindow.h"

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
    std::unordered_map<int, int> type_from_char_pos;
    std::vector<std::tuple<int, int>> _longest_comon_substrings_indices;

public:
    explicit MultipleLongestCommonSubstr(std::vector<std::vector<T>> v) : _files_arrays(std::move(v)) {}

    void initialize_text_properties() {
        std::set<T> char_map;
        _n_sentinels = _files_arrays.size();
        for (int i = 0, char_type = 0; i < _n_sentinels; i++) {
            std::vector<T> curr_txt = _files_arrays[i];
            int curr_txt_size = curr_txt.size();
            _text_length += curr_txt_size;
            for (int c = 0; c < curr_txt_size; c++) {
                char_map.insert(curr_txt[c]);
                type_from_char_pos[char_type++] = i;
            }
            type_from_char_pos[char_type++] = i;
        }
        _text_length += _n_sentinels; // Total text length also include sentinels (Eg. {ab,er,zer} -> ab$er!zer&)
        _alphabet_length = (int) char_map.size() + _n_sentinels;
        _min_symbol_value = *char_map.begin();
        _shift = _n_sentinels - _min_symbol_value;
    }

    void build_combined_shifted_text() {
        int i;
        for (i = 0; i < _n_sentinels; i++) {
            for (auto &chr : _files_arrays[i]) {
                _combined_text.push_back(chr + _shift);
            }
            _combined_text.push_back(i);
        }
    }

    bool enoughUniqueColorsInWindow(std::set<int> &types_in_current_window, int lowest_index, int highest_index) {
        types_in_current_window.clear();
        for (int i = lowest_index; i <= highest_index; i++) {
            types_in_current_window.insert(type_from_char_pos[_sarray[i]]);
        }
        return types_in_current_window.size() == _k;
    }

    void print_results() {
        for (std::tuple<int, int> indices : _longest_comon_substrings_indices) {
            std::vector<char> word;
            word.reserve(std::get<1>(indices));
            int i = _sarray[std::get<0>(indices)];
            for (int j = 0; j < std::get<1>(indices); j++) {
                word.push_back((char) (_combined_text[i + j] - _shift));
            }
            std::string string(word.begin(), word.end());
            std::cout << string << std::endl;
        }
    }

    void solve() {
        std::set<int> types_in_current_window;
        initialize_text_properties();
        build_combined_shifted_text();
        SuffixArray sa(_sarray, _combined_text, _alphabet_length);

        std::vector<int> lcp_array = longest_common_prefix_from_suffix_array(_combined_text, _sarray);


        int lowest_index = _n_sentinels, highest_index = lowest_index;
        SlidingWindow sliding_window(lcp_array, lowest_index + 1, highest_index + 1);
        int current_longest_length = INT32_MIN;
        while (true) {
            bool shrinkWindow =
                    highest_index == _text_length - 1 ||
                    enoughUniqueColorsInWindow(types_in_current_window, lowest_index, highest_index);
            if (shrinkWindow) {
                sliding_window.shrink();
                lowest_index++;
            } else {
                sliding_window.advance();
                highest_index++;
            }
            if (lowest_index == _text_length - 1) break;
            if (lowest_index != highest_index ||
                !enoughUniqueColorsInWindow(types_in_current_window, lowest_index, highest_index)) {
                int min_lcp = sliding_window.get_min();
                if (min_lcp > 0) {
                    if (min_lcp > current_longest_length) {
                        // A new longest string, hence we do not need previous anymore !
                        current_longest_length = min_lcp;
                        _longest_comon_substrings_indices.clear();
                    }
                    if (min_lcp == current_longest_length) {
                        // Another substring of same length of the previous longest substring is found
                        _longest_comon_substrings_indices.emplace_back(lowest_index, min_lcp);
                    }
                }
            }
        }
    }
};

#endif //LOGEST_COMMON_SUBSTRING_MULTIPLELONGESTCOMMONSUBSTRING_H
