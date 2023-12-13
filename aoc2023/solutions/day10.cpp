#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"


using Pos = std::array<int, 2>;

bool is_inbound(const std::vector<std::string>& map, const Pos& p) {
    if (p[0] < 0 || p[1] < 0) return false;
    if (p[1] >= static_cast<int>(map.size()) || p[0] >= static_cast<int>(map[p[1]].size())) return false;

    return true;
}

const std::vector<Pos>& get_relative_connection_points_for_pipe(char pipe) {
    static std::map<char, std::vector<Pos>> pipe_relative_connection_points = {
        {'|', {{ 0, -1}, { 0,  1}}},
        {'-', {{-1,  0}, { 1,  0}}},
        {'L', {{ 0, -1}, { 1,  0}}},
        {'J', {{ 0, -1}, {-1,  0}}},
        {'7', {{ 0,  1}, {-1,  0}}},
        {'F', {{ 0,  1}, { 1,  0}}},
        {'.', {}},
    };

    return pipe_relative_connection_points[pipe];
}

bool is_connected(char pipe, const Pos& x1, const Pos& x2) {

    const auto& pipe_dirs = get_relative_connection_points_for_pipe(pipe);

    if (pipe_dirs.size() == 0) return false;

    for (const auto& pipe_dir : pipe_dirs) {
        if (x1[0] + pipe_dir[0] == x2[0] && x1[1] + pipe_dir[1] == x2[1]) {
            return true;
        }
    }

    return false;
}

bool traverse(const std::vector<std::string>& map, Pos cur_pos, Pos from, std::vector<Pos>& loop_positions) {
    char pipe = map[cur_pos[1]][cur_pos[0]];

    while (true) {
        loop_positions.push_back(cur_pos);

        const auto& pipe_dirs = get_relative_connection_points_for_pipe(pipe);

        Pos next_pos;

        for (const auto& pipe_dir : pipe_dirs) {
            Pos connection_point = {cur_pos[0] + pipe_dir[0], cur_pos[1] + pipe_dir[1]};

            if (connection_point == from) continue;

            pipe = map[connection_point[1]][connection_point[0]];

            if (pipe == 'S') {
                return true;
            } else if (is_connected(pipe, connection_point, cur_pos)) {
                next_pos = connection_point;
            } else {
                return false;
            }
        }

        from = cur_pos;
        cur_pos = next_pos;
    }
}

aoch::Result solve_day10(aoch::Input& in) {
    aoch::Result a;

    // find start pos
    Pos start;
    for (size_t y = 0; y < in.size(); ++y) {
        for (size_t x = 0; x < in[y].size(); ++x) {
            if (in[y][x] == 'S') start = {static_cast<int>(x), static_cast<int>(y)};
        }
    }

    // find the loop from start pos
    const std::vector<Pos> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    std::vector<Pos> loop_positions;

    for (const auto& dir : dirs) {
        Pos cur_pos = {start[0] + dir[0], start[1] + dir[1]};

        if (is_inbound(in, cur_pos)) {
            char pipe = in[cur_pos[1]][cur_pos[0]];

            if (is_connected(pipe, cur_pos, start)) {
                Pos from = start;
                std::vector<Pos> tmp {start};
                if (traverse(in, cur_pos, from, tmp)) {
                    loop_positions = std::move(tmp);
                    break;
                }
            }
        }
    }

    a.part1 = std::to_string(loop_positions.size() / 2);

    // part 2
    // using the shoelace formula & Pick's theorem to calculate the enclosed tiles.

    // we add start to the of the vector end even tho it is already in it at the start to make the calculation
    // easier, elsewise we would have to do another addition step after the for loop
    loop_positions.push_back(start);

    float pipe_enclosed_area = 0;
    for (size_t i = 0; i < loop_positions.size() - 1; ++i) {
        pipe_enclosed_area += loop_positions[i][0] * loop_positions[i+1][1] - loop_positions[i][1] * loop_positions[i+1][0];
    }
    pipe_enclosed_area = std::abs(pipe_enclosed_area / 2.0);

    // minus 1 from the size because start pos is accounted twice for
    float pipe_points_halfed = (loop_positions.size() - 1) / 2.0;
    int enclosed_tiles_count = pipe_enclosed_area - pipe_points_halfed + 1.0;

    a.part2 = std::to_string(enclosed_tiles_count);

    return a;
}
