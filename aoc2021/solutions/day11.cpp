#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

#include "aoch/AOCSolutionTypes.hpp"


void update_flash(std::vector<std::vector<int>>& d, size_t i, size_t j) {

    if (i > 0) {
        d[i-1][j]++;
        if (j > 0) d[i-1][j-1]++;
        if (j < d[0].size()-1) d[i-1][j+1]++;
    }

    if (i < d.size() - 1) {
        d[i+1][j]++;
        if (j < d[0].size() - 1) d[i+1][j+1]++;
        if (j > 0) d[i+1][j-1]++;
    }

    if (j > 0) d[i][j-1]++;
    if (j < d[0].size() - 1) d[i][j+1]++;

}

std::array<int, 2> step(std::vector<std::vector<int>>& d, int steps, bool p2 = false) {
    int fc = 0;
    int faf = 0;

    int cs = 0;
    size_t fsc = 0;

    std::vector<bool> has_flashed(100);
    bool flashed = false;

    while (cs++ < steps || (p2 && !faf)) {

        for (auto& l : d) {
            for (auto& n : l) {
                n++;
            }
        }


        has_flashed.clear();
        fsc = 0;

        do {
            flashed = false;
            for (size_t i = 0; i < 10; ++i) {
                for (size_t j = 0; j < 10; ++j) {
                    if (has_flashed[i*10 + j]) continue;

                    if (d[i][j] > 9) {
                        flashed = true;

                        update_flash(d, i, j);
                        has_flashed[i * 10 + j] = true;
                        fsc++;
                        fc++;
                    }
                }
            }
        } while (flashed);

        if (fsc == 100 && !faf) {
            faf = cs;
        }

        for (size_t i = 0; i < 10; ++i) {
            for (size_t j = 0; j < 10; ++j) {
                if (has_flashed[i * 10 + j]) d[i][j] = 0;
                has_flashed[i * 10 + j] = false;
            }
        }
    }

    return {fc, faf};
}


aoch::Result solve_day11(aoch::Input& in) {

    aoch::Result a;

    std::vector<std::vector<int>> op_el(in.size());

    for (size_t i = 0; i < 10; ++i) {
        op_el[i].reserve(10);
        for (auto c : in[i]) {
            op_el[i].push_back(c - '0');
        }
    }

    a.part1 = std::to_string(step(op_el, 100)[0]);

    auto r = step(op_el, 0, true);
    a.part2 = std::to_string(r[1] + 100);

    return a;
}
