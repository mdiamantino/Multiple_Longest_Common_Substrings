#pragma once

#include <string>
#include <utility>
#include <vector>

/**
 * https://github.com/tsushiy/competitive-programming-library/blob/ab914eee607088b0b6f5b85e177f4693b4be5d99/string/sa_is.hpp
 */
class SuffixArray {
public:
    static const int _alphabet_size = 256;
    std::vector<int> &_suffix_array;
    std::vector<int> _text;

    SuffixArray(std::vector<int> &suffixArray, std::vector<int> text) : _suffix_array(suffixArray),
                                                                         _text(std::move(text)) { sa_is(); }


private:
    void sa_is() {
        int n = _text.size();

        // Scan S once to classify all the characters as L- or S-type into type;
        std::vector<bool> type(n);  // S: true, L: false;
        type.back() = true;
        for (int i = n - 2; i >= 0; --i) {
            if (_text[i] == _text[i + 1]) {
                type[i] = type[i + 1];
            } else {
                type[i] = _text[i] < _text[i + 1];
            }
        }

        // Scan type once to find all the LMS-substrings in S into lms;
        std::vector<int> lms;
        for (int i = 0; i < n; ++i) {
            if (is_lms(type, i)) lms.emplace_back(i);
        }
        int nlms = lms.size();

        // Induced sort all the LMS-substrings using lms and bucket;
        induced_sort(type, lms);
        for (int i = 0, cnt = 0; cnt != nlms && i < n; ++i) {
            if (is_lms(type, _suffix_array[i])) _suffix_array[cnt++] = _suffix_array[i];
        }
        _suffix_array.resize(nlms);

        // Name each LMS-substring in S by its bucket index to get a new shortened string S1;
        std::vector<int> s1(n, -1);
        s1[_suffix_array[0]] = 0;
        int name = 0;
        for (int i = 0; i < nlms - 1; ++i) {
            int p = _suffix_array[i], q = _suffix_array[i + 1];
            bool same = i != 0;
            for (int j = 1; same; ++j) {
                if (_text[p + j] != _text[q + j]) same = false;
                if (!is_lms(type, p + j) && !is_lms(type, q + j)) continue;
                if (!is_lms(type, p + j) && !is_lms(type, q + j)) same = false;
                break;
            }
            if (!same) ++name;
            s1[q] = name;
        }
        for (int i = 0, cnt = 0; cnt != nlms && i < n; ++i) {
            if (s1[i] != -1) s1[cnt++] = s1[i];
        }
        s1.resize(nlms);

        // Each character in S1 is unique
        // then Directly compute SA1 from S1
        // else SA-IS(S1, SA1); where recursive call happens
        std::vector<int> sa1(nlms);
        int k1 = name + 1;
        if (k1 == nlms) {
            for (int i = 0; i < k1; ++i) sa1[s1[i]] = i;
        } else {
            sa_is();
        }
        for (int &idx : sa1) idx = lms[idx];

        // Induce SA from SA1
        induced_sort(type, sa1);
    }

    static bool is_lms(const std::vector<bool> &type, int idx) {
        return idx > 0 && type[idx] && !type[idx - 1];
    }

    void calc_bucket(std::vector<int> &bucket) {
        bucket.assign(_alphabet_size, 0);
        for (int ch : _text) ++bucket[ch];
        for (int i = 0; i < _alphabet_size - 1; ++i) bucket[i + 1] += bucket[i];
    }

    void induced_sort(const std::vector<bool> &type, const std::vector<int> &lms) {
        int n = _text.size();
        _suffix_array.resize(n);
        _suffix_array.assign(n, -1);

        std::vector<int> bucket(_alphabet_size);

        calc_bucket(bucket);
        for (auto it = lms.rbegin(); it != lms.rend(); ++it) {
            _suffix_array[--bucket[_text[*it]]] = *it;
        }

        calc_bucket(bucket);
        for (int i = 0; i < n; ++i) {
            if (!(_suffix_array[i] == -1 || _suffix_array[i] == 0 || type[_suffix_array[i] - 1])) {
                _suffix_array[bucket[_text[_suffix_array[i] - 1] - 1]++] = _suffix_array[i] - 1;
            }
        }

        calc_bucket(bucket);
        for (int i = n - 1; i >= 0; --i) {
            if (!(_suffix_array[i] == -1 || _suffix_array[i] == 0 || !type[_suffix_array[i] - 1])) {
                _suffix_array[--bucket[_text[_suffix_array[i] - 1]]] = _suffix_array[i] - 1;
            }
        }
    }
};