#include <iostream>
#include <string>
#include <vector>

#include "../aoc.hpp"


answer solve_day01(input& in) {

    answer a;

    std::vector<int> data;
    data.reserve(in.size());

    for (const auto& l : in)
        data.push_back(std::stoi(l));

    int incr_count = 0;

    for (size_t i = 1; i < data.size(); ++i)
        incr_count += data[i] > data[i-1];

    a.part1 = std::to_string(incr_count);


    // part 2

    incr_count = 0;

    for (size_t i = 0; i < data.size() - 3; ++i)
        incr_count += data[i+3] > data[i];

    a.part2 = std::to_string(incr_count);

    return a;
}
