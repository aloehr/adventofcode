#include <array>
#include <iostream>
#include <limits>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


int win_against_boss_cost(std::array<int, 3> weapon, std::array<int, 3> armor, std::array<int, 3> ring1, std::array<int, 3> ring2, std::array<int, 3> boss) {
    int player_hp = 100;
    int player_dmg = weapon[1] + ring1[1] + ring2[1];
    int player_def = armor[2] + ring1[2] + ring2[2];
    int cost = weapon[0] + armor[0] + ring1[0] + ring2[0];

    int dmg_to_boss = std::max(1, player_dmg - boss[2]);
    int dmg_to_player = std::max(1, boss[1] - player_def);

    int turns_until_boss_defeated = boss[0] / dmg_to_boss + ((boss[0] % dmg_to_boss) != 0);
    int turns_until_player_defeated = player_hp / dmg_to_player + ((player_hp % dmg_to_player) != 0);

    if (turns_until_boss_defeated <= turns_until_player_defeated) {
        return cost;
    } else {
        return -1;
    }
}

int lose_against_boss_cost(std::array<int, 3> weapon, std::array<int, 3> armor, std::array<int, 3> ring1, std::array<int, 3> ring2, std::array<int, 3> boss) {
    int player_hp = 100;
    int player_dmg = weapon[1] + ring1[1] + ring2[1];
    int player_def = armor[2] + ring1[2] + ring2[2];
    int cost = weapon[0] + armor[0] + ring1[0] + ring2[0];

    int dmg_to_boss = std::max(1, player_dmg - boss[2]);
    int dmg_to_player = std::max(1, boss[1] - player_def);

    int turns_until_boss_defeated = boss[0] / dmg_to_boss + ((boss[0] % dmg_to_boss) != 0);
    int turns_until_player_defeated = player_hp / dmg_to_player + ((player_hp % dmg_to_player) != 0);

    if (turns_until_boss_defeated <= turns_until_player_defeated) {
        return -1;
    } else {
        return cost;
    }
}

aoch::Result solve_day21(aoch::Input& in) {
    aoch::Result r;

    int boss_hp = std::stoi(aoch::split(in[0])[2]);
    int boss_dmg = std::stoi(aoch::split(in[1])[1]);
    int boss_def = std::stoi(aoch::split(in[2])[1]);

    std::array<int, 3> boss {boss_hp, boss_dmg, boss_def};

    std::vector<std::array<int, 3>> weapons {
        {8,  4, 0},
        {10, 5, 0},
        {25, 6, 0},
        {40, 7, 0},
        {74, 8, 0}
    };

    std::vector<std::array<int, 3>> armors {
        {0,   0, 0},
        {13,  0, 1},
        {31,  0, 2},
        {53,  0, 3},
        {75,  0, 4},
        {102, 0, 5},
    };

    std::vector<std::array<int, 3>> rings {
        {0,   0, 0},
        {0,   0, 0},
        {25,  1, 0},
        {50,  2, 0},
        {100, 3, 0},
        {20,  0, 1},
        {40,  0, 2},
        {80,  0, 3},
    };

    int best_cost = std::numeric_limits<int>::max();
    int best_cost2 = 0;

    for (size_t i = 0; i < weapons.size(); ++i) {
        for (size_t j = 0; j < armors.size(); ++j) {
            for (size_t k = 0; k < rings.size()-1; ++k) {
                if (k == 1) continue;

                for (size_t l = k+1; l < rings.size(); ++l) {
                    int current_cost = win_against_boss_cost(weapons[i], armors[j], rings[k], rings[l], boss);
                    int current_cost2 = lose_against_boss_cost(weapons[i], armors[j], rings[k], rings[l], boss);

                    if (current_cost >= 0) {
                        best_cost = std::min(best_cost, current_cost);
                    }
                    if (current_cost2 >= 0) {
                        best_cost2 = std::max(best_cost2, current_cost2);
                    }
                }
            }
        }
    }

    r.part1 = std::to_string(best_cost);
    r.part2 = std::to_string(best_cost2);

    return r;
}
