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

static void convert(std::vector<std::string>& map) {
    for (size_t i = 0; i < map.size(); ++i) {
        for (size_t j = 0; j < map[i].size(); ++j) {
            switch (map[i][j]) {
                case '/':
                    map[i][j] = 0;
                    break;
                case '-':
                    map[i][j] = 1;
                    break;
                case '|':
                    map[i][j] = 2;
                    break;
                case '\\':
                    map[i][j] = 3;
                    break;
                case '.':
                    map[i][j] = 4;
                    break;
            }
        }
    }

    return;
}

bool is_inbound(const std::vector<std::string>& map, const PosT& p, const DirT& d) {
    if (p[0] == 0 && d[0] < 0) return false;
    if (p[1] == 0 && d[1] < 0) return false;
    if (p[0] + d[0] >= map[0].size()) return false;
    if (p[1] + d[1] >= map.size()) return false;

    return true;
}

unsigned int start_traverse(const std::vector<std::string>& map, PosT start_pos, Dir start_d) {
    // tells the relative next direction based on an index based on current tile type and current light direction
    static std::vector<std::vector<DirT>> next_dirs {
        // '/'
        {{ 1,  0}}, // T
        {{ 0,  1}}, // L
        {{ 0, -1}}, // R
        {{-1,  0}}, // B

        // '-'
        {{-1,  0}, { 1,  0}},
        {{-1,  0}},
        {{ 1,  0}},
        {{-1,  0}, { 1,  0}},

        // '|'
        {{ 0, -1}},
        {{ 0, -1}, { 0,  1}},
        {{ 0, -1}, { 0,  1}},
        {{ 0,  1}},

        // '\\'
        {{-1,  0}},
        {{ 0, -1}},
        {{ 0,  1}},
        {{ 1,  0}},

        // '.'
        {{ 0, -1}},
        {{-1,  0}},
        {{ 1,  0}},
        {{ 0,  1}},
    };

    // symmetry masks for the bitfield based on an index based on current tile type and current light direction
    static std::vector<unsigned int> symmetry_masks {
        // '/'
        0x3, 0x3, 0xC, 0xC, // order T L R B

        // '-'
        0xF, 0xF, 0xF, 0xF,

        // '|'
        0xF, 0xF, 0xF, 0xF,

        // '\\'
        0x5, 0xA, 0x5, 0xA,

        // '.'
        0x9, 0x6, 0x6, 0x9,
    };

    static std::map<DirT, Dir> relative_pos_to_dir{
        {{ 0,  1}, Dir::BOTTOM},
        {{ 0, -1}, Dir::TOP},
        {{ 1,  0}, Dir::RIGHT},
        {{-1,  0}, Dir::LEFT},
    };

    std::vector<std::vector<char>> memo(map.size(), std::vector<char>(map[0].size(), 0));
    std::vector<std::pair<PosT, Dir>> queue {std::make_pair(start_pos, start_d)};

    while (queue.size()) {
        auto cur = std::move(queue.back());
        queue.pop_back();

        while (true) {
            // memo is a bitset with 4 bits per tile. (one bit for every light direction).
            if (memo[cur.first[1]][cur.first[0]] & (1 << cur.second)) {
                break;
            }

            char cur_tile = map[cur.first[1]][cur.first[0]];

            // index for current tile type and current light direction
            unsigned int idx = (cur_tile << 2) | cur.second;

            // there are symmetries depending on direction you enter the tile and the tile type.
            memo[cur.first[1]][cur.first[0]] |= symmetry_masks[idx];

            auto& next_dirs_ref = next_dirs[idx];

            // if we split the beam, put one of the two beams on the  queue while continue following the other one;
            if (next_dirs_ref.size() > 1 && is_inbound(map, cur.first, next_dirs_ref[1])) {
                PosT next_pos = {cur.first[0] + next_dirs_ref[1][0], cur.first[1] + next_dirs_ref[1][1]};
                queue.push_back(std::make_pair(next_pos, relative_pos_to_dir[next_dirs_ref[1]]));
            }

            if (!is_inbound(map, cur.first, next_dirs_ref[0])) {
                break;
            }

            cur.first[0] += next_dirs_ref[0][0];
            cur.first[1] += next_dirs_ref[0][1];
            cur.second = relative_pos_to_dir[next_dirs_ref[0]];
        }
    }

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

    convert(in);

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
