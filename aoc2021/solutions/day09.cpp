#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <set>

#include "../aoc.hpp"

int calc_basin_size(input& data, size_t y, size_t x, bool init = true) {

    static std::set<std::array<size_t, 2>> visited;
    if (init) visited.clear();

    // current location inbound
    if (y < 0 || x < 0 || y >= data.size() || x >= data[y].size()) {
        return 0;
    }

    // height 9 not part of basin
    if (data[y][x] == '9')
        return 0;

    // have we already visited this place?
    if (!visited.insert({y, x}).second) {
        return 0;
    }

    return 1 + calc_basin_size(data, y+1, x, false)
             + calc_basin_size(data, y-1, x, false)
             + calc_basin_size(data, y, x+1, false)
             + calc_basin_size(data, y, x-1, false);
}

answer solve_day09(input& in) {

    answer a;

    int risk_level = 0;
    std::vector<std::array<size_t, 2>> low_points;


    for (size_t i = 0; i < in.size(); ++i) {
        for (size_t j = 0; j < in[i].size(); ++j) {

            int c = 0;
            int cur_pos_val = in[i][j];
            if (i < 1 || in[i-1][j] > cur_pos_val) c++;
            if (i >= in.size() - 1 || in[i+1][j] > cur_pos_val) c++;
            if (j < 1 || in[i][j-1] > cur_pos_val) c++;
            if (j >= in[i].size() - 1 || in[i][j+1] > cur_pos_val) c++;

            if (c == 4) {
                risk_level += (cur_pos_val - '0') + 1;
                low_points.push_back({i, j});
            }

        }
    }

    a.part1 = std::to_string(risk_level);


    // part 2

    std::vector<int> basin_sizes;
    for (const auto& lp : low_points) {
        basin_sizes.push_back(calc_basin_size(in, lp[0], lp[1]));
    }

    std::sort(basin_sizes.begin(), basin_sizes.end());

    size_t bss = basin_sizes.size();
    a.part2 = std::to_string(basin_sizes[bss-1] * basin_sizes[bss-2] * basin_sizes[bss-3]);

    return a;
}
