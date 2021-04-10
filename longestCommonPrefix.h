#ifndef LOGEST_COMMON_SUBSTRING_LONGESTCOMMONPREFIX_H
#define LOGEST_COMMON_SUBSTRING_LONGESTCOMMONPREFIX_H

// http://web.cs.iastate.edu/~cs548/references/linear_lcp.pdf
template<typename T>
std::vector<int> longest_common_prefix_from_suffix_array(const std::vector <T> &word, const std::vector<T> &suffix_array) {
    int number_of_suffixes = suffix_array.size();
    // Building inverse of the suffix array
    std::vector<int> rank(number_of_suffixes, 0);
    for (int i = 0; i < number_of_suffixes; i++) rank[suffix_array[i]] = i;

    std::vector<int> longest_common_prefix(number_of_suffixes, 0);
    for (int i = 0, len = 0; i < number_of_suffixes; i++) {
        if (rank[i] > 0) {
            int k = suffix_array[rank[i] - 1];
            while ((i + len < number_of_suffixes) && (k + len < number_of_suffixes) &&
                   word[i + len] == word[k + len])
                len++;
            longest_common_prefix[rank[i]] = len;
            if (len > 0) len--;
        }
    }
    return longest_common_prefix;
}

#endif //LOGEST_COMMON_SUBSTRING_LONGESTCOMMONPREFIX_H
