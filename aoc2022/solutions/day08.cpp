#include <array>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>


int calc_scenic_score(const std::vector<std::string> &tree_map, std::array<size_t, 2> pos) {
    static const std::array< std::array<int, 2>, 4> dirs {{
        { 0,  1},
        { 0, -1},
        { 1,  0},
        {-1,  0},
    }};

    const size_t h = tree_map.size();
    const size_t w = tree_map[0].size();

    const char tree_height = tree_map[pos[1]][pos[0]];
    int scenic_score = 1;

    for (const auto &d : dirs) {
        int distance = 0;
        auto cur_pos = pos;

        while (cur_pos[0] + d[0] < w && cur_pos[1] + d[1] < h) {
            distance++;
            cur_pos = {cur_pos[0] + d[0], cur_pos[1] + d[1]};

            if (tree_map[cur_pos[1]][cur_pos[0]] >= tree_height) {
                break;
            }
        }

        scenic_score *= distance;
    }

    return scenic_score;
}

aoch::Result solve_day08(aoch::Input& in) {
    aoch::Result a;

    size_t h = in.size();
    size_t w = in[0].size();

    std::vector< std::vector<int> > visible(h, std::vector<int>(w));
    char max_height_seen = 0;

    auto inner_loop_logic = [&max_height_seen, &in, &visible] (const auto x, const auto y) {
        if (in[y][x] > max_height_seen) {
            visible[y][x] = 1;
        }

        max_height_seen = std::max(max_height_seen, in[y][x]);
    };

    for (size_t y = 0; y < h; ++y) {
        max_height_seen = 0;
        for (size_t x = 0; x < w; ++x) {
            inner_loop_logic(x, y);
        }
    }

    for (size_t y = 0; y < h; ++y) {
        max_height_seen = 0;
        for (size_t x = w-1; x < w; --x) {
            inner_loop_logic(x, y);
        }
    }

    for (size_t x = 0; x < w; ++x) {
        max_height_seen = 0;
        for (size_t y = 0; y < h; ++y) {
            inner_loop_logic(x, y);
        }
    }

    for (size_t x = 0; x < w; ++x) {
        max_height_seen = 0;
        for (size_t y = h-1; y < h; --y) {
            inner_loop_logic(x, y);
        }
    }

    int visible_count = std::accumulate(visible.cbegin(), visible.cend(), 0, [] (auto acc, const auto &row) {
        return std::accumulate(row.cbegin(), row.cend(), acc);
    });

    a.part1 = std::to_string(visible_count);


    // part 2
    int max_scenic_score = 0;
    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            max_scenic_score = std::max(max_scenic_score, calc_scenic_score(in, {x, y}));
        }
    }

    a.part2 = std::to_string(max_scenic_score);

    return a;
}
