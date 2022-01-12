#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <array>

#include "../aoc.hpp"


int cps(const char c) {
    if (c == 'A') return 1;
    else if (c == 'B') return 10;
    else if (c == 'C') return 100;
    else if (c == 'D') return 1000;

    return -1;
}

bool can_move_to(const std::string& h, const size_t from, const size_t to) {

    int move_dir = from <  to ? 1 : -1;

    if (h[to] != '.') return false;

    for (size_t i = from + move_dir; i != to; i+= move_dir) {
        if (h[i] != '.') return false;
    }

    return true;
}

size_t calc_move_cost(const std::string& h, const std::vector<std::string>& rs, size_t i, size_t j, size_t k) {
    size_t room_pos = 2 + 2 * j;
    int steps = (k + 1) + (i < room_pos ? room_pos - i : i - room_pos);

    char type = h[i];
    if (type == '.') {
        type = rs[j][k];
    }

    return steps * cps(type);
}

bool is_room_entrance(const size_t i) {
    return i == 2 || i == 4 || i == 6 || i == 8;
}

std::tuple<int, bool> bruteforce(std::string& h, std::vector<std::string>& rs, int cost = 0, int lowest_cost = -1) {

    static std::map<std::string, int> mem;

    // base case
    bool found_solution = true;
    for (size_t i = 0; i < rs.size() && found_solution; ++i) {
        for (auto c : rs[i]) {
            if (c != ('A' + (char)i)) {
                found_solution = false;
                break;
            }
        }
    }

    if (found_solution) {
        return std::make_tuple(cost, true);
    }

    std::string x = h + rs[0] + rs[1] + rs[2] + rs[3];
    mem[x] = cost;

    // first we find all possible moves for amphipods in the hallway
    std::vector<std::array<size_t, 4>> possible_moves;

    for (size_t i = 0; i < h.size(); ++i) {
        if (!is_room_entrance(i) && h[i] != '.') {

            size_t room_id = h[i] - 'A';
            size_t room_pos = 2 + (room_id * 2);

            if (can_move_to(h, i, room_pos)) {
                for (size_t j = rs[room_id].size(); j > 0; --j) {

                    if (rs[room_id][j-1] == '.') {
                        size_t move_cost = calc_move_cost(h, rs, i, room_id, j-1);
                        possible_moves.push_back({i, room_id, j-1, move_cost});
                        break;
                    } else if (rs[room_id][j-1] != h[i]) {
                        break;
                    }
                }
            }
        }
    }

    // then we find every move for amphipods in their rooms
    for (size_t i = 0; i < rs.size(); ++i) {

        // Don't touch rooms which are filled with only correct amphipods.
        // This is actually a huge performance improvement, from 2 min to 750 ms.
        bool only_correct_amphipods_in_room = true;
        for (auto c : rs[i]) {
            if (c == '.') continue;

            if (c != ('A' + (char) i)) {

                only_correct_amphipods_in_room = false;
                break;
            }
        }
        if (only_correct_amphipods_in_room) continue;

        // get the first amphipod in the room
        size_t j;
        for (j = 0; j < rs[i].size(); ++j) {
            if (rs[i][j] != '.') {
                break;
            }
        }

        if (j != rs[i].size()) {

            size_t room_pos = 2 + (i * 2);
            for (size_t k = 0; k < h.size(); ++k) {
                if (h[k] == '.' && !is_room_entrance(k) && can_move_to(h, room_pos, k)) {

                    size_t move_cost = calc_move_cost(h, rs, k, i, j);
                    possible_moves.push_back({k, i, j, move_cost});
                }
            }
        }
    }

    // we apply every move we found and try recursivly if we get a solution with it
    for (const auto& m : possible_moves) {
        int new_cost = cost + m[3];
        if (lowest_cost != -1 && lowest_cost <= new_cost) continue;

        std::swap(rs[m[1]][m[2]], h[m[0]]);

        x = h + rs[0] + rs[1] + rs[2] + rs[3];
        if (mem.count(x) == 0 || mem[x] > new_cost) {
            auto [full_cost, solution] = bruteforce(h, rs, new_cost, lowest_cost);

            if (solution) {
                if (lowest_cost == -1) {
                    lowest_cost = full_cost;
                }

                lowest_cost = std::min(lowest_cost, full_cost);
            }
        }

        std::swap(rs[m[1]][m[2]], h[m[0]]);
    }

    // we return from the initial call, clear mem for later calls
    if (cost == 0) mem.clear();

    return std::make_tuple(lowest_cost, lowest_cost != -1);
}

answer solve_day23(input& in) {

    answer a;

    std::string hallway(in[1].size() - 2, '.');
    std::vector<std::string> rooms(4, "..");

    for (size_t i = 3; i <= 9; i += 2) {
        rooms[(i-3) / 2][0] = in[2][i];
        rooms[(i-3) / 2][1] = in[3][i];
    }

    auto [c, d] = bruteforce(hallway, rooms);
    a.part1 = std::to_string(c);


    // part 2
    std::vector<std::string> rooms2 = {
        rooms[0].substr(0, 1) + "DD" + rooms[0].substr(1, 1),
        rooms[1].substr(0, 1) + "CB" + rooms[1].substr(1, 1),
        rooms[2].substr(0, 1) + "BA" + rooms[2].substr(1, 1),
        rooms[3].substr(0, 1) + "AC" + rooms[3].substr(1, 1)
    };

    std::tie(c, d) = bruteforce(hallway, rooms2);
    a.part2 = std::to_string(c);

    return a;
}
