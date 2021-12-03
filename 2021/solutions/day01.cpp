#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../aoc.hpp"

answer solve_day01(input& in) {

    answer a;

    // remove empty line
    in.pop_back();

    std::vector<int> data;
    data.reserve(in.size());

    for (auto& l : in)
        data.push_back(std::stoi(l));

    int incr_count = 0;

    for (unsigned int i = 1; i < data.size(); ++i) {
        if (data[i] > data[i-1])
            incr_count++;
    }

    a.part1 = std::to_string(incr_count);


    // part 2

    incr_count = 0;

    for (unsigned int i = 0; i < data.size() - 3; ++i) {
        int prev_window = data[i] + data[i+1] + data[i+2];
        int cur_window = data[i+1] + data[i+2] + data[i+3];

        if (cur_window > prev_window) {
            incr_count++;
        }
    }

    a.part2 = std::to_string(incr_count);

    return a;
}
