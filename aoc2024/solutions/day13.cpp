#include <array>
#include <iostream>
#include <string>
#include <optional>

#include <aoch/AOCSolutionTypes.hpp>


int parse_int(const std::string& s, const size_t start, const size_t end) {
    int ret = 0;

    for (size_t i = start; i < end; ++i) {
        ret = ret * 10 + (s[i] & 0xF);
    }

    return ret;
}

std::optional<std::pair<long long, long long>> calc(std::array<long long, 2> button_a, std::array<long long, 2> button_b, std::array<long long, 2> goal) {
    // a * ax + b * bx = gx
    // a * ay + b * by = gy
    long long b_press_count = button_b[1] * button_a[0] - button_b[0] * button_a[1];
    long long gy_mod = goal[1] * button_a[0] - goal[0] * button_a[1];

    // if b_presses == 0 && gy_mod != 0 -> no solution,
    // if b_presses and gy_mod are zero -> infinite solutions,
    // if gy_mod % b_presses != 0 -> solution is not an integer
    if (b_press_count == 0 || gy_mod % b_press_count != 0) return std::nullopt;

    b_press_count = gy_mod / b_press_count;

    long long a_press_count = goal[0] - button_b[0] * b_press_count;

    // again if the mod is not 0 the solution will not be an integer
    if (a_press_count % button_a[0] != 0) return std::nullopt;

    a_press_count = a_press_count / button_a[0];

    return std::make_pair(a_press_count, b_press_count);
}

aoch::Result solve_day13(aoch::Input& in) {
    aoch::Result a;

    long long total = 0;
    long long total2 = 0;

    for (size_t i = 0; i < in.size(); i += 4) {

        const auto parse_line = [&in] (const size_t i, const char before_num) {
            std::array<long long, 2> ret;

            size_t num_start = in[i].find_first_of(before_num) + 1;
            size_t comma_pos = in[i].find_first_of(',');

            ret[0] = parse_int(in[i], num_start, comma_pos);

            num_start = in[i].find_first_of(before_num, comma_pos + 1) + 1;
            ret[1] = parse_int(in[i], num_start, in[i].size());

            return ret;
        };

        const auto button_a_dir = parse_line(i, '+');
        const auto button_b_dir = parse_line(i + 1, '+');
        auto goal_pos = parse_line(i + 2, '=');

        auto sol1 = calc(button_a_dir, button_b_dir, goal_pos);
        if (sol1) total += sol1->first * 3 + sol1->second;


        // part 2
        long long add = 1e13;
        goal_pos[0] += add;
        goal_pos[1] += add;

        auto sol2 = calc(button_a_dir, button_b_dir, goal_pos);
        if (sol2) total2 += sol2->first * 3 + sol2->second;
    }

    a.part1 = std::to_string(total);
    a.part2 = std::to_string(total2);

    return a;
}
