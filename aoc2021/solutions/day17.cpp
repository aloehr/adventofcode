#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <tuple>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


std::pair<bool, int> will_hit(std::array<int, 2> probe_pos, std::array<int, 2> probe_vel, const std::array<int, 2>& x_area, const std::array<int, 2>& y_area) {

    int y_max = probe_pos[1];

    while (probe_pos[0] <= x_area[1] && probe_pos[1] >= y_area[0]) {

        if (probe_pos[0] >= x_area[0] && probe_pos[1] <= y_area[1]) {
            return std::make_pair(true, y_max);
        }

        probe_pos[0] += probe_vel[0];
        probe_pos[1] += probe_vel[1];

        probe_vel[1]--;

        if (probe_vel[0] > 0) probe_vel[0]--;
        else if (probe_vel[0] < 0) probe_vel[0]++;

        probe_vel[0] = std::max(0, probe_vel[0]);

        y_max = std::max(y_max, probe_pos[1]);
    }

    return std::make_pair(false, 0);
}

aoch::Result solve_day17(aoch::Input& in) {

    aoch::Result a;

    auto tmp = aoch::split(in[0]);
    auto xstr = tmp[2].substr(2, tmp[2].size() - 3);
    auto ystr = tmp[3].substr(2);

    size_t idx = 0;

    std::array<int, 2> x_area = {std::stoi(xstr, &idx), std::stoi(xstr.substr(idx+2))};
    std::array<int, 2> y_area = {std::stoi(ystr, &idx), std::stoi(ystr.substr(idx+2))};

    std::array<int, 2> probe_init_pos = {0, 0};
    int min_x_vel = std::ceil(-0.5 + std::sqrt(0.25 + ((float) 2*x_area[0])));
    int max_x_vel = x_area[1];
    int min_y_vel = y_area[0];
    int max_y_vel = std::abs(y_area[0]) - 1;

    int hit_count = 0;
    int y_max = 0;
    std::array<int, 2> best_vel {-1, -1};
    for (int x = min_x_vel; x <= max_x_vel; ++x) {
        for (int y = min_y_vel; y <= max_y_vel; ++y) {

            auto [hit, c_y_max] = will_hit(probe_init_pos, {x, y}, x_area, y_area);

            if (hit) {
                hit_count++;

                if (c_y_max > y_max) {
                    y_max = c_y_max;
                    best_vel = {x, y};
                }
            }
        }
    }

    a.part1 = std::to_string(y_max);
    a.part2 = std::to_string(hit_count);

    return a;
}
