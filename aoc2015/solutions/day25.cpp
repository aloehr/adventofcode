#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"

int calc_rounds_from(int row, int column) {
    int  last_full_row = row + column - 2;

    int result = (last_full_row * last_full_row + last_full_row) / 2;
    result += column;

    // because we start with 1st code already known we need to do one round less than
    // the number want to calculate
    return result - 1;
}

aoch::Result solve_day25(aoch::Input& in) {
    aoch::Result r;

    auto tokens = aoch::split(in[0]);

    int row = std::stoi(tokens[15]);
    int column = std::stoi(tokens[17]);

    long long first_code = 20151125;
    int mult = 252533;
    int mod = 33554393;

    long long current_code = first_code;
    int rounds = calc_rounds_from(row, column);

    while (rounds--) {
        current_code = (current_code * mult) % mod;
    }

    r.part1 = std::to_string(current_code);
    r.part2 = "NA";

    return r;
}
