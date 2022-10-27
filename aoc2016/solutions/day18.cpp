#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"


bool is_trap(const std::string& last_row, size_t pos) {
    // we assume pos is inside last_row and not the first or last entry since these are paddings for the wall
    return last_row[pos - 1] != last_row[pos + 1];
}

int gen_next_row(const std::string& last_row, std::string& next_row) {
    int count = 0;

    // only check from 1 to size() - 2 because the two outer entries are wall padding for is_trap() call
    for (size_t i = 1; i < next_row.size() - 1; ++i) {
        if (is_trap(last_row, i)) {
            next_row[i] = '^';
        } else {
            next_row[i] = '.';
            count++;
        }
    }

    return count;
}

aoch::Result solve_day18(aoch::Input& in) {
    aoch::Result r;

    int count = 0;

    for (auto c : in[0]) {
        if (c == '.') count++;
    }

    // we padd the input row with a safe place on each side to eliminate if / else in the is_trap() calls.
    // but now we have to be carefull in gen_next_row() and not check the corner indices.
    std::string last_row = "." + in[0] + '.';
    std::string next_row(last_row.size(), '.');

    int row_count = 1;
    while (row_count < 40) {
        count += gen_next_row(last_row, next_row);
        std::swap(last_row, next_row);
        row_count++;
    }

    r.part1 = std::to_string(count);


    // part 2
    while (row_count < 400000) {
        count += gen_next_row(last_row, next_row);
        std::swap(last_row, next_row);
        row_count++;
    }

    r.part2 = std::to_string(count);

    return r;
}
