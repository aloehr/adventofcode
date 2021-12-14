#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

#include "../aoc.hpp"

void transform(
    std::unordered_map<std::string, long long>& pairs_quantity,
    std::unordered_map<char, long long>& chars_quantity,
    std::unordered_map<std::string, char>& rules,
    unsigned int steps
) {
    auto pq_copy = pairs_quantity;
    std::string tmp = "XX";

    while (steps--) {
        for (auto& p : pq_copy) p.second = 0;

        for (const auto& p : pairs_quantity) {
            char r = rules[p.first];

            tmp[0] = p.first[0];
            tmp[1] = r;
            pq_copy[tmp] += p.second;

            tmp[0] = r;
            tmp[1] = p.first[1];
            pq_copy[tmp] += p.second;

            chars_quantity[r] += p.second;
        }

        std::swap(pq_copy, pairs_quantity);
    }
}

answer solve_day14(input& in) {

    answer a;

    std::unordered_map<std::string, long long> pairs_quantity;
    std::unordered_map<char, long long> chars_quantity;

    std::unordered_map<std::string, char> rules;

    for (size_t i = 2; i < in.size(); ++i) {
        auto tmp = split(in[i]);

        rules[tmp[0]] = tmp[2][0];

        pairs_quantity[tmp[0]] = 0;
        chars_quantity[tmp[2][0]] = 0;
    }

    for (auto c : in[0]) {
        chars_quantity[c]++;
    }
    for (size_t i = 0; i < in[0].size() - 1; ++i) {
        pairs_quantity[in[0].substr(i, 2)]++;
    }

    transform(pairs_quantity, chars_quantity, rules, 10);

    long long mmin = chars_quantity.begin()->second;
    long long mmax= chars_quantity.begin()->second;

    for (auto& p : chars_quantity) {
        mmin = std::min(mmin, p.second);
        mmax = std::max(mmax, p.second);
    }

    a.part1 = std::to_string(mmax - mmin);


    // part 2

    transform(pairs_quantity, chars_quantity, rules, 30);

    mmin = chars_quantity.begin()->second;
    mmax= chars_quantity.begin()->second;

    for (auto& p : chars_quantity) {
        mmin = std::min(mmin, p.second);
        mmax = std::max(mmax, p.second);
    }

    a.part2 = std::to_string(mmax - mmin);

    return a;
}
