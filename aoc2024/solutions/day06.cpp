#include <array>
#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>
#include <cstring>

#include <aoch/AOCSolutionTypes.hpp>


inline bool is_inbound(const std::pair<int, int> p, int max_x, int max_y) {
    return p.first >= 0 && p.first <= max_x && p.second >= 0 && p.second <= max_y;
}

aoch::Result solve_day06(aoch::Input& in) {
    aoch::Result a;
//     in = {
// "....#.....",
// ".........#",
// "..........",
// "..#.......",
// ".......#..",
// "..........",
// ".#..^.....",
// "........#.",
// "#.........",
// "......#...",
//     };

    const int max_y = in[0].size() - 1;
    const int max_x = in.size() - 1;

    std::unordered_set<int> visited;

    std::pair<int, int> guard_start_pos;
    size_t guard_dir_idx = 0;

    const std::array<std::array<int, 2>, 4> guard_dirs = {{{0,-1}, {1,0}, {0,1}, {-1,0}}};

    for (size_t i = 0; i < in.size(); ++i) {
        for (size_t j = 0; j < in.size(); ++j) {
            if (in[i][j] == '^') {
                guard_start_pos = {j, i};
            }
        }
    }

    std::pair<int, int> guard_pos = guard_start_pos;

    while (is_inbound(guard_pos, max_x, max_y)) {
        visited.insert(guard_pos.first << 8 | guard_pos.second);

        for (size_t i = 0; i < 4; ++i) {
            std::pair<int, int> next_pos = {
                guard_pos.first  + guard_dirs[(guard_dir_idx + i) % 4][0],
                guard_pos.second + guard_dirs[(guard_dir_idx + i) % 4][1]
            };

            if (is_inbound(next_pos, max_x, max_y) && in[next_pos.second][next_pos.first] == '#') {
                continue;
            }

            guard_dir_idx = (guard_dir_idx + i) % 4;
            guard_pos = next_pos;

            break;
        }
    }

    a.part1 = std::to_string(visited.size());


    // part 2
    std::array<bool, (1 << 18) - (1 << 16)> v{};

    auto has_loop = [&v, &in, &guard_start_pos] () {

        const std::array<std::array<int, 2>, 4> guard_dirs = {{{0,-1}, {1,0}, {0,1}, {-1,0}}};
        v.fill(0);
        std::pair<int, int> guard_pos = guard_start_pos;
        int guard_dir_idx = 0;

        auto hash = [] (const std::pair<int, int>& pos, int dir) {
            return (pos.first << 10) | (pos.second << 2) | dir;
        };

        bool loop_detected = false;

        bool on_map = true;
        while (on_map) {

            int hashed_guard_state = hash(guard_pos, guard_dir_idx);

            if (v[hashed_guard_state]) {
                loop_detected = true;
                break;
            }

            v[hashed_guard_state] = true;

            while (true) {

                bool out_of_bound = false;
                switch (guard_dir_idx) {
                    case 0:
                        out_of_bound = guard_pos.second == 0;
                        break;
                    case 1:
                        out_of_bound = guard_pos.first == static_cast<int>(in[0].size() - 1);
                        break;
                    case 2:
                        out_of_bound = guard_pos.second == static_cast<int>(in.size() - 1);
                        break;
                    case 3:
                        out_of_bound = guard_pos.first == 0;
                        break;
                }

                if (out_of_bound) {
                    on_map = false;
                    break;
                }


                int nextx = guard_pos.first  + guard_dirs[guard_dir_idx][0];
                int nexty = guard_pos.second + guard_dirs[guard_dir_idx][1];

                if (in[nexty][nextx] == '#') {
                    break;
                }

                guard_pos = {nextx, nexty};

            }

            for (size_t i = 1; i < 4; ++i) {
                bool out_of_bound = false;
                switch ((guard_dir_idx + i) % 4) {
                    case 0:
                        out_of_bound = guard_pos.second == 0;
                        break;
                    case 1:
                        out_of_bound = guard_pos.first == static_cast<int>(in[0].size() - 1);
                        break;
                    case 2:
                        out_of_bound = guard_pos.second == static_cast<int>(in.size() - 1);
                        break;
                    case 3:
                        out_of_bound = guard_pos.first == 0;
                        break;
                }

                if (out_of_bound) {
                    on_map = false;
                    break;
                }

                std::pair<int, int> next_pos = {
                    guard_pos.first  + guard_dirs[(guard_dir_idx + i) % 4][0],
                    guard_pos.second + guard_dirs[(guard_dir_idx + i) % 4][1]
                };

                if (in[next_pos.second][next_pos.first] == '#') {
                    continue;
                }

                guard_dir_idx = (guard_dir_idx + i) % 4;
                guard_pos = next_pos;
                break;
            }
        }

        return loop_detected;
    };

    visited.erase(guard_start_pos.first << 8 | guard_start_pos.second);

    int count = 0;
    for (int n : visited) {
        int i = n & ((1 << 8) - 1);
        int j = n >> 8;

        in[i][j] = '#';
        count += has_loop();
        in[i][j] = '.';
    }

    a.part2 = std::to_string(count);
    return a;
}
