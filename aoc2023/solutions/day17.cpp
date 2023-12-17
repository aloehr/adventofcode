#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"


enum Dir {NORTH, EAST, SOUTH, WEST};

using PosT = std::array<size_t, 2>;
using DirT = std::array<int, 2>;

struct State {
    PosT pos;
    Dir d;
    unsigned int straight_line_moves_count = 0;
    int cur_heat_loss = 0;

    bool operator<(const State& o) const {
        return this->cur_heat_loss > o.cur_heat_loss;
    }
};

static bool is_inbound(const std::vector<std::string>& map, const PosT& p, const DirT& d) {
    if (p[0] + d[0] < 0 || p[1] + d[1] < 0) return false;
    if (p[1] + d[1] >= map.size() || p[0] + d[0] >= map[0].size()) return false;

    return true;
}

int dijkstra(
        const std::vector<std::string>& map,
        PosT start, PosT dest,
        unsigned int max_straight_line_moves = 3,
        unsigned int min_straight_line_moves = 0
) {
    static const std::vector<DirT> dir_to_relative_dir {
        { 0, -1},
        { 1,  0},
        { 0,  1},
        {-1,  0},
    };

    static const std::vector<Dir> move_left {
        Dir::WEST,
        Dir::NORTH,
        Dir::EAST,
        Dir::SOUTH,
    };

    static const std::vector<Dir> move_right {
        Dir::EAST,
        Dir::SOUTH,
        Dir::WEST,
        Dir::NORTH,
    };

    // using a heap as priority queue
    std::vector<State> front {
        {{0, 0}, Dir::EAST, 0, 0},
        {{0, 0}, Dir::SOUTH, 0, 0},
    };
    std::make_heap(front.begin(), front.end());

    // calc required amount of bits for the straight line moves count
    unsigned int needed_bits = 0;
    while (((1u << needed_bits) - 1) < max_straight_line_moves) needed_bits++;

    std::vector<std::vector<std::vector<int>>> heat_loss(
        map.size(),
        std::vector<std::vector<int>>(
            map[0].size(),
            std::vector<int>(4 * (1 << needed_bits), std::numeric_limits<int>::max())
        )
    );

    while(front.size()) {
        std::pop_heap(front.begin(), front.end());
        auto cur = front.back();
        front.pop_back();

        // we can't stop if we moved less steps than min_straight_line_moves
        if (cur.straight_line_moves_count >= min_straight_line_moves) {
            unsigned int heat_loss_idx = cur.d << needed_bits | cur.straight_line_moves_count;
            // if the heat loss of the current move is not better drop this move
            if (heat_loss[cur.pos[1]][cur.pos[0]][heat_loss_idx] <= cur.cur_heat_loss) {
                continue;
            }

            // set new best heat loss value for this state
            heat_loss[cur.pos[1]][cur.pos[0]][heat_loss_idx] = cur.cur_heat_loss;

            // set best heat value to current's move heat_value for all states in cur position with
            // same direction that have a greater heat_value while also having greater straight_line_move_count.
            // But only if current's move straight_line_moves_count is bigger than the min_straight_line_moves.
            for (unsigned int i = cur.straight_line_moves_count+1; i <= max_straight_line_moves; ++i) {
                unsigned int cur_idx = (cur.d << needed_bits) | i;
                if (heat_loss[cur.pos[1]][cur.pos[0]][cur_idx] > cur.cur_heat_loss) {
                    heat_loss[cur.pos[1]][cur.pos[0]][cur_idx] = cur.cur_heat_loss;
                } else {
                    break;
                }
            }

            // we reached the goal
            if (cur.pos[1] == dest[1] && cur.pos[0] == dest[0]) {
                break;
            }
        }

        // move straight if possible
        if (cur.straight_line_moves_count < max_straight_line_moves && is_inbound(map, cur.pos, dir_to_relative_dir[cur.d])) {
            PosT next_pos = {cur.pos[0] + dir_to_relative_dir[cur.d][0], cur.pos[1] + dir_to_relative_dir[cur.d][1]};
            front.push_back({next_pos, cur.d, cur.straight_line_moves_count + 1, cur.cur_heat_loss + (map[next_pos[1]][next_pos[0]] - '0')});
            std::push_heap(front.begin(), front.end());
        }

        // move left if possible
        if (cur.straight_line_moves_count >= min_straight_line_moves) {
            Dir next_dir = move_left[cur.d];
            DirT test_dir = {
                std::max(static_cast<int>(min_straight_line_moves), 1) * dir_to_relative_dir[next_dir][0],
                std::max(static_cast<int>(min_straight_line_moves), 1) * dir_to_relative_dir[next_dir][1]
            };

            if (is_inbound(map, cur.pos, test_dir)) {
                PosT next_pos = {cur.pos[0] + dir_to_relative_dir[next_dir][0], cur.pos[1] + dir_to_relative_dir[next_dir][1]};
                front.push_back({next_pos, next_dir, 1, cur.cur_heat_loss + (map[next_pos[1]][next_pos[0]] - '0')});
                std::push_heap(front.begin(), front.end());
            }
        }

        // move right if possible
        if (cur.straight_line_moves_count >= min_straight_line_moves) {
            Dir next_dir = move_right[cur.d];
            DirT test_dir = {
                std::max(static_cast<int>(min_straight_line_moves), 1) * dir_to_relative_dir[next_dir][0],
                std::max(static_cast<int>(min_straight_line_moves), 1) * dir_to_relative_dir[next_dir][1]
            };

            if (is_inbound(map, cur.pos, test_dir)) {
                PosT next_pos = {cur.pos[0] + dir_to_relative_dir[next_dir][0], cur.pos[1] + dir_to_relative_dir[next_dir][1]};
                front.push_back({next_pos, next_dir, 1, cur.cur_heat_loss + (map[next_pos[1]][next_pos[0]] - '0')});
                std::push_heap(front.begin(), front.end());
            }
        }
    }

    // find the best heat value for destination and return it
    const std::vector<int>& dest_heat_loss = heat_loss[dest[1]][dest[0]];
    return *std::min_element(dest_heat_loss.begin(), dest_heat_loss.end());
}

aoch::Result solve_day17(aoch::Input& in) {
    aoch::Result a;

    PosT start = {0, 0};
    PosT destination = {in[0].size() - 1, in.size() - 1};

    a.part1 = std::to_string(dijkstra(in, start, destination));
    a.part2 = std::to_string(dijkstra(in, start, destination, 10, 4));

    return a;
}
