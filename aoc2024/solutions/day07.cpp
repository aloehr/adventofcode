#include <string>
#include <utility>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>
#include <aoch/string.hpp>


bool is_possible(const std::pair<long, std::vector<int>>& eq, const size_t i = 0, const long cur_val = 0) {
    if (i == eq.second.size()) return cur_val == eq.first;

    if (cur_val > eq.first) return false;

    if (i == 0) {
        return is_possible(eq, 1, eq.second[0]);
    }

    return is_possible(eq, i + 1, cur_val + eq.second[i]) ||
           is_possible(eq, i + 1, cur_val * eq.second[i]);
}

int digits_shift_multi(int val) {
    int ret = 1;

    while (val) {
        ret *= 10;
        val /= 10;
    }

    return ret;
}

bool is_possible2(const std::pair<long, std::vector<int>>& eq, size_t i = 0, long cur_val = 0) {
    if (cur_val > eq.first) return false;
    if (i == eq.second.size()) return cur_val == eq.first;

    if (i == 0) {
        return is_possible2(eq, 1, eq.second[0]);
    }

    return is_possible2(eq, i + 1, cur_val + eq.second[i]) ||
           is_possible2(eq, i + 1, cur_val * eq.second[i]) ||
           is_possible2(eq, i + 1, cur_val * digits_shift_multi(eq.second[i]) + eq.second[i]);
}

aoch::Result solve_day07(aoch::Input& in) {
    aoch::Result a;

    std::vector<std::pair<long, std::vector<int>>> equations;
    equations.reserve(in.size());

    for (const auto& line : in) {
        const auto tokens1 = aoch::split(line, ':');
        const auto tokens2 = aoch::split(tokens1[1]);

        std::pair<long, std::vector<int>> cur_eq;
        cur_eq.first = std::stol(tokens1[0]);
        cur_eq.second.reserve(tokens1[2].size());

        for (const auto& t : tokens2) {
            cur_eq.second.push_back(std::stoi(t));
        }

        equations.push_back(std::move(cur_eq));
    }

    long sum  = 0;
    long sum2 = 0;
    for (const auto& eq : equations) {
        if (is_possible(eq)) {
            sum  += eq.first;
            sum2 += eq.first;
        } else if (is_possible2(eq)) {
            sum2 += eq.first;
        }
    }

    a.part1 = std::to_string(sum);
    a.part2 = std::to_string(sum2);

    return a;
}
