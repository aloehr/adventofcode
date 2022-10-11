#include <array>
#include <iostream>
#include <string>
#include <set>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"

enum class turn {LEFT, RIGHT};

typedef std::array<int, 2> vec2;

aoch::Result solve_day01(aoch::Input& in) {
    aoch::Result r;

    auto tokens = aoch::split(in[0]);

    std::vector<std::pair<turn, int>> instructions;
    instructions.reserve(tokens.size());

    for (const auto& t : tokens) {
        turn dir = turn::RIGHT;

        if (t[0] == 'L') {
            dir = turn::LEFT;
        }

        instructions.push_back({dir, std::stoi(t.substr(1))});
    }

    const std::vector<vec2> dirs {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    vec2 current_pos {0, 0};
    std::set<vec2> visited_locations = {{0, 0}};

    bool found_intersection = false;
    int dir_idx = 0;

    for (const auto& p : instructions) {
        if (p.first == turn::LEFT) {
            dir_idx--;

            if (dir_idx < 0) {
                dir_idx += 4;
            }
        } else {
            dir_idx++;
            dir_idx %= 4;
        }

        if (!found_intersection) {
            int n = p.second;

            while (n--) {
                current_pos[0] += dirs[dir_idx][0];
                current_pos[1] += dirs[dir_idx][1];

                if (!found_intersection && !visited_locations.insert(current_pos).second ) {
                    r.part2 = std::to_string(std::abs(current_pos[0]) + std::abs(current_pos[1]));
                    found_intersection = true;
                }
            }
        } else {
            current_pos[0] += dirs[dir_idx][0] * p.second;
            current_pos[1] += dirs[dir_idx][1] * p.second;
        }
    }

    r.part1 = std::to_string(std::abs(current_pos[0]) + std::abs(current_pos[1]));

    return r;
}
