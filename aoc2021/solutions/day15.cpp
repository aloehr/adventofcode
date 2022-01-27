#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <queue>

#include "aoch/AOCSolutionTypes.hpp"


// resizes aoch::Input according to p2 requirements
aoch::Input resize(const aoch::Input& in) {
    size_t h = in.size();
    size_t w = in[0].size();

    aoch::Input ret(h * 5, std::string(w*5, '0'));

    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 5; ++j) {
            for (size_t k = 0; k < h; ++k) {
                for (size_t l = 0; l < w; ++l) {
                    int tmp = in[k][l] - '0' + i + j;
                    ret[i * h + k][j * w + l] = (tmp > 9 ? tmp - 9 : tmp) + '0';
                }
            }
        }
    }

    return ret;
}

// returns an vec of valid neighbours
std::vector<std::array<size_t, 2>> neighbours(const std::array<size_t, 2>& pos, const size_t h, const size_t w) {
    std::vector<std::array<size_t, 2>> ret(4);

    if (pos[0] > 0) ret.push_back({pos[0] - 1, pos[1]});
    if (pos[0] < h - 1) ret.push_back({pos[0] + 1, pos[1]});
    if (pos[1] > 0) ret.push_back({pos[0], pos[1] - 1});
    if (pos[1] < w - 1) ret.push_back({pos[0], pos[1] + 1});

    return ret;
}

// simple dikstra algo with priority queue
int dikstra(const aoch::Input& rel_d) {
    size_t h = rel_d.size();
    size_t w = rel_d[0].size();

    int inf = std::numeric_limits<int>::max();

    std::vector<int> abs_d(h * w, inf);
    abs_d[0] = 0;

    auto cmp = [&] (const size_t a, const size_t b) {
        return abs_d[a] > abs_d[b];
    };

    std::priority_queue<size_t, std::vector<size_t>, decltype(cmp)> q(cmp);
    q.push(0);

    while (q.size()) {

        auto u = q.top();
        q.pop();

        for (const auto& v : neighbours({u/w, u%w}, h, w)) {
            size_t tmp = v[0] * w + v[1];

            if (abs_d[tmp] == inf) {
                abs_d[tmp] = abs_d[u] + rel_d[v[0]][v[1]] - '0';
                q.push(tmp);
            }
        }
    }

    return abs_d.back();
}

aoch::Result solve_day15(aoch::Input& in) {

    aoch::Result a;

    a.part1 = std::to_string(dikstra(in));

    // part 2
    a.part2 = std::to_string(dikstra(resize(in)));

    return a;
}
