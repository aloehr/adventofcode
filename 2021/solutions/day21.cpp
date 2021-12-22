#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>

#include "../aoc.hpp"


answer solve_day21(input& in) {
    answer a;

    int p1_start_pos = std::stoi(split(in[0]).back()) - 1;
    int p2_start_pos = std::stoi(split(in[1]).back()) - 1;

    int dice_roll_count = 0;
    int p1_pos = p1_start_pos;
    int p2_pos = p2_start_pos;
    int p1_score = 0;
    int p2_score = 0;

    while(p1_score < 1000 || p2_score < 1000) {
        p1_pos += 3 * dice_roll_count + 6;
        dice_roll_count += 3;
        p1_pos %= 10;

        p1_score += p1_pos + 1;

        if (p1_score >= 1000) break;

        p2_pos += 3 * dice_roll_count + 6;
        dice_roll_count += 3;
        p2_pos %= 10;

        p2_score += p2_pos + 1;

        if (p2_score >= 1000) break;
    }

    int loser_score = std::min(p1_score, p2_score);

    a.part1 = std::to_string(dice_roll_count * loser_score);


    long p1_wins = 0;
    long p2_wins = 0;

    // {number of rolls, roll outcome sum}
    std::vector<std::array<int, 2>> dice_rolls {
        {1, 3},
        {3, 4},
        {6, 5},
        {7, 6},
        {6, 7},
        {3, 8},
        {1, 9},
    };

    std::vector<std::vector<long>> states(21 * 10, std::vector<long>(21 * 10));

    // pos * 21 + score
    states[(p1_start_pos) * 21 + 0][(p2_start_pos) * 21 + 0] = 1;

    bool unfinished_games = true;

    while(unfinished_games) {
        unfinished_games = false;

        for (size_t i = 0; i < states.size(); ++i) {

            int p1_pos = i / 21;
            int p1_score = i % 21;

            for (size_t j = 0; j < states.size(); ++j) {

                // we have no games with this state
                if (!states[i][j]) continue;


                int p2_pos = j / 21;
                int p2_score = j % 21;


                for (const auto& p1_r : dice_rolls) {

                    int cur_p1_pos = (p1_pos + p1_r[1]) % 10;
                    int cur_p1_score = p1_score + cur_p1_pos + 1;
                    auto count = states[i][j] * p1_r[0];

                    if (cur_p1_score >= 21) {
                        p1_wins += count;
                        continue;
                    }

                    for (const auto& p2_r : dice_rolls) {

                        int cur_p2_pos = (p2_pos + p2_r[1]) % 10;
                        int cur_p2_score = p2_score + cur_p2_pos + 1;
                        auto count2 = count * p2_r[0];

                        if (cur_p2_score >= 21) {
                            p2_wins += count2;
                            continue;
                        }

                        size_t p1_idx = cur_p1_pos * 21 + cur_p1_score;
                        size_t p2_idx = cur_p2_pos * 21 + cur_p2_score;

                        states[p1_idx][p2_idx] += count2;
                        unfinished_games = true;
                    }
                }

                states[i][j] = 0;
            }
        }
    }

    a.part2 = std::to_string(std::max(p1_wins, p2_wins));

    return a;
}
