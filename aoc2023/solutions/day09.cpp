#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


bool is_zero(int n) {
    return n == 0;
}

int extrapolate(const std::vector<int>& seq) {
    if (std::all_of(seq.cbegin(), seq.cend(), is_zero)) return 0;

    std::vector<int> sub_seq;
    sub_seq.reserve(seq.size() - 1);

    for (size_t i = 0; i < seq.size() - 1; ++i) {
        sub_seq.push_back(seq[i+1] - seq[i]);
    }

    return seq.back() + extrapolate(sub_seq);
}

int extrapolate_front(const std::vector<int>& seq) {
    return extrapolate({seq.rbegin(), seq.rend()});
}

aoch::Result solve_day09(aoch::Input& in) {
    aoch::Result a;

    std::vector<std::vector<int>> sequences;
    sequences.reserve(in.size());

    for (const auto& l : in) {
        auto tokens = aoch::split(l);

        std::vector<int> cur_seq;
        cur_seq.reserve(tokens.size());

        for (const auto& t : tokens) {
            cur_seq.push_back(std::stoi(t));
        }

        sequences.push_back(std::move(cur_seq));
    }

    int sum = 0;
    for (const auto& s : sequences) {
        sum += extrapolate(s);
    }

    a.part1 = std::to_string(sum);


    // part 2
    sum = 0;
    for (const auto& s : sequences) {
        sum += extrapolate_front(s);
    }

    a.part2 = std::to_string(sum);

    return a;
}
