//
// Created by mdc on 11.04.21.
//

#ifndef LOGEST_COMMON_SUBSTRING_SAIS2_H
#define LOGEST_COMMON_SUBSTRING_SAIS2_H

#ifndef SAIS_H
#define SAIS_H

#include <type_traits>
#include <vector>

#define MAKE_UNSIGNED(X)                                                       \
  (static_cast<typename std::make_unsigned<                                    \
       typename std::decay<decltype((X))>::type>::type>((X)))

enum class suffix_type {
    S = 0,
    L = 1,
};

template<typename T>
std::vector<suffix_type> built_type_map(const T &data) {
    std::vector<suffix_type> result(data.size() + 1, suffix_type::S);

    result[data.size() - 1] = suffix_type::L;
    if (data.size() > 0)
        for (std::size_t i = data.size() - 1; i-- > 0;)
            if ((MAKE_UNSIGNED(data[i]) > MAKE_UNSIGNED(data[i + 1])) ||
                (data[i] == data[i + 1] && result[i + 1] == suffix_type::L))
                result[i] = suffix_type::L;

    return result;
}

bool is_lms_char(int index, const std::vector<suffix_type> &types) {
    return index > 0 && (types[index] < types[index - 1]);
}

template<typename T>
bool lms_substrings_equal(const T &data, const std::vector<suffix_type> &types,
                          std::size_t a, std::size_t b) {
    for (;; a++, b++) {
        if (is_lms_char(a, types) != is_lms_char(b, types) || data[a] != data[b])
            return false;
        else if (is_lms_char(a + 1, types) && is_lms_char(b + 1, types))
            return true;
    }
}

struct bucket {
    std::size_t size;
    std::size_t head;
    std::size_t tail;
};

void reset_head_tails(std::vector<bucket> &buckets) {
    std::size_t offset = 1;
    for (auto &&b : buckets) {
        b.head = offset;
        offset += b.size;
        b.tail = offset - 1;
    }
}

template<typename T>
std::vector<bucket> create_buckets(const T &data, std::size_t alphabet = 256) {
    std::vector<bucket> result(alphabet, bucket{});

    for (auto &&c : data)
        result[MAKE_UNSIGNED(c)].size++;
    reset_head_tails(result);

    return result;
}

template<typename T>
std::vector<int> induce_lms(T &data, const std::vector<suffix_type> &types,
                            std::vector<bucket> buckets) {
    reset_head_tails(buckets);
    std::vector<int> guess(data.size() + 1, -1);

    for (std::size_t i = 0; i < data.size(); i++) {
        if (is_lms_char(i, types)) {
            const std::size_t index = MAKE_UNSIGNED(data[i]);
            guess[buckets[index].tail--] = i;
        }
    }
    guess[0] = data.size();

    return guess;
}

template<typename T>
void induce_LS_sort(const T &data, const std::vector<suffix_type> &types,
                    std::vector<int> &guess, std::vector<bucket> &buckets) {
    reset_head_tails(buckets);
    for (auto &&index : guess) {
        if (index != -1) {
            auto j = index - 1;
            if (j >= 0 && types[j] == suffix_type::L) {
                const std::size_t bucket_index = MAKE_UNSIGNED(data[MAKE_UNSIGNED(j)]);
                guess[buckets[bucket_index].head++] = j;
            }
        }
    }
    induce_S_sort(data, types, guess, buckets);
}

template<typename T>
void induce_S_sort(const T &data, const std::vector<suffix_type> &types,
                   std::vector<int> &guess, std::vector<bucket> &buckets) {
    reset_head_tails(buckets);
    for (std::size_t i = guess.size(); i-- > 0;) {
        int index = guess[i];
        if (index != -1) {
            auto j = index - 1;
            if (j >= 0 && types[j] == suffix_type::S) {
                const std::size_t bucket_index = MAKE_UNSIGNED(data[MAKE_UNSIGNED(j)]);
                guess[buckets[bucket_index].tail--] = j;
            }
        }
    }
}

template<typename T>
auto reduce(const T &data, const std::vector<suffix_type> &types,
            std::vector<int> &guess) {
    std::vector<int> lms_names(data.size() + 1, -1), summary_sufix_offsets,
            summary_string;
    int current_name = 0, last_lms_suffix;

    lms_names[MAKE_UNSIGNED(guess[0])] = current_name;
    last_lms_suffix = guess[0];
    for (std::size_t i = 1; i < guess.size(); i++) {
        std::size_t suffix_offset = MAKE_UNSIGNED(guess[i]);
        if (is_lms_char(suffix_offset, types)) {
            if (!lms_substrings_equal(data, types, last_lms_suffix, suffix_offset))
                current_name++;

            last_lms_suffix = suffix_offset;
            lms_names[suffix_offset] = current_name;
        }
    }

    for (std::size_t i = 0; i < lms_names.size(); i++)
        if (lms_names[i] != -1) {
            summary_sufix_offsets.push_back(i);
            summary_string.push_back(lms_names[i]);
        }

    return std::make_tuple(summary_string, current_name + 1,
                           summary_sufix_offsets);
}

template<typename T>
std::vector<int> induce_lms(const T &data,
                            const std::vector<int> &summary_suffix_array,
                            const std::vector<int> &summary_suffix_offsets,
                            std::vector<bucket> &buckets) {
    reset_head_tails(buckets);
    std::vector<int> suffix_offsets(data.size() + 1, -1);

    for (std::size_t i = summary_suffix_array.size(); i-- > 2;) {
        const auto string_index =
                summary_suffix_offsets[MAKE_UNSIGNED(summary_suffix_array[i])];
        const auto bucket_index = MAKE_UNSIGNED(data[MAKE_UNSIGNED(string_index)]);
        suffix_offsets[buckets[bucket_index].tail--] = string_index;
    }
    suffix_offsets[0] = data.size();

    return suffix_offsets;
}

template<typename T>
std::vector<int> sais_suffix_array(const T &data,
                                   std::size_t alphabet_size = 256) {
    auto buckets = create_buckets(data, alphabet_size);
    const auto typemap = built_type_map(data);
    auto guess = induce_lms(data, typemap, buckets);
    induce_LS_sort(data, typemap, guess, buckets);
    auto s = reduce(data, typemap, guess);

    std::vector<int> s_array;
    if (std::get<0>(s).size() == std::get<1>(s)) {
        std::vector<int> suffix_array(std::get<0>(s).size() + 1, -1);
        suffix_array[0] = std::get<0>(s).size();
        for (auto &&c : std::get<0>(s)) {
            suffix_array[std::get<0>(s)[MAKE_UNSIGNED(c)] + 1] = c;
        }
        s_array = std::move(suffix_array);
    } else
        s_array = sais_suffix_array(std::get<0>(s), std::get<1>(s));

    auto result = induce_lms(data, s_array, std::get<2>(s), buckets);
    induce_LS_sort(data, typemap, result, buckets);

    return result;
}

#endif // SAIS_H


#endif //LOGEST_COMMON_SUBSTRING_SAIS2_H
