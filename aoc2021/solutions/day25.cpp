#include <iostream>
#include <string>
#include <vector>

#include "../aoc.hpp"


bool step(input& data) {
    bool moved = false;
    input next_state = data;

    size_t w = data[0].size();
    size_t h = data.size();

    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            if (data[y][x] == '>') {
                if (data[y][(x+1) % w] == '.') {
                    next_state[y][x] = '.';
                    next_state[y][(x+1) % w] = '>';
                    moved = true;
                }
            }
        }
    }

    data = next_state;
    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            if (data[y][x] == 'v') {
                if (data[(y+1) % h][x] == '.') {
                    next_state[y][x] = '.';
                    next_state[(y+1) % h][x] = 'v';
                    moved = true;
                }
            }
        }
    }

    std::swap(data, next_state);
    return moved;
}

answer solve_day25(input& in) {

    answer a;

    int steps = 1;
    while (step(in)) {
        steps++;
    }

    a.part1 = std::to_string(steps);
    a.part2 = "NA";

    return a;
}
