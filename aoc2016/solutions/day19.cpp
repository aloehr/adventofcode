#include <array>
#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"


int elves1(int num_elves) {
    std::vector<int> circle(num_elves);

    for (size_t i = 0; i < circle.size() - 1; ++i) {
        circle[i] = i + 1;
    }

    circle.back() = 0;


    int cur = 0;

    while (circle[cur] != cur) {
        int next = circle[circle[cur]];
        circle[cur] = next;

        cur = next;
    }

    return cur + 1;
}

int elves2(int num_elves) {
    std::vector<std::array<int, 2>> circle(num_elves);

    for (size_t i = 1; i < circle.size() - 1; ++i) {
        circle[i][0] = i + 1;
        circle[i][1] = i - 1;
    }

    int last_idx = circle.size() - 1;
    circle[0][0] = 1;
    circle[0][1] = last_idx;
    circle.back()[0] = 0;
    circle.back()[1] = last_idx - 1;

    int cur = 0;
    int other_side_left = circle.size() / 2;
    int other_side_right = (circle.size() + 1) / 2;

    while (cur != circle[cur][0]) {
        if (other_side_left == other_side_right) {
            other_side_left = circle[other_side_left][1];
            other_side_right = circle[other_side_right][0];

            circle[other_side_left][0] = other_side_right;
            circle[other_side_right][1] = other_side_left;
        } else {
            int before_left = circle[other_side_left][1];
            circle[before_left][0] = other_side_right;
            circle[other_side_right][1] = before_left;
            other_side_left = other_side_right;
        }

        cur = circle[cur][0];
        other_side_left = circle[other_side_left][0];
        other_side_right = circle[other_side_right][0];
    }

    return cur + 1;
}

aoch::Result solve_day19(aoch::Input& in) {
    aoch::Result r;

    int num_elves = std::stoi(in[0]);

    r.part1 = std::to_string(elves1(num_elves));
    r.part2 = std::to_string(elves2(num_elves));

    return r;
}
