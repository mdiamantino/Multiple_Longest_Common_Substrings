//
// Created by mdc on 10.04.21.
//

#ifndef LOGEST_COMMON_SUBSTRING_SUFFIXARRAY_H
#define LOGEST_COMMON_SUBSTRING_SUFFIXARRAY_H


#include <vector>

class SuffixArray {
public:
    std::vector<int> &_suffix_array;
    std::vector<int> _text;
    const int _alphabet_size;

    SuffixArray(std::vector<int> &suffixArray, std::vector<int> text, int alphabet_size) :
            _suffix_array(suffixArray),
            _text(std::move(text)),
            _alphabet_size(alphabet_size) { sa_is(); }

private:
    void sa_is();

    static bool is_lms(const std::vector<bool> &type, int idx);

    void calc_bucket(std::vector<int> &bucket);

    void induced_sort(const std::vector<bool> &type, const std::vector<int> &lms);

};


#endif //LOGEST_COMMON_SUBSTRING_SUFFIXARRAY_H
