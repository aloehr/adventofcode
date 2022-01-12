#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <cassert>
#include <cmath>

#include "../aoc.hpp"


void follow_instruction(char c, int units, int& f_dir, int& NS, int& EW) {

    const char dir[4] = {'N', 'E', 'S', 'W'};

    switch (c) {
        case 'N':
            NS += units;
            break;
        case 'S':
            NS -= units;
            break;
        case 'E':
            EW += units;
            break;
        case 'W':
            EW -= units;
            break;
        case 'F':
            follow_instruction(dir[f_dir], units, f_dir, NS, EW); 
            break;
        case 'R':
            f_dir = (f_dir + units / 90) % 4;
            break;
        case 'L':
            f_dir = (f_dir + 4 - units / 90 % 4) % 4;
            break;

    }
}

void rotate(int units, int& WP_NS, int& WP_EW, bool ccw = false) {
    int tmp = WP_NS;

    switch (units / 90 % 4) {
        case 0: 
            break;
        case 1:
            WP_NS = ccw ? WP_EW : -WP_EW;
            WP_EW = ccw ? -tmp : tmp;
            break;
        case 2:
            WP_NS = -WP_NS;
            WP_EW = -WP_EW;
            break;
        case 3:
            WP_NS = ccw ? -WP_EW : WP_EW;
            WP_EW = ccw ? tmp : -tmp;
            break;
    }
}

void follow_instruction2(char c, int units, int& S_NS, int& S_EW, int& WP_NS, int& WP_EW) {

    switch (c) {
        case 'N':
            WP_NS += units;
            break;
        case 'S':
            WP_NS -= units;
            break;
        case 'E':
            WP_EW += units;
            break;
        case 'W':
            WP_EW -= units;
            break;
        case 'F':
            S_NS += units * WP_NS;
            S_EW += units * WP_EW;
            break;
        case 'R':
            rotate(units, WP_NS, WP_EW);
            break;
        case 'L':
            rotate(units, WP_NS, WP_EW, true);
            break;
    }
}

answer solve_day12(input& in) {

    answer a;
    in.pop_back();
    std::vector<std::pair<char, int>> data;
    
    for (const auto& l : in) {
        data.push_back(std::make_pair(l[0], std::stoi(l.substr(1))));
    }

    for (auto x : data) if (x.first == 'L' || x.first == 'R') assert(x.second % 90 == 0);

    int forward_dir = 1;

    int NS = 0;
    int EW = 0;

    for (const auto& p : data) {
        follow_instruction(p.first, p.second, forward_dir, NS, EW);
    }

    int S_NS = 0;
    int S_EW = 0;
    int WP_NS = 1;
    int WP_EW = 10;

    for (const auto& p : data) {
        follow_instruction2(p.first, p.second, S_NS, S_EW, WP_NS, WP_EW);
    }
    
    a.part1 = std::to_string(std::abs(NS) + std::abs(EW));
    a.part2 = std::to_string(std::abs(S_NS) + std::abs(S_EW));

    return a;
}
