#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


std::string trim(const std::string& str) {
    uint first_non_space_char_idx = 0;
    uint last_non_space_char_idx = str.length() - 1;

    while (str[first_non_space_char_idx] == ' ') {
        first_non_space_char_idx++;
    }

    while (str[last_non_space_char_idx] == ' ') {
        last_non_space_char_idx--;
    }

    return str.substr(first_non_space_char_idx, last_non_space_char_idx - first_non_space_char_idx + 1);
}

bool is_game_possible(std::vector<std::array<uint, 3>> game, std::array<uint, 3> cubes) {
    for (const auto& move : game) {
        for (uint i = 0; i < cubes.size(); ++i) {
            if (move[i] > cubes[i]) {
                return false;
            }
        }
    }

    return true;
}

aoch::Result solve_day02(aoch::Input& in) {

    aoch::Result a;

    std::map<std::string, uint> color_to_id { {"red", 0}, {"green", 1}, {"blue", 2} };
    std::vector< std::vector< std::array<uint, 3> > > games;

    for (uint i = 0; i < in.size(); ++i) {
        std::vector< std::array<uint, 3> > cur_game;
        auto game_tokens = aoch::split(aoch::split(in[i], ':')[1], ';');

        for (const auto& move : game_tokens) {
            std::array<uint, 3> cur_move {0, 0, 0};
            auto move_tokens = aoch::split(move, ',');

            for (const auto& move_token : move_tokens) {
                auto trimmed_move_token = trim(move_token);

                auto color_tokens = aoch::split(trimmed_move_token, ' ');

                cur_move[color_to_id[color_tokens[1]]] = std::stoi(color_tokens[0]);
            }

            cur_game.push_back(cur_move);
        }

        games.push_back(cur_game);
    }

    uint sum = 0;

    for (uint i = 0; i < games.size(); ++i) {
        sum += (i+1) * is_game_possible(games[i], {12, 13, 14});
    }

    a.part1 = std::to_string(sum);


    // part 2
    sum = 0;

    for (const auto& game : games) {
        std::array<uint, 3> cur_min_cubes = game[0];

        for (const auto& move : game) {
            for (uint i = 0; i < cur_min_cubes.size(); ++i) {
                cur_min_cubes[i] = std::max(cur_min_cubes[i], move[i]);
            }
        }

        sum += cur_min_cubes[0] * cur_min_cubes[1] * cur_min_cubes[2];
    }

    a.part2 = std::to_string(sum);

    return a;
}
