#include <array>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "aoch/AOCSolutionTypes.hpp"


typedef std::array<int, 2> pos;

int num_one_bits(int n) {
    int c = 0;

    while (n) {
        c += n & 1;
        n >>= 1;
    }

    return c;
}

bool is_open_space(const pos& p, int fav_number) {
    int val = p[0]*p[0] + 3*p[0] + 2*p[0]*p[1] + p[1] + p[1]*p[1];
    val += fav_number;

    if (num_one_bits(val) % 2 == 0) return true;
    else return false;
}

std::array<int, 2> solve(const pos& goal, int fav_number, int goal2) {
    std::set<pos> visited;

    std::array<int, 2> res {-1, -1};
    int steps = 0;

    const pos init_state {1, 1};

    visited.insert(init_state);
    std::vector<pos> front = {init_state};
    std::vector<pos> next_front;

    std::array<pos, 4> possible_moves {{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};

    while (front.size() && (res[0] < 0 || res[1] >= 0)) {

        if (steps == goal2) {
            res[1] = visited.size();
        }

        for (auto& c : front) {

            if (c == goal) {
                res[0] = steps;
            }

            for (const auto& m : possible_moves) {
                c[0] += m[0];
                c[1] += m[1];

                if (c[0] < 0 || c[1] < 0) {
                    c[0] -= m[0];
                    c[1] -= m[1];
                    continue;
                }

                if (is_open_space(c, fav_number) && visited.count(c) == 0) {
                    visited.insert(c);
                    next_front.push_back(c);
                }

                c[0] -= m[0];
                c[1] -= m[1];
            }
        }

        steps++;

        front.clear();
        std::swap(front, next_front);
    }

    return res;
}

aoch::Result solve_day13(aoch::Input& in) {
    aoch::Result r;

    int fav_number = std::stoi(in[0]);
    pos goal = {31, 39};

    auto res = solve(goal, fav_number, 50);
    r.part1 = std::to_string(res[0]);
    r.part2 = std::to_string(res[1]);

    return r;
}
