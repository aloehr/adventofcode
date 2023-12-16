#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/math.hpp"


using PosT = std::array<size_t, 2>;
using DirT = std::array<int, 2>;

enum Dir {TOP, LEFT, RIGHT, BOTTOM};

bool is_inbound(const std::vector<std::string>& map, const PosT& p, const DirT& d) {
    if (p[0] == 0 && d[0] < 0) return false;
    if (p[1] == 0 && d[1] < 0) return false;
    if (p[0] + d[0] >= map[0].size()) return false;
    if (p[1] + d[1] >= map.size()) return false;

    return true;
}

void traverse(const std::vector<std::string>& map, PosT cur_pos, Dir d, std::vector<std::vector<char>>& memo) {
    static std::map<int, std::vector<DirT>> next_dirs{
        {'.' | (Dir::TOP << 8),    {{ 0, -1}}},
        {'.' | (Dir::LEFT << 8),   {{-1,  0}}},
        {'.' | (Dir::RIGHT << 8),  {{ 1,  0}}},
        {'.' | (Dir::BOTTOM << 8), {{ 0,  1}}},
        {'|' | (Dir::TOP << 8),    {{ 0, -1}}},
        {'|' | (Dir::LEFT << 8),   {{ 0, -1}, { 0,  1}}},
        {'|' | (Dir::RIGHT << 8),  {{ 0, -1}, { 0,  1}}},
        {'|' | (Dir::BOTTOM << 8), {{ 0,  1}}},
        {'-' | (Dir::TOP << 8),    {{-1,  0}, { 1,  0}}},
        {'-' | (Dir::LEFT << 8),   {{-1,  0}}},
        {'-' | (Dir::RIGHT << 8),  {{ 1,  0}}},
        {'-' | (Dir::BOTTOM << 8), {{-1,  0}, { 1,  0}}},
        {'/' | (Dir::TOP << 8),    {{ 1,  0}}},
        {'/' | (Dir::LEFT << 8),   {{ 0,  1}}},
        {'/' | (Dir::RIGHT << 8),  {{ 0, -1}}},
        {'/' | (Dir::BOTTOM << 8), {{-1,  0}}},
        {'\\' | (Dir::TOP << 8),    {{-1,  0}}},
        {'\\' | (Dir::LEFT << 8),   {{ 0, -1}}},
        {'\\' | (Dir::RIGHT << 8),  {{ 0,  1}}},
        {'\\' | (Dir::BOTTOM << 8), {{ 1,  0}}},
    };

    static std::map<DirT, Dir> relative_pos_to_dir{
        {{ 0,  1}, Dir::BOTTOM},
        {{ 0, -1}, Dir::TOP},
        {{ 1,  0}, Dir::RIGHT},
        {{-1,  0}, Dir::LEFT},
    };

    // memo is a bitset with 4 bits per tile. (one bit for every light direction).
    if (memo[cur_pos[1]][cur_pos[0]] & (1 << d)) {
        return;
    }
    memo[cur_pos[1]][cur_pos[0]] |= 1 << d;

    char cur_tile = map[cur_pos[1]][cur_pos[0]];

    for (const auto& next_dir : next_dirs[cur_tile | (d << 8)]) {
        if (!is_inbound(map, cur_pos, next_dir)) continue;

        PosT next_pos = {cur_pos[0] + next_dir[0], cur_pos[1] + next_dir[1]};
        traverse(map, next_pos, relative_pos_to_dir[next_dir], memo);
    }

    return;
}

unsigned int start_traverse(const std::vector<std::string>& map, PosT cur_pos, Dir d) {
    std::vector<std::vector<char>> memo(map.size(), std::vector<char>(map[0].size(), 0));

    traverse(map, cur_pos, d, memo);

    unsigned int sum = 0;
    for (size_t i = 0; i < memo.size(); ++i) {
        for (size_t j = 0; j < memo[i].size(); ++j) {
            sum += memo[i][j] != 0;
        }
    }

    return sum;
}

aoch::Result solve_day16(aoch::Input& in) {
    aoch::Result a;

    a.part1 = std::to_string(start_traverse(in, {0, 0}, Dir::RIGHT));


    // part 2
    unsigned int best = 0;

    // top row
    for (size_t i = 1; i < in[0].size(); ++i) {
        best = std::max(start_traverse(in, {i, 0}, Dir::BOTTOM), best);
    }

    // bottomw row
    for (size_t i = 0; i < in[0].size(); ++i) {
        best = std::max(start_traverse(in, {i, in.size() - 1}, Dir::TOP), best);
    }

    // left & right sides minus the corners
    for (size_t i = 1; i < in.size() - 1; ++i) {
        best = std::max(start_traverse(in, {0, i}, Dir::RIGHT), best);
        best = std::max(start_traverse(in, {in[0].size() - 1, i}, Dir::LEFT), best);
    }

    a.part2 = std::to_string(best);

    return a;
}
