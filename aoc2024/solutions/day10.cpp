#include <iostream>
#include <string>
#include <unordered_map>

#include <aoch/AOCSolutionTypes.hpp>


int calc_tralhead_score_internal(size_t x, size_t y, const aoch::Input& map, std::unordered_map<size_t, int>& visited) {
    const size_t hash = x << 32 | y;

    if (map[y][x] == '9') {
        visited[hash] = 1;
        return 1;
    }

    if (visited.count(hash)) {
        return visited[hash];
    }

    int total_ways = 0;

    if (x > 0 && map[y][x] - map[y][x-1] == -1)
        total_ways += calc_tralhead_score_internal(x-1, y, map, visited);

    if (y > 0 && map[y][x] - map[y-1][x] == -1)
        total_ways += calc_tralhead_score_internal(x, y-1, map, visited);

    if (x < map[0].size() - 1 && map[y][x] - map[y][x+1] == -1)
        total_ways += calc_tralhead_score_internal(x+1, y, map, visited);

    if (y < map.size() - 1 && map[y][x] - map[y+1][x] == -1)
        total_ways += calc_tralhead_score_internal(x, y+1, map, visited);

    visited[hash] = total_ways;

    return total_ways;
}

std::pair<int, int> calc_tralhead_score(size_t x, size_t y, const aoch::Input& map) {
    std::unordered_map<size_t, int> visited;

    std::pair<int, int> res{};
    res.second = calc_tralhead_score_internal(x, y, map, visited);

    for (auto n : visited) {
        int x = n.first >> 32;
        int y = n.first;

        if (map[y][x] == '9') res.first++;
    }

    return res;
}

aoch::Result solve_day10(aoch::Input& in) {
    aoch::Result a;

    int trailhead_scores_sum = 0;
    int trailhead_scores_sum2 = 0;

    for (size_t y = 0; y < in.size(); ++y) {
        for (size_t x = 0; x < in[0].size(); ++x) {
            if (in[y][x] != '0') continue;

            const auto scores = calc_tralhead_score(x, y, in);
            trailhead_scores_sum += scores.first;
            trailhead_scores_sum2 += scores.second;
        }
    }

    a.part1 = std::to_string(trailhead_scores_sum);
    a.part2 = std::to_string(trailhead_scores_sum2);

    return a;
}
