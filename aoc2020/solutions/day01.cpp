#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../aoc.hpp"

answer solve_day01(input& in) {

    answer a;
    in.pop_back();

    std::vector<int> data;
    data.reserve(in.size());

    for (auto& l : in) {
        data.emplace_back(std::stoi(l)); 
    }

    std::sort(data.begin(), data.end());

    for (unsigned int i = 0; i < data.size() - 1; ++i) {
        if (std::binary_search(data.begin() + i + 1, data.end(), 2020 - data[i])) {
            a.part1 = std::to_string(data[i] * (2020 - data[i]));
            break;
        }
    }

    bool found = false;

    for (unsigned int i = 0; i < (data.size() - 2) && !found; ++i) {
        for (unsigned int j = i+1; j < data.size() - 1 && !found; ++j) {
            int c = data[i] + data[j];

            if (std::binary_search(data.begin() + j + 1, data.end(), 2020 - c)) {
                found = true;
                a.part2 =  std::to_string(data[i] * data[j] * (2020 - c));
            }
        }
    }

    return a;
}
