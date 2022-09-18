#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"

aoch::Result solve_day01(aoch::Input& in) {
    aoch::Result r;

    int floor = 0;
    bool first_basement_visit = true;

    for (size_t i = 0; i < in[0].size(); ++i) {
        char c = in[0][i];
        if (c == '(') {
            floor++;
        } else {
            floor--;
        }

        if (floor < 0 && first_basement_visit) {
            first_basement_visit = false;
            r.part2 = std::to_string(i + 1);
        }
    }

    r.part1 = std::to_string(floor);

    return r;
}
