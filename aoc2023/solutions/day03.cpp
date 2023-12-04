#include <array>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"


using Position = std::array<uint, 2>;
using Delta = std::array<int, 2>;

static bool is_digit(char x) {
    return x >= '0' && x <= '9';
}

bool is_inbound(const std::vector<std::string>& map, const Position& pos, const Delta& dt) {
    if (dt[0] < 0 && pos[0] == 0) return false;
    if (pos[0] + dt[0] >= map.size()) return false;

    if (dt[1] < 0 && pos[1] == 0) return false;
    if (pos[1] + dt[1] >= map[pos[0] + dt[0]].size()) return false;

    return true;
}

bool has_pos_adjacent_symbol(const std::vector<std::string>& map, const Position& pos) {

    const std::vector<Delta> dirs { {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1} };

    for (const auto& dt : dirs) {
        if (is_inbound(map, pos, dt)) {
            Position cur_pos {pos[0] + dt[0], pos[1] + dt[1]};

            char cur_char = map[cur_pos[1]][cur_pos[0]];

            if (!is_digit(cur_char) && cur_char != '.') return true;
        }
    }

    return false;
}

aoch::Result solve_day03(aoch::Input& in) {

    aoch::Result a;

    uint sum = 0;

    std::map<std::string, uint> id_to_number;
    std::map<Position, std::string> pos_to_id;

    for (uint y = 0; y < in.size(); ++y) {

        bool found_symbol_for_current_number = false;
        std::vector<Position> number_positions;
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

                const std::vector<Delta> dirs { {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1} };

                for (const auto& dir : dirs) {
                    Position cur_pos = {x + dir[0], y + dir[1]};

                    if (pos_to_id.count(cur_pos)) {
                        number_ids.insert(pos_to_id[cur_pos]);
                    }
                }

                if (number_ids.size() == 2) {
                    uint ratio = 1;

                    for (const auto& id : number_ids) {
                        ratio *= id_to_number[id];
                    }

                    sum += ratio;
                }
            }
        }
    }

    a.part2 = std::to_string(sum);

    return a;
}
