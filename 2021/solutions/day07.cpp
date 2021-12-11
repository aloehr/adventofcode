#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#include "../aoc.hpp"


answer solve_day07(input& in) {

    answer a;

    std::vector<int> crab_pos;

    for (auto& s : split(in[0], ',')) {
        crab_pos.push_back(std::stoi(s));
    }

    std::sort(crab_pos.begin(), crab_pos.end());

    // median minimizes for p1
    int median = crab_pos[crab_pos.size() >> 1];

    // for p2 the minimum is at most 0.5 < avg and 0.5 > avg,
    // so we just check the 2 integers between the avg
    float avg = (float) (sum(crab_pos)) / (float)(crab_pos.size());
    int avg_floored = std::ceil(avg);
    int avg_ceiled = std::floor(avg);

    int best_fuel_cost = 0;
    int best1 = 0;
    int best2 = 0;

    for (auto cp : crab_pos) {
        best_fuel_cost += std::abs(median - cp);

        int d1 = std::abs(avg_floored - cp);
        int d2 = std::abs(avg_ceiled - cp);

        best1 += d1 * (d1 + 1) / 2;
        best2 += d2 * (d2 + 1) / 2;
    }

    a.part1 = std::to_string(best_fuel_cost);
    a.part2 = std::to_string(std::min(best1, best2));

    return a;
}
