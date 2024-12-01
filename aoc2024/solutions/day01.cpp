#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <unordered_map>

#include <aoch/AOCSolutionTypes.hpp>
#include <aoch/string.hpp>


int string_to_int(const std::string& s, size_t start) {
    int ret = 0;

    for (size_t i = 0; i < 5; ++i) {
        ret = ret * 10 + (s[start + i] - '0');
    }

    return ret;
}

aoch::Result solve_day01(aoch::Input& in) {
    aoch::Result a;

    std::vector<int> list_left(in.size()), list_right(in.size());

    for (size_t i = 0; i < in.size(); ++i) {
        const std::string& line = in[i];

        list_left[i] = string_to_int(line, 0);
        list_right[i] = string_to_int(line, 8);
    }

    std::sort(list_left.begin(), list_left.end());
    std::sort(list_right.begin(), list_right.end());

    int diff_sum = 0;
    for (size_t i = 0; i < list_left.size(); ++i) {
        int diff = list_left[i] - list_right[i];
        if (diff < 0) diff *= -1;

        diff_sum += diff;
    }

    a.part1 = std::to_string(diff_sum);


    // part 2
    std::unordered_map<int, int> list_right_counter;
    for (auto n : list_right) list_right_counter[n]++;

    int similarity_score = 0;
    for (auto n : list_left) {
        if (list_right_counter.count(n)) {
            similarity_score += n * list_right_counter[n];
        }
    }

    a.part2 = std::to_string(similarity_score);

    return a;
}
