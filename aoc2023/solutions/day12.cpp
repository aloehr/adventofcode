#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <numeric>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


bool fits_exactly(const std::string& condition_record, const std::vector<unsigned int>& damaged_groups) {
    size_t i = 0;

    for (size_t j = damaged_groups.size(); j > 0; --j)  {
        auto cur_count = damaged_groups[j-1];

        while (cur_count) {
            if (condition_record[i++] == '.') {
                return false;
            }

            cur_count--;
        }

        if (i < condition_record.size() && condition_record[i++] == '#') {
            return false;
        }
    }

    return true;
}

unsigned long long count_possible_arrangements(std::string condition_record, std::vector<unsigned int>& damaged_groups) {
    static std::unordered_map<std::string, unsigned long long> memo;

    size_t i = 0;
    while (condition_record[i] == '.') { i++; }

    condition_record = condition_record.substr(i);

    if (condition_record == "" && damaged_groups.empty()) return 1;
    else if (condition_record == "" && damaged_groups.size()) return 0;

    size_t min_size = std::accumulate(damaged_groups.cbegin(), damaged_groups.cend(), damaged_groups.empty() ? 0 : damaged_groups.size() - 1);
    if (condition_record.size() < min_size) {
        return 0;
    }

    if (condition_record.size() == min_size) {
        if (fits_exactly(condition_record, damaged_groups)) {
            return 1;
        } else {
            return 0;
        }

    }

    std::string hash = condition_record;
    for (auto n : damaged_groups) {
        hash += std::to_string(n) + ",";
    }

    if (memo.count(hash)) {
        return memo[hash];
    }

    unsigned long long ret = 0;

    if (condition_record.front() == '?') {
        condition_record.front() = '#';
        ret += count_possible_arrangements(condition_record, damaged_groups);
        ret += count_possible_arrangements(condition_record.substr(1), damaged_groups);
        condition_record.front() = '?';

        memo[hash] = ret;
        return ret;
    } else {
        unsigned count = 0;
        i = 0;
        while (i < condition_record.size() && condition_record[i] == '#') {
            count++;
            i++;
        }

        if ((count && damaged_groups.empty()) || (count && damaged_groups.back() < count)) {
            memo[hash] = 0;
            return 0;
        }
        if (condition_record.size() == i) {
            if (count && count == damaged_groups.back() && damaged_groups.size() == 1) {
                memo[hash] = 1;
                return 1;
            } else if (count == 0 && damaged_groups.empty()) {
                memo[hash] = 1;
                return 1;
            } else {
                memo[hash] = 0;
                return 0;
            }
        }
        if (condition_record[i] == '.') {
            if (damaged_groups.back() != count) {
                memo[hash] = 0;
                return 0;
            }

            auto tmp = damaged_groups.back();
            damaged_groups.pop_back();
            ret += count_possible_arrangements(condition_record.substr(i), damaged_groups);
            damaged_groups.push_back(tmp);
        } else {
            if (damaged_groups.back() == count) {
                condition_record[i] = '.';
                auto tmp = damaged_groups.back();
                damaged_groups.pop_back();
                ret += count_possible_arrangements(condition_record.substr(i), damaged_groups);
                damaged_groups.push_back(tmp);
            } else if (damaged_groups.back() > count) {
                condition_record[i] = '#';
                ret += count_possible_arrangements(condition_record, damaged_groups);
            } else {
                memo[hash] = 0;
                return 0;
            }
        }

    }

    memo[hash] = ret;
    return ret;
}

aoch::Result solve_day12(aoch::Input& in) {
    aoch::Result a;

    unsigned int sum1 = 0;
    unsigned long long sum2 = 0;

    for (const auto& l : in) {
        auto tokens = aoch::split(l);

        std::string& condition_record = tokens[0];

        auto groups_tokens = aoch::split(tokens[1], ',');
        std::vector<std::string> groups_tokens2(groups_tokens.rbegin(), groups_tokens.rend());
        std::vector<unsigned int> damaged_groups;
        damaged_groups.reserve(groups_tokens2.size());

        for (const auto& num_str : groups_tokens2) {
            damaged_groups.push_back(std::stoul(num_str));
        }

        sum1 += count_possible_arrangements(condition_record, damaged_groups);

        std::string condition_record_copy = condition_record;
        auto damaged_groups_copy = damaged_groups;

        int i = 4;
        while (i--) {
            damaged_groups.insert(damaged_groups.cend(), damaged_groups_copy.cbegin(), damaged_groups_copy.cend());
            condition_record += "?" + condition_record_copy;
        }

        sum2 += count_possible_arrangements(condition_record, damaged_groups);
    }

    a.part1 = std::to_string(sum1);
    a.part2 = std::to_string(sum2);

    return a;
}
