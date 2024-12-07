#include <string>
#include <utility>
#include <iostream>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>


bool is_possible(const std::vector<int>& vals, size_t i, const long long cur_result) {
    if (i >= vals.size()) return cur_result == 0;

    if (cur_result % vals[i] == 0 && is_possible(vals, i - 1, cur_result / vals[i])) {
        return true;
    }

    const long long tmp = cur_result - vals[i];
    if (tmp >= 0 && is_possible(vals, i - 1, tmp)) {
        return true;
    }

    return false;
}

bool is_possible2(const std::vector<int>& vals, size_t i, const long long cur_result) {
    if (i >= vals.size()) return cur_result == 0;


    if (cur_result >= vals[i]) {
        int val = vals[i];
        long long next_result = cur_result;

        while (val && (val % 10) == (next_result % 10)) {
            val /= 10;
            next_result /= 10;
        }

        if (val == 0 && is_possible2(vals, i - 1, next_result)) {
            return true;
        }
    }

    if (cur_result % vals[i] == 0) {
        if (is_possible2(vals, i - 1, cur_result / vals[i])) return true;
    }

    const long long tmp = cur_result - vals[i];
    if (tmp >= 0 && is_possible2(vals, i - 1, tmp)) {
        return true;
    }

    return false;
}

long long parse_long(const std::string& s, const size_t start, const size_t end) {
    long long ret = 0;

    for (size_t i = start; i < end; ++i) {
        ret = ret * 10 + (s[i] & 0xF);
    }

    return ret;
}

aoch::Result solve_day07(aoch::Input& in) {
    aoch::Result a;

    std::vector<std::pair<long long, std::vector<int>>> equations;
    equations.reserve(in.size());

    for (const auto& line : in) {
        std::vector<int> vals;
        vals.reserve(12);

        size_t cur_num_start = 0;
        size_t cur_num_end = line.find_first_of(':');

        const long long result = parse_long(line, cur_num_start, cur_num_end);

        cur_num_start = cur_num_end + 2;

        while (cur_num_start < line.size()) {
            cur_num_end = line.find_first_of(' ', cur_num_start);
            if (cur_num_end == std::string::npos) cur_num_end = line.size();

            vals.push_back(parse_long(line, cur_num_start, cur_num_end));

            cur_num_start = cur_num_end + 1;
        }

        equations.emplace_back(result, std::move(vals));
    }

    long long sum  = 0;
    long long sum2 = 0;

    for (const auto& eq : equations) {
        if (is_possible(eq.second, eq.second.size() - 1, eq.first)) {
            sum  += eq.first;
            sum2 += eq.first;
        } else if (is_possible2(eq.second, eq.second.size() - 1, eq.first)) {
            sum2 += eq.first;
        }
    }

    a.part1 = std::to_string(sum);
    a.part2 = std::to_string(sum2);

    return a;
}
