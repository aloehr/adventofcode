#include <iostream>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"


int presents_for_house2(int house) {
    int sum = 0;

    for (int i = 1; i <= 50; ++i) {
        if ((house % i) == 0) {
            sum += house / i * 11;
        }
    }

    return sum;
}

aoch::Result solve_day20(aoch::Input& in) {
    aoch::Result r;

    int min_number_presents = std::stoi(in[0]);
    int max = min_number_presents / 10;
    std::vector<int> houses(max+1, 0);

    for (int i = 1; i <= max; ++i) {
        for (int j = i; j <= max; j += i) {
                houses[j] += i;
        }
    }

    for (size_t i = 1; i < houses.size(); ++i) {
        if (houses[i] >= max) {
            r.part1 = std::to_string(i);
            break;
        }
    }

    // part 2
    for (int i = 1; i < max; ++i) {
        if (presents_for_house2(i) >= min_number_presents) {
            r.part2 = std::to_string(i);
            break;
        }
    }

    return r;
}
