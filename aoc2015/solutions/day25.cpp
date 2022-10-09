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

// @TODO put something like this into aoc_helper lib
long long powmod_by_squaring(long long x, long long e, const long long mod) {
    if (e == 0) return 1;
    if (e == 1) return x % mod;

    if (e % 2 == 0) {
        return powmod_by_squaring((x * x) % mod, e/2, mod);
    } else {
        return (x * powmod_by_squaring((x * x) % mod, e/2, mod)) % mod;
    }
}

aoch::Result solve_day25(aoch::Input& in) {
    aoch::Result r;

    auto tokens = aoch::split(in[0]);

    int row = std::stoi(tokens[15]);
    int column = std::stoi(tokens[17]);

    const int first_code = 20151125;
    const int mult = 252533;
    const int mod = 33554393;

    int rounds = calc_rounds_from(row, column);

    int res = (first_code * powmod_by_squaring(mult, rounds, mod)) % mod;

    r.part1 = std::to_string(res);
    r.part2 = "NA";

    return r;
}
