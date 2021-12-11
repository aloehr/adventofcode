#include <iostream>
#include <string>
#include <vector>

#include "../aoc.hpp"

void simulate(std::vector<long>& cur_state, int days) {

    std::vector<long> next_state(cur_state.size());

    while (days--) {

        for (unsigned int i = 0; i < 8; ++i)
            next_state[i] = cur_state[i+1];

        next_state[6] += cur_state[0];
        next_state[8] = cur_state[0];

        cur_state.swap(next_state);
    }
}

answer solve_day06(input& in) {

    answer a;

    std::vector<long> fish_pop_state(9);

    for (auto& s : split(in[0], ',')) {
        fish_pop_state[std::stol(s)]++;
    }

    simulate(fish_pop_state, 80);

    a.part1 = std::to_string(sum(fish_pop_state));

    // part 2

    simulate(fish_pop_state, 256 - 80);

    a.part2 = std::to_string(sum(fish_pop_state));

    return a;
}
