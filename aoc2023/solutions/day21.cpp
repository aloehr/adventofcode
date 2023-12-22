#include<array>
#include<iostream>
#include<numeric>
#include<string>
#include<unordered_set>
#include<tuple>
#include<vector>
#include<queue>

#include<aoch/AOCSolutionTypes.hpp>


using PosT = std::array<int, 2>;
using MapT = std::vector<std::string>;
using QueueItemT = std::pair<PosT, unsigned int>;

static bool is_inbound(const MapT& map, const PosT& pos) {
    if (pos[0] < 0 || pos[1] < 0) return false;
    if (static_cast<size_t>(pos[1]) >= map.size() || static_cast<size_t>(pos[0]) >= map[pos[1]].size()) return false;
    return true;
}

unsigned long long bfs(MapT& map, const PosT& start, unsigned int max_steps) {

    std::unordered_set<size_t> seen_states;
    std::queue<QueueItemT> queue;
    queue.push({ std::make_pair(start, 0) });
    unsigned long long reachable = 0;

    while (queue.size()) {
        PosT cur_pos;
        unsigned int cur_steps;
        std::tie(cur_pos, cur_steps) = queue.front();
        queue.pop();

        size_t hash = (static_cast<size_t>(cur_pos[0]) << 32) | cur_pos[1];
        if (seen_states.count(hash)) {
            continue;
        } else {
            seen_states.insert(hash);
        }

        if ((cur_steps%2) == (max_steps%2)) {
            reachable++;
        }

        if (cur_steps == max_steps) continue;

        PosT next_pos = {cur_pos[0] + 1, cur_pos[1]};
        if (is_inbound(map, next_pos) && map[next_pos[1]][next_pos[0]] == '.') {
            queue.push(std::make_pair(next_pos, cur_steps+1));
        }

        next_pos = {cur_pos[0] - 1, cur_pos[1]};
        if (is_inbound(map, next_pos) && map[next_pos[1]][next_pos[0]] == '.') {
            queue.push(std::make_pair(next_pos, cur_steps+1));
        }

        next_pos = {cur_pos[0], cur_pos[1] + 1};
        if (is_inbound(map, next_pos) && map[next_pos[1]][next_pos[0]] == '.') {
            queue.push(std::make_pair(next_pos, cur_steps+1));
        }

        next_pos = {cur_pos[0], cur_pos[1] - 1};
        if (is_inbound(map, next_pos) && map[next_pos[1]][next_pos[0]] == '.') {
            queue.push(std::make_pair(next_pos, cur_steps+1));
        }
    }

    return reachable;
}

aoch::Result solve_day21(aoch::Input& in) {
// in = {
// "...........",
// ".....###.#.",
// ".###.##..#.",
// "..#.#...#..",
// "....#.#....",
// ".##..S####.",
// ".##..#...#.",
// ".......##..",
// ".##.#.####.",
// ".##..##.##.",
// "..........."};
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


    // part 1
    a.part1 = std::to_string(bfs(in, start, 64));


    // part 2
    // TODO
    // a.part2 = std::to_string();

    return a;
}
