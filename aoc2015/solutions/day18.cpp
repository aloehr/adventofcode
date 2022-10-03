#include <iostream>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"


void simulate(std::vector<std::string>& data, int steps, bool part2 = false) {
    auto next_step = data;

    while (steps--) {
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data[i].size(); ++j) {
                int c = 0;

                if (i > 0) {
                    if (j > 0 && data[i-1][j-1] == '#') c++;
                    if (data[i-1][j] == '#') c++;
                    if ((j < data[i-1].size() - 1) && data[i-1][j+1] == '#') c++;
                }

                if (j > 0 && data[i][j-1] == '#') c++;
                if ((j < data[i].size() - 1) && data[i][j+1] == '#') c++;

                if (i < data.size() - 1) {
                    if (j > 0 && data[i+1][j-1] == '#') c++;
                    if (data[i+1][j] == '#') c++;
                    if ((j < data[i+1].size() - 1) && data[i+1][j+1] == '#') c++;
                }

                if (data[i][j] == '#') {
                    if (c == 2 || c == 3) {
                        next_step[i][j] = '#';
                    } else {
                        next_step[i][j] = '.';
                    }
                } else {
                    if (c == 3) {
                        next_step[i][j] = '#';
                    } else {
                        next_step[i][j] = '.';
                    }
                }
            }
        }

        if (part2) {
            next_step[0][0] = '#';
            next_step[next_step.size() - 1][0] = '#';
            next_step[0][next_step[0].size() - 1] = '#';
            next_step[next_step.size() - 1][next_step[next_step.size() - 1].size() - 1] = '#';
        }

        std::swap(data, next_step);
    }
}

int count_on(const std::vector<std::string>& data) {
    int count = 0;

    for (const auto& l : data) {
        for (auto c : l) {
            if (c == '#') count++;
        }
    }

    return count;
}

aoch::Result solve_day18(aoch::Input& in) {
    aoch::Result r;

    auto copy = in;

    simulate(in, 100);
    r.part1 = std::to_string(count_on(in));


    // part 2
    copy[0][0] = '#';
    copy[copy.size() - 1][0] = '#';
    copy[0][copy[0].size() - 1] = '#';
    copy[copy.size() - 1][copy[copy.size() - 1].size() - 1] = '#';

    simulate(copy, 100, true);
    r.part2 = std::to_string(count_on(copy));

    return r;
}
