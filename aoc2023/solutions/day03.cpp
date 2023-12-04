#include <array>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"

static bool is_digit(char x) {
    return x >= '0' && x <= '9';
}

bool has_pos_adjacent_symbol(std::vector<std::string> map, std::array<uint, 2> pos) {

    char cur;

    // row above
    if (pos[1] > 0) {
        if (pos[0] > 0) {
            cur = map[pos[1] - 1][pos[0] - 1];
            if (!is_digit(cur) && cur != '.') return true;
        }

        cur = map[pos[1] - 1][pos[0]];
        if (!is_digit(cur) && cur != '.') return true;

        if (pos[0] < map[pos[1]].size() - 1) {
            cur = map[pos[1] - 1][pos[0] + 1];
            if (!is_digit(cur) && cur != '.') return true;
        }
    }

    // cur row
    if (pos[0] > 0) {
        cur = map[pos[1]][pos[0] - 1];
        if (!is_digit(cur) && cur != '.') return true;
    }

    if (pos[0] < map[pos[1]].size() - 1) {
        cur = map[pos[1]][pos[0] + 1];
        if (!is_digit(cur) && cur != '.') return true;
    }

    // row below above
    if (pos[1] < map.size() - 1) {
        if (pos[0] > 0) {
            cur = map[pos[1] + 1][pos[0] - 1];
            if (!is_digit(cur) && cur != '.') return true;
        }

        cur = map[pos[1] + 1][pos[0]];
        if (!is_digit(cur) && cur != '.') return true;

        if (pos[0] < map[pos[1]].size() - 1) {
            cur = map[pos[1] + 1][pos[0] + 1];
            if (!is_digit(cur) && cur != '.') return true;
        }
    }

    return false;
}

aoch::Result solve_day03(aoch::Input& in) {

    aoch::Result a;

    uint sum = 0;

    std::map<std::string, uint> id_to_number;
    std::map<std::array<uint, 2>, std::string> pos_to_id;

    for (uint y = 0; y < in.size(); ++y) {

        bool found_symbol_for_current_number = false;
        std::vector<std::array<uint, 2>> number_positions;
        std::string pos_str = "";
        std::string cur_number = "";
        for (uint x = 0; x < in[y].size(); ++x) {
            if (!is_digit(in[y][x])) {

                if (cur_number != "") {
                    pos_str += std::to_string(x-1) + ":" + std::to_string(y);
                    id_to_number[pos_str] = std::stoi(cur_number);

                    for (const auto& pos : number_positions) {
                        pos_to_id[pos] = pos_str;
                    }
                }
                if (found_symbol_for_current_number) {
                    sum += std::stoi(cur_number);
                }
                found_symbol_for_current_number = false;
                cur_number = "";
                number_positions.clear();

            } else {
                if (cur_number == "") {
                    pos_str = std::to_string(x) + ":" + std::to_string(y) + ";";
                }

                number_positions.push_back({x,y});

                cur_number += in[y][x];

                if (has_pos_adjacent_symbol(in, {x, y})) {
                    found_symbol_for_current_number = true;
                }
            }

        }

        if (found_symbol_for_current_number) {
            sum += std::stoi(cur_number);
        }

        if (cur_number != "") {
            pos_str += std::to_string(in[y].size() - 1) + ":" + std::to_string(y);
            id_to_number[pos_str] = std::stoi(cur_number);

            for (const auto& pos : number_positions) {
                pos_to_id[pos] = pos_str;
            }

            number_positions.clear();
        }
    }

    a.part1 = std::to_string(sum);


    // part 2
    sum = 0;

    for (uint y = 0; y < in.size(); ++y) {
        for (uint x = 0; x < in[y].size(); ++x) {
            if (in[y][x] == '*') {
                std::set<std::string> number_ids;

                std::array<uint, 2> cur_pos = {x-1, y-1};
                if (pos_to_id.count(cur_pos)) number_ids.insert(pos_to_id[cur_pos]);

                cur_pos = {x, y-1};
                if (pos_to_id.count(cur_pos)) number_ids.insert(pos_to_id[cur_pos]);

                cur_pos = {x+1, y-1};
                if (pos_to_id.count(cur_pos)) number_ids.insert(pos_to_id[cur_pos]);

                cur_pos = {x-1, y};
                if (pos_to_id.count(cur_pos)) number_ids.insert(pos_to_id[cur_pos]);

                cur_pos = {x+1, y};
                if (pos_to_id.count(cur_pos)) number_ids.insert(pos_to_id[cur_pos]);

                cur_pos = {x-1, y+1};
                if (pos_to_id.count(cur_pos)) number_ids.insert(pos_to_id[cur_pos]);

                cur_pos = {x, y+1};
                if (pos_to_id.count(cur_pos)) number_ids.insert(pos_to_id[cur_pos]);

                cur_pos = {x+1, y+1};
                if (pos_to_id.count(cur_pos)) number_ids.insert(pos_to_id[cur_pos]);

                if (number_ids.size() == 2) {
                    uint tmp = 1;

                    for (const auto& id : number_ids) {
                        tmp *= id_to_number[id];
                    }

                    sum += tmp;
                }
            }
        }
    }

    a.part2 = std::to_string(sum);

    return a;
}
