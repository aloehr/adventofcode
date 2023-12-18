#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


enum Dir {NORTH, EAST, SOUTH, WEST};

using PosT = std::array<long long, 2>;

long long area(const std::vector<PosT>& points) {
    // using shoelace formula and Pick's theorem to calculate the area
    long long double_area = 0;
    long long point_count = 0;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        point_count += std::abs(points[i][0] - points[i+1][0]) + std::abs(points[i][1] - points[i+1][1]);
        double_area += points[i][0] * points[i+1][1] - points[i][1] * points[i+1][0];
    }

    point_count += std::abs(points[points.size()-1][0] - points[0][0]) + std::abs(points[points.size()-1][1] - points[0][1]);
    double_area += points[points.size()-1][0] * points[0][1] - points[points.size()-1][1] * points[0][0];

    return double_area/2 + point_count/2 + 1;
}

aoch::Result solve_day18(aoch::Input& in) {
    aoch::Result a;

    std::map<char, Dir> char_to_dir {
        {'U', Dir::NORTH},
        {'R', Dir::EAST},
        {'D', Dir::SOUTH},
        {'L', Dir::WEST},
    };

    std::map<char, char> char_to_char {
        {'0', 'R'},
        {'1', 'D'},
        {'2', 'L'},
        {'3', 'U'},
    };

    const std::vector<PosT> dir_to_relative_dir {
        { 0, -1},
        { 1,  0},
        { 0,  1},
        {-1,  0},
    };

    PosT cur_pos {0, 0};
    PosT cur_pos2 {0, 0};
    std::vector<PosT> trench_positions;
    std::vector<PosT> trench_positions2;

    for (const auto& l : in) {
        auto tokens = aoch::split(l);
        PosT d = dir_to_relative_dir[char_to_dir[tokens[0][0]]];
        int trench_segment_length = std::stoi(tokens[1]);

        cur_pos[0] += trench_segment_length * d[0];
        cur_pos[1] += trench_segment_length * d[1];
        trench_positions.push_back(cur_pos);

        char dir_num = tokens[2][tokens[2].size() - 2];
        d = dir_to_relative_dir[char_to_dir[char_to_char[dir_num]]];
        trench_segment_length = std::stoi(tokens[2].substr(2, tokens[2].size() - 4), nullptr, 16);

        cur_pos2[0] += trench_segment_length * d[0];
        cur_pos2[1] += trench_segment_length * d[1];
        trench_positions2.push_back(cur_pos2);
    }

    a.part1 = std::to_string(area(trench_positions));
    a.part2 = std::to_string(area(trench_positions2));

    return a;
}
