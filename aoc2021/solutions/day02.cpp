#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "../aoc.hpp"

answer solve_day02(input& in) {

    answer a;

    std::vector<std::array<int, 2>> data;
    data.reserve(in.size());

    for (const auto& l : in) {
        size_t del_pos = l.find(' ');
        int tmp = 0;

        if (l[0] == 'u') {
            tmp = 1;
        } else if (l[0] == 'd') {
            tmp = 2;
        }

        data.push_back(std::array<int, 2>({tmp, std::stoi(l.substr(del_pos+1))}));
    }

    int depth = 0;
    int horizontal_pos = 0;

    for (const auto& c : data) {
        if (c[0] == 0) {
            horizontal_pos += c[1];
        } else if (c[0] == 1) {
            depth -= c[1];

        } else {
            // down
            depth += c[1];
        }
    }

    a.part1 = std::to_string(depth * horizontal_pos);

    // part 2

    depth = 0;
    horizontal_pos = 0;
    int aim = 0;

    for (const auto& c : data) {
        if (c[0] == 0) {
            horizontal_pos += c[1];
            depth += aim * c[1];
        } else if (c[0] == 1) {
            aim -= c[1];
        } else {
            // down
            aim += c[1];
        }
    }

    a.part2 = std::to_string(depth * horizontal_pos);

    return a;
}
