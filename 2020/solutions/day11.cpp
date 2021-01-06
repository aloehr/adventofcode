#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

#include "../aoc.hpp"


using state_t = std::vector<std::string>;

bool is_occupied(const state_t& state, int pos_x, int pos_y) {
    return  state[pos_y][pos_x] == '#';
}

bool is_occupied_in_dir(const state_t& state, int pos_x, int pos_y, int dir_x, int dir_y) {

    pos_x += dir_x;
    pos_y += dir_y;

    while (pos_x >= 0 && pos_x < (int) state[0].size() && pos_y >= 0 && pos_y < (int) state.size()) {

        if (state[pos_y][pos_x] != '.') {

            if (state[pos_y][pos_x] == '#')
                return true;
            else
                return false;
        }

        pos_x += dir_x;
        pos_y += dir_y;
    }

    return false;
}

void change_seat_to(const state_t& state, state_t& ns, int pos_x, int pos_y, bool& changed) {

    if (state[pos_y][pos_x] == '.') return;

    int count_occupied = 0;
    if (pos_y > 0) {
        if (pos_x > 0) count_occupied   += is_occupied(state, pos_x - 1, pos_y - 1);
        count_occupied  += is_occupied(state, pos_x + 0, pos_y - 1);
        if ((unsigned int) pos_x < state[0].size()-1) count_occupied  += is_occupied(state, pos_x + 1, pos_y - 1);
    }
    if (pos_x > 0) count_occupied  += is_occupied(state, pos_x - 1, pos_y + 0);
    if ((unsigned int) pos_x < state[0].size()-1) count_occupied  += is_occupied(state, pos_x + 1, pos_y + 0);

    if ((unsigned int) pos_y < state.size()-1) {
        if (pos_x > 0) count_occupied  += is_occupied(state, pos_x - 1, pos_y + 1);
        count_occupied  += is_occupied(state, pos_x + 0, pos_y + 1);
        if ((unsigned int) pos_x < state[0].size()-1) count_occupied  += is_occupied(state, pos_x + 1, pos_y + 1);
    }


    if (is_occupied(state, pos_x, pos_y) && count_occupied >= 4) {
        changed = true;
        ns[pos_y][pos_x] = 'L';
    }
    else if (state[pos_y][pos_x] == 'L' && count_occupied == 0) {
        changed = true;
        ns[pos_y][pos_x] = '#';
    }
    else {
        ns[pos_y][pos_x] = state[pos_y][pos_x];
    }
}

void change_seat_to2(const state_t& state, state_t& ns, int pos_x, int pos_y, bool& changed) {

    if (state[pos_y][pos_x] == '.') return;

    int count_occupied;
    count_occupied   = is_occupied_in_dir(state, pos_x, pos_y, -1, -1)
        + is_occupied_in_dir(state, pos_x, pos_y,  0, -1)
        + is_occupied_in_dir(state, pos_x, pos_y,  1, -1)
        + is_occupied_in_dir(state, pos_x, pos_y, -1,  0)
        + is_occupied_in_dir(state, pos_x, pos_y,  1,  0)
        + is_occupied_in_dir(state, pos_x, pos_y, -1,  1)
        + is_occupied_in_dir(state, pos_x, pos_y,  0,  1)
        + is_occupied_in_dir(state, pos_x, pos_y,  1,  1);

    if (is_occupied(state, pos_x, pos_y) && count_occupied >= 5) {
        changed = true;
        ns[pos_y][pos_x] = 'L';
    }
    else if (state[pos_y][pos_x] == 'L' && count_occupied == 0) {
        changed = true;
        ns[pos_y][pos_x] = '#';
    }
    else {
        ns[pos_y][pos_x] = state[pos_y][pos_x];
    }
}

void print(const state_t& d, int i) {
    std::cout << i << ":" << '\n';
    for (const auto& str : d) {
        std::cout << str << '\n';
    }
}

answer solve_day11(input& in)  {

    answer a;
    in.pop_back();

    state_t current_state = in;
    state_t next_state = in;
    
    size_t rows = in.size();
    size_t cols = in[0].size();

    bool changed = true; 

    while (changed) {
        changed = false;
        for (size_t y = 0; y < rows; ++y) {
            for (size_t x = 0; x < cols; ++x) {
                change_seat_to(current_state, next_state, x, y, changed);
            }
        }

        std::swap(current_state, next_state);
    }

    int count_occupied = 0;
    for (const auto& str : current_state) {
        for (const auto c : str) {
            count_occupied += c == '#';
        }
    }


    changed = true;
    current_state = in;

    while (changed) {
        changed = false;
        for (size_t y = 0; y < rows; ++y) {
            for (size_t x = 0; x < cols; ++x) {
                change_seat_to2(current_state, next_state, x, y, changed);
            }
        }

        std::swap(current_state, next_state);
    }

    int count_occupied2 = 0;
    for (const auto& str : current_state) {
        for (const auto c : str) {
            count_occupied2 += c == '#';
        }
    }

    a.part1 = std::to_string(count_occupied);
    a.part2 = std::to_string(count_occupied2);

    return a;
}
