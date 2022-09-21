#include <array>
#include <cmath>
#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


struct cmd {
    int action; // 0 = turn on, 1 = turn off, 2 = toggle
    std::array<size_t, 2> r1;
    std::array<size_t, 2> r2;

    void print() {
        if (action == 0)
            std::cout << "turn on;";
        else if (action == 1)
            std::cout << "turn off;";
        else
            std::cout << "toggle;";

        std::cout << r1[0] << "," << r1[1] << " - " << r2[0] << "," << r2[1] << '\n';
    }
};

aoch::Result solve_day06(aoch::Input& in) {
    aoch::Result r;

    std::array<bool, 1000000> grid;
    grid.fill(false);
    std::vector<cmd> cmds(in.size());


    for (size_t i = 0; i < in.size(); ++i) {
        auto tokens = aoch::split(in[i]);

        if (tokens[0] == "toggle") {
            cmds[i].action = 2;

            auto tmp = aoch::split(tokens[1], ',');
            cmds[i].r1[0] = std::stoi(tmp[0]);
            cmds[i].r1[1] = std::stoi(tmp[1]);

            tmp = aoch::split(tokens[3], ',');
            cmds[i].r2[0] = std::stoi(tmp[0]);
            cmds[i].r2[1] = std::stoi(tmp[1]);
        } else  {
            cmds[i].action = tokens[1] == "on" ? 0 : 1;

            auto tmp = aoch::split(tokens[2], ',');
            cmds[i].r1[0] = std::stoi(tmp[0]);
            cmds[i].r1[1] = std::stoi(tmp[1]);

            tmp = aoch::split(tokens[4], ',');
            cmds[i].r2[0] = std::stoi(tmp[0]);
            cmds[i].r2[1] = std::stoi(tmp[1]);
        }
    }

    for (const auto& c : cmds) {
        for (size_t x = std::min(c.r1[0], c.r2[0]); x <= std::max(c.r1[0], c.r2[0]); ++x) {
            for (size_t y = std::min(c.r1[1], c.r2[1]); y <= std::max(c.r1[1], c.r2[1]); ++y) {

                size_t idx = y * 1000 + x;

                if (c.action == 0) {
                    grid[idx] = true;
                } else if (c.action == 1) {
                    grid[idx] = false;
                } else {
                    grid[idx] = !grid[idx];
                }
            }
        }
    }


    int on_count = 0;
    for (size_t i = 0; i < grid.size(); ++i) {
        if (grid[i]) on_count++;
    }

    r.part1 = std::to_string(on_count);


    // part 2
    std::array<int, 1000000> grid2;
    grid2.fill(0);

    for (const auto& c : cmds) {
        for (size_t x = std::min(c.r1[0], c.r2[0]); x <= std::max(c.r1[0], c.r2[0]); ++x) {
            for (size_t y = std::min(c.r1[1], c.r2[1]); y <= std::max(c.r1[1], c.r2[1]); ++y) {

                size_t idx = y * 1000 + x;

                if (c.action == 0) {
                    grid2[idx]++;
                } else if (c.action == 1) {
                    grid2[idx] = std::max(0, grid2[idx] - 1);
                } else {
                    grid2[idx] += 2;
                }
            }
        }
    }

    long total_brightness = 0;

    for (size_t i = 0; i < grid.size(); ++i) {
        total_brightness += grid2[i];
    }

    r.part2 = std::to_string(total_brightness);

    return r;
}
