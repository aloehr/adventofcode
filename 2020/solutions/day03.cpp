#include <vector>
#include <string>

#include "../aoc.hpp"

unsigned int check_slope(const std::vector<std::string>& data, size_t step_right, size_t step_down) {

    int count_tree = 0;

    size_t current_right = step_right;
    size_t current_down = step_down;
    while (current_down < data.size()) {
        if (data[current_down][current_right] == '#') {
            count_tree++; 
        }
        else {
        }

        current_right = (current_right + step_right) % data[0].size();
        current_down += step_down;
    }

    return count_tree;
}

answer solve_day03(input& in) {

    answer a;
    in.pop_back();
    
    unsigned int part1 = check_slope(in, 3, 1);

    a.part1 = std::to_string(part1);
    a.part2 = std::to_string(check_slope(in, 1, 1) * part1 * check_slope(in, 5, 1) * check_slope(in, 7, 1) * check_slope(in, 1, 2));

    return a;
}
