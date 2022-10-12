#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"


aoch::Result solve_day06(aoch::Input& in) {
    aoch::Result r;

    std::vector<std::array<int, 26>> data(in[0].size());

    for (auto& l : in) {
        for (size_t i = 0; i < l.size(); ++i) {
            data[i][l[i] - 'a']++;
        }
    }

    for (size_t i = 0; i < data.size(); ++i) {
        int best = data[i][0];
        int best_idx = 0;

        int least_common = data[i][0];
        int least_common_idx = 0;

        for (size_t j = 1; j < data[i].size(); ++j) {
            if (best < data[i][j]) {
                best = data[i][j];
                best_idx = j;
            }

            if (data[i][j] != 0 && (least_common == 0 || least_common > data[i][j])) {
                least_common = data[i][j];
                least_common_idx = j;
            }
        }

        r.part2 += 'a' + least_common_idx;
        r.part1 += 'a' + best_idx;
    }

    return r;
}
