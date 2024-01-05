#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"

#include "BucketQueue.hpp"


enum Dir {NORTH, EAST, SOUTH, WEST};

using PosT = std::array<size_t, 2>;
using DirT = std::array<int, 2>;

struct State {
    PosT pos;
    Dir d;
    int cur_heat_loss = 0;

    State(const PosT& pos, Dir d, int cur_heat_loss)
        : pos(pos), d(d), cur_heat_loss(cur_heat_loss) {}

    bool operator<(const State& o) const {
        return this->cur_heat_loss > o.cur_heat_loss;
    }
};

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

    BucketQueue<State> front(1000);
    front.insert({start, Dir::EAST, 0}, 0);
    front.insert({start, Dir::SOUTH, 0}, 0);

    size_t map_h = map.size();
    size_t map_w = map[0].size();

    std::vector<int> seen(map_h * map_w * 4, 0);

    while(front.size()) {
        auto cur = front.min();
        front.pop_min();

        size_t seen_idx = (cur.pos[1] * map_w +  cur.pos[0]) * 4 + cur.d;
        if (seen[seen_idx]) continue;
        seen[seen_idx] = 1;

        // we reached the goal
        if (cur.pos == dest) {
            return cur.cur_heat_loss;
        }

        // move left and right
        for (size_t i = 1; i <= 3; i+=2) {
            PosT next_pos = cur.pos;
            auto next_heat_loss = cur.cur_heat_loss;
            Dir next_dir = static_cast<Dir>((cur.d + i) % 4);

            for (size_t j = 1; j <= max_straight_line_moves; ++j) {

                const DirT& next_rel_dir = dir_to_relative_dir[next_dir];

                next_pos[0] += next_rel_dir[0];
                next_pos[1] += next_rel_dir[1];

                if (next_pos[0] >= map_w || next_pos[1] >= map_h) break;

                next_heat_loss += map[next_pos[1]][next_pos[0]] - '0';

                if (j < min_straight_line_moves) continue;

                front.insert({next_pos, next_dir, next_heat_loss}, next_heat_loss);
            }
        }
    }

    // will only be reached if we never reach the goal
    return -1;
}

aoch::Result solve_day17(aoch::Input& in) {
    aoch::Result a;

    PosT start = {0, 0};
    PosT destination = {in[0].size() - 1, in.size() - 1};

    a.part1 = std::to_string(dijkstra(in, start, destination));
    a.part2 = std::to_string(dijkstra(in, start, destination, 10, 4));

    return a;
}
