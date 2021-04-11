#ifndef LOGEST_COMMON_SUBSTRING_LONGESTCOMMONPREFIX_H
#define LOGEST_COMMON_SUBSTRING_LONGESTCOMMONPREFIX_H

/**
 * Function to comput the array of longest common prefixes from the suffix array
 * Complexity : O(n) where n is the length of the text
 * Based on http://web.cs.iastate.edu/~cs548/references/linear_lcp.pdf
 * @tparam T Type of characters in the initial text (int, char, ...)
 * @param text : text
 * @param suffix_array : Suffix array of the text
 * @return
 */
template<typename T>
std::vector<int> ComputeLongestCommonPrefix(const std::vector<T> &text, const std::vector<T> &suffix_array) {
    int number_of_suffixes = suffix_array.size();
    // Building inverse of the suffix array
    std::vector<int> rank(number_of_suffixes, 0);
    for (std::size_t i = 0; i < number_of_suffixes; ++i) rank[suffix_array[i]] = i;

    std::vector<int> longest_common_prefix(number_of_suffixes, 0);
    for (std::size_t i = 0, len = 0; i < number_of_suffixes; ++i) {
        if (rank[i] > 0) {
            int k = suffix_array[rank[i] - 1];
            while ((i + len < number_of_suffixes) && (k + len < number_of_suffixes) &&
                   text[i + len] == text[k + len])
                len++;
            longest_common_prefix[rank[i]] = len;
            if (len > 0) len--;
        }
    }
    return longest_common_prefix;
}

#endif //LOGEST_COMMON_SUBSTRING_LONGESTCOMMONPREFIX_H
