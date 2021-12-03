
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "../aoc.hpp"

// start 22:33
answer solve_day02(input& in) {

    answer a;

    // remove empty line
    in.pop_back();



    std::vector<std::pair<std::string, int>> data;
    data.reserve(in.size());

    for (auto& l : in) {
        size_t sep_pos = l.find(' ');

        std::string cmd = l.substr(0, sep_pos);
        int val = std::stoi(l.substr(sep_pos+1));
        data.push_back(std::make_pair(cmd, val));
    }

    int depth = 0;
    int horizontal_pos = 0;

    for (auto& c : data) {
        if (c.first == "forward") {
            horizontal_pos += c.second;
        } else if (c.first == "up") {
            depth -= c.second;

        } else {
            // down
            depth += c.second;
        }
    }

    a.part1 = std::to_string(depth * horizontal_pos);

    // part 2

    depth = 0;
    horizontal_pos = 0;
    int aim = 0;

    for (auto& c : data) {
        if (c.first == "forward") {
            horizontal_pos += c.second;
            depth += aim * c.second;
        } else if (c.first == "up") {
            aim -= c.second;
        } else {
            // down
            aim += c.second;
        }
    }

    a.part2 = std::to_string(depth * horizontal_pos);
/*
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
*/
    return a;
}
