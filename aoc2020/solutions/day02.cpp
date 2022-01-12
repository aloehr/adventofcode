#include <iostream>
#include <string>
#include <vector>

#include "../aoc.hpp"

answer solve_day02(input& in) {

    answer a;
    in.pop_back();

    int valid_pw_count = 0;
    int valid_pw_count2 = 0;

    for (auto& line : in) {
        size_t idx = 0;
        int min_count = std::stoi(line, &idx);

        line = line.substr(idx+1);

        int max_count = std::stoi(line, &idx);

        char c = line[idx+1];
        idx += 4;

        int count = 0;

        for (size_t i = idx; i < line.size(); ++i) {
            if (line[i] == c) count++;
        }

        if (count >= min_count && count <= max_count) valid_pw_count++;

        size_t pos1 = min_count - 1;
        size_t pos2 = max_count - 1;

        if ((line[idx + pos1] == c && line[idx + pos2] != c) || (line[idx + pos1] != c && line[idx + pos2] == c)) valid_pw_count2++;
    }

    a.part1 = std::to_string(valid_pw_count);
    a.part2 = std::to_string(valid_pw_count2);

    return a;
}
