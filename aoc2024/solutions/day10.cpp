#include <iostream>
#include <string>
#include <unordered_set>

#include <aoch/AOCSolutionTypes.hpp>


int calc_tralhead_scores_internal(size_t x, size_t y, const aoch::Input& map, std::unordered_set<size_t>& reached_peaks) {
    if (map[y][x] == '9') {
        reached_peaks.insert(x << 32 | y);
        return 1;
    }

    int total_ways = 0;

    if (x > 0 && map[y][x] - map[y][x-1] == -1)
        total_ways += calc_tralhead_scores_internal(x-1, y, map, reached_peaks);

    if (y > 0 && map[y][x] - map[y-1][x] == -1)
        total_ways += calc_tralhead_scores_internal(x, y-1, map, reached_peaks);

    if (x < map[0].size() - 1 && map[y][x] - map[y][x+1] == -1)
        total_ways += calc_tralhead_scores_internal(x+1, y, map, reached_peaks);

    if (y < map.size() - 1 && map[y][x] - map[y+1][x] == -1)
        total_ways += calc_tralhead_scores_internal(x, y+1, map, reached_peaks);

    return total_ways;
}

std::pair<int, int> calc_tralhead_scores(size_t x, size_t y, const aoch::Input& map) {
    std::unordered_set<size_t> reached_peaks;

    std::pair<int, int> res;
    res.second = calc_tralhead_scores_internal(x, y, map, reached_peaks);
    res.first = reached_peaks.size();

    return res;
}

aoch::Result solve_day10(aoch::Input& in) {
    aoch::Result a;

    int trailhead_scores_sum = 0;
    int trailhead_scores_sum2 = 0;

    for (size_t y = 0; y < in.size(); ++y) {
        for (size_t x = 0; x < in[0].size(); ++x) {
            if (in[y][x] != '0') continue;

            const auto scores = calc_tralhead_scores(x, y, in);
            trailhead_scores_sum += scores.first;
            trailhead_scores_sum2 += scores.second;
        }
    }

    a.part1 = std::to_string(trailhead_scores_sum);
    a.part2 = std::to_string(trailhead_scores_sum2);

    return a;
}
