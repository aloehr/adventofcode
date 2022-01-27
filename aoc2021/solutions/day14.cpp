#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <tuple>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


std::tuple<long, long> min_max(const std::vector<long>& v) {
    long min = v[0];
    long max = v[0];

    for (size_t i = 1; i < v.size(); ++i) {
        // special min, min == smallest min > 0 and only if everything is 0 it will be 0
        if (!min) {
            min = v[i];
        }
        else if (v[i]) {
            min = std::min(min, v[i]);
        }

        max = std::max(max, v[i]);
    }

    return std::make_tuple(min, max);
}

void transform(
    std::unordered_map<std::string, long>& pairs_quantity,
    std::vector<long>& chars_quantity,
    std::unordered_map<std::string, std::array<std::string, 2>>& rules,
    unsigned int steps
) {
    auto pq_copy = pairs_quantity;

    while (steps--) {
        for (auto& [pair, count] : pq_copy) {
            count = 0;
        }

        for (const auto& [pair, count] : pairs_quantity) {
            const auto& np = rules[pair];

            pq_copy[np[0]] += count;
            pq_copy[np[1]] += count;

            chars_quantity[np[0][1] - 'A'] += count;
        }

        std::swap(pq_copy, pairs_quantity);
    }
}

aoch::Result solve_day14(aoch::Input& in) {

    aoch::Result a;

    std::unordered_map<std::string, long> pairs_quantity;
    std::vector<long> chars_quantity(26);

    std::unordered_map<std::string, std::array<std::string, 2>> rules;

    for (size_t i = 2; i < in.size(); ++i) {
        auto tmp = aoch::split(in[i]);

        std::string np1 = tmp[0].substr(0, 1) + tmp[2];
        std::string np2 = tmp[2] + tmp[0].substr(1, 1);

        rules[tmp[0]] = {np1, np2};

        pairs_quantity[tmp[0]] = 0;
    }

    for (auto c : in[0]) {
        chars_quantity[c - 'A']++;
    }
    for (size_t i = 0; i < in[0].size() - 1; ++i) {
        pairs_quantity[in[0].substr(i, 2)]++;
    }

    transform(pairs_quantity, chars_quantity, rules, 10);

    auto [min, max] = min_max(chars_quantity);

    a.part1 = std::to_string(max - min);


    // part 2

    transform(pairs_quantity, chars_quantity, rules, 30);

    std::tie(min, max) = min_max(chars_quantity);

    a.part2 = std::to_string(max - min);

    return a;
}
