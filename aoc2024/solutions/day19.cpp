#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>


using jump_table_t = std::array<std::array<size_t, 2>, 128>;

bool starts_with(const std::string_view s, const std::string_view cmp) {
    if (cmp.size() > (s.size())) return false;

    for (size_t i = 0; i < cmp.size(); ++i) {
        if (s[i] != cmp[i]) return false;
    }

    return true;
}

long long design_possible(
    const std::string_view design,
    const std::vector<std::string_view>& patterns,
    std::unordered_map<std::string_view, long long>& memo,
    const jump_table_t& jump_table
) {
    if (design.empty()) return 1;

    if (memo.count(design)) {
        return memo[design];
    }

    long long total = 0;

    for (auto [i, end] = jump_table[design[0]]; i < end; ++i) {
        if (starts_with(design, patterns[i])) {
            total += design_possible(design.substr(patterns[i].size()), patterns, memo, jump_table);
        }
    }

    memo[design] = total;
    return total;
}

std::vector<std::string_view> parse_patterns(std::string_view s) {
    std::vector<std::string_view> ret;

    size_t start = 0;
    size_t end;

    while ((end = s.find_first_of(',', start)) != std::string::npos) {
        ret.push_back(s.substr(start, end - start));

        // skipping comma and whie space
        start = end + 2;
    }

    // add the last pattern
    ret.push_back(s.substr(start));

    return ret;
}

aoch::Result solve_day19(aoch::Input& in) {
    aoch::Result a;

    std::vector<std::string_view> patterns = parse_patterns(in[0]);

    std::sort(patterns.begin(), patterns.end());

    jump_table_t jump_table{};

    for (size_t i = 0; i < patterns.size(); ++i) {
        const char c = patterns[i][0];
        if (jump_table[c][1] == 0) {
            jump_table[c] = {i, patterns.size()};

            if (i > 0) {
                jump_table[patterns[i-1][0]][1] = i;
            }
        }
    }

    std::vector<std::string> designs;
    designs.reserve(in.size() - 2);

    for (size_t i = 2; i < in.size(); ++i) {
        designs.push_back(std::move(in[i]));
    }

    int count_possible = 0;
    long long total_ways = 0;

    std::unordered_map<std::string_view, long long> memo;

    for (const auto& d : designs) {
        long long possible_ways = design_possible(d, patterns, memo, jump_table);

        count_possible += possible_ways != 0;
        total_ways += possible_ways;
    }

    a.part1 = std::to_string(count_possible);
    a.part2 = std::to_string(total_ways);

    return a;
}
