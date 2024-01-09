#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_set>
#include <tuple>
#include <vector>
#include <queue>
#include <sstream>

#include <aoch/AOCSolutionTypes.hpp>


using PosT = std::array<int, 2>;
using MapT = std::vector<std::string>;
using QueueItemT = std::pair<PosT, unsigned int>;

static bool is_inbound(const MapT& map, const PosT& pos) {
    if (pos[0] < 0 || pos[1] < 0) return false;
    if (static_cast<size_t>(pos[1]) >= map.size() || static_cast<size_t>(pos[0]) >= map[pos[1]].size()) return false;
    return true;
}

std::vector<unsigned int> bfs(const MapT& map, const PosT& start, bool odd_tiles_count) {
    std::vector<unsigned int> ret;
    std::unordered_set<size_t> seen_states;
    std::vector<QueueItemT> queue;
    std::vector<QueueItemT> next_queue;
    queue.push_back({start, 0});
    unsigned int reachable = 0;

    while (queue.size() || next_queue.size()) {

        if (!queue.size()) {
            std::swap(queue, next_queue);
            ret.push_back(reachable);
        }

        PosT cur_pos;
        unsigned int cur_steps;
        std::tie(cur_pos, cur_steps) = queue.back();
        queue.pop_back();

        size_t hash = ((0ul | (0u | cur_pos[0])) << 32) | (0u | cur_pos[1]);
        if (seen_states.count(hash)) {
            continue;
        } else {
            seen_states.insert(hash);
        }

        if (cur_steps % 2 == odd_tiles_count) {
            reachable++;
        }

        PosT next_pos = {cur_pos[0] + 1, cur_pos[1]};
        if (is_inbound(map, next_pos) && map[next_pos[1]][next_pos[0]] == '.') {
            next_queue.push_back({next_pos, cur_steps + 1});
        }

        next_pos = {cur_pos[0] - 1, cur_pos[1]};
        if (is_inbound(map, next_pos) && map[next_pos[1]][next_pos[0]] == '.') {
            next_queue.push_back({next_pos, cur_steps + 1});
        }

        next_pos = {cur_pos[0], cur_pos[1] + 1};
        if (is_inbound(map, next_pos) && map[next_pos[1]][next_pos[0]] == '.') {
            next_queue.push_back({next_pos, cur_steps + 1});
        }

        next_pos = {cur_pos[0], cur_pos[1] - 1};
        if (is_inbound(map, next_pos) && map[next_pos[1]][next_pos[0]] == '.') {
            next_queue.push_back({next_pos, cur_steps + 1});
        }
    }

    return ret;
}

unsigned long long calc_reachable_plots_infinite_grid(const std::vector<std::string>& map, unsigned long max_steps) {
    unsigned long long total = 0;

    // we assume that the map is a square
    int map_size = map.size();

    assert(map_size % 2 == 1 && map_size % 2 == 1);

    PosT start = {map_size / 2, map_size / 2};

    enum {
        BOTTOM_LEFT, BOTTOM_MIDDLE, BOTTOM_RIGHT,
        MIDDLE_LEFT, MIDDLE_RIGHT,
        TOP_LEFT, TOP_MIDDLE, TOP_RIGHT
    };

    const unsigned int num_corner_points = TOP_RIGHT + 1;

    enum {EVEN, ODD};

    std::vector<PosT> tile_starts {
        {0, map_size - 1}, {map_size / 2, map_size - 1}, {map_size - 1, map_size - 1},
        {0, map_size / 2}, {map_size - 1, map_size / 2},
        {0, 0}, {map_size / 2, 0}, {map_size - 1, 0},
    };

    std::vector<std::vector<std::vector<unsigned int>>> reachables(TOP_RIGHT + 1, std::vector<std::vector<unsigned int>>(2));

    auto reachable_start = bfs(map, start, max_steps%2);

    total += reachable_start[std::min(reachable_start.size() - 1, max_steps)];
    if (max_steps <= static_cast<unsigned long>(map_size / 2)) return total;

    for (size_t i = 0; i < num_corner_points; ++i) {
        reachables[i][EVEN] = bfs(map, tile_starts[i], false);
        reachables[i][ODD] = bfs(map, tile_starts[i], true);
    }

    unsigned int cur_steps = map_size / 2 + 1;

    // cross
    unsigned int steps_left = max_steps - cur_steps;
    unsigned int num_packs_filled = (steps_left - (reachables[BOTTOM_MIDDLE][0].size() - 1 - map_size)) / (2 * map_size);
    if (reachables[BOTTOM_LEFT][0].size() - 1 - map_size > steps_left) {
        num_packs_filled = 0;
    }

    total += num_packs_filled * (reachables[MIDDLE_LEFT][EVEN].back() + reachables[MIDDLE_LEFT][ODD].back());
    total += num_packs_filled * (reachables[MIDDLE_RIGHT][EVEN].back() + reachables[MIDDLE_RIGHT][ODD].back());
    total += num_packs_filled * (reachables[TOP_MIDDLE][EVEN].back() + reachables[TOP_MIDDLE][ODD].back());
    total += num_packs_filled * (reachables[BOTTOM_MIDDLE][EVEN].back() + reachables[BOTTOM_MIDDLE][ODD].back());

    unsigned int cur_cur_steps = cur_steps + num_packs_filled * 2 * map_size;
    while (cur_cur_steps <= max_steps) {
        bool is_odd = max_steps % 2 != (cur_cur_steps) % 2;
        unsigned long steps_left = max_steps - cur_cur_steps;
        total += reachables[MIDDLE_RIGHT][is_odd][std::min(reachables[MIDDLE_RIGHT][is_odd].size() - 1, steps_left)];
        total += reachables[MIDDLE_LEFT][is_odd][std::min(reachables[MIDDLE_LEFT][is_odd].size() - 1, steps_left)];
        total += reachables[TOP_MIDDLE][is_odd][std::min(reachables[TOP_MIDDLE][is_odd].size() - 1, steps_left)];
        total += reachables[BOTTOM_MIDDLE][is_odd][std::min(reachables[BOTTOM_MIDDLE][is_odd].size() - 1, steps_left)];
        cur_cur_steps += map_size;
    }

    // corners
    cur_steps = 2 * (map_size / 2 + 1);

    while (cur_steps <= max_steps) {
        steps_left = max_steps - cur_steps;
        num_packs_filled = (steps_left - (reachables[BOTTOM_LEFT][0].size() - 1 - map_size)) / (2 * map_size);
        if (reachables[BOTTOM_LEFT][0].size() - 1 - map_size > steps_left) {
            num_packs_filled = 0;
        }

        total += num_packs_filled * (reachables[BOTTOM_RIGHT][EVEN].back() + reachables[BOTTOM_RIGHT][ODD].back());
        total += num_packs_filled * (reachables[BOTTOM_LEFT][EVEN].back() + reachables[BOTTOM_LEFT][ODD].back());
        total += num_packs_filled * (reachables[TOP_RIGHT][EVEN].back() + reachables[TOP_RIGHT][ODD].back());
        total += num_packs_filled * (reachables[TOP_LEFT][EVEN].back() + reachables[TOP_LEFT][ODD].back());

        // BOTTOM_RIGHT
        cur_cur_steps = cur_steps + num_packs_filled * 2 * map_size;

        while (cur_cur_steps <= max_steps) {
            bool is_odd = max_steps % 2 != (cur_cur_steps) % 2;
            unsigned long steps_left = max_steps - cur_cur_steps;
            total += reachables[BOTTOM_RIGHT][is_odd][std::min(reachables[BOTTOM_RIGHT][is_odd].size() - 1, steps_left)];
            total += reachables[BOTTOM_LEFT][is_odd][std::min(reachables[BOTTOM_LEFT][is_odd].size() - 1, steps_left)];
            total += reachables[TOP_RIGHT][is_odd][std::min(reachables[TOP_RIGHT][is_odd].size() - 1, steps_left)];
            total += reachables[TOP_LEFT][is_odd][std::min(reachables[TOP_LEFT][is_odd].size() - 1, steps_left)];
            cur_cur_steps += map_size;
        }

        cur_steps += map_size;
    }

    return total;
}

aoch::Result solve_day21(aoch::Input& in) {
    aoch::Result a;

    // parsing
    PosT start;
    bool found = false;
    for (size_t y = 0; y < in.size() && !found; ++y) {
        for (size_t x = 0; x < in[y].size() && !found; ++x) {
            if (in[y][x] == 'S') {
                start = {static_cast<int>(x), static_cast<int>(y)};
                in[y][x] = '.';
                found = true;
            }
        }
    }

    a.part1 = std::to_string(bfs(in, start, 64%2)[64]);
    a.part2 = std::to_string(calc_reachable_plots_infinite_grid(in, 26501365));

    return a;
}
