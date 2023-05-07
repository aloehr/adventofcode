#include <algorithm>

#include <iostream>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"


bool greater(int a, int b) {
    return a > b;
}

aoch::Result solve_day01(aoch::Input& in) {

    aoch::Result a;

    int cur_elve_calories = 0;
    std::vector<int> elves_cals;
    in.push_back("");

    for (const auto& num_str : in) {
        if (num_str.empty()) {
            elves_cals.push_back(cur_elve_calories);
            cur_elve_calories = 0;
        } else {
            cur_elve_calories += std::stoi(num_str);
        }
    }

    std::sort(elves_cals.begin(), elves_cals.end(), greater);

    a.part1 = std::to_string(elves_cals[0]);
    a.part2 = std::to_string(elves_cals[0] + elves_cals[1] + elves_cals[2]);

    return a;
}
