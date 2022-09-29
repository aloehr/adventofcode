#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"

aoch::Result solve_day14(aoch::Input& in) {
    aoch::Result r;

    std::vector<std::array<int, 3>> reindeers;

    for (auto& l : in) {
        auto tokens = aoch::split(l);

        int fly_distance = std::stoi(tokens[3]);
        int fly_duration = std::stoi(tokens[6]);
        int rest_duration = std::stoi(tokens[13]);

        reindeers.push_back({fly_distance, fly_duration, rest_duration});
    }

    const int total_time = 2503;

    int longest_distance = 0;

    for (auto& re : reindeers) {

        int fly_and_rest_duration = re[1] + re[2];
        int full_cycles = total_time / fly_and_rest_duration;
        int distance = re[0] * re[1] * full_cycles;
        distance += re[0] * std::min(total_time % fly_and_rest_duration, re[1]);

        longest_distance = std::max(longest_distance, distance);
    }

    r.part1 = std::to_string(longest_distance);

    // part 2

    // resting/flying, remaining rest/fly time, traveled distance, score
    std::vector<std::array<int, 4>> reindeer_states(reindeers.size(), {0, 0, 0, 0});

    int t = 0;

    while (t < total_time) {

        // update reindeers
        for (size_t i = 0; i < reindeers.size(); ++i) {
            auto& re = reindeer_states[i];
            if (re[0] == 0) {
                re[2] += reindeers[i][0];
                re[1]++;

                if (re[1] == reindeers[i][1]) {
                    re[1] = 0;
                    re[0] = 1;
                }
            } else {
                re[1]++;

                if (re[1] == reindeers[i][2]) {
                    re[1] = 0;
                    re[0] = 0;
                }

            }
        }

        // give leading reindeers 1 point
        int leading_distance = 0;

        for (auto& s : reindeer_states) {
            leading_distance = std::max(leading_distance, s[2]);
        }

        for (auto& s : reindeer_states) {
            if (s[2] == leading_distance) {
                s[3]++;
            }
        }

        t++;
    }

    int best_score = 0;
    for (auto& s : reindeer_states) {
        best_score = std::max(best_score, s[3]);
    }

    r.part2 = std::to_string(best_score);

    return r;

}
