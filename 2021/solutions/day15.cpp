#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <limits>

#include "../aoc.hpp"

input resize(const input& in) {
    size_t h = in.size();
    size_t w = in[0].size();

    input ret(h * 5, std::string(w*5, '0'));


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

std::vector<std::array<size_t, 2>> neighbours(const std::array<size_t, 2>& pos, const size_t h, const size_t w) {
    std::vector<std::array<size_t, 2>> ret;

    if (pos[0] > 0) ret.push_back({pos[0] - 1, pos[1]});
    if (pos[0] < h - 1) ret.push_back({pos[0] + 1, pos[1]});
    if (pos[1] > 0) ret.push_back({pos[0], pos[1] - 1});
    if (pos[1] < w - 1) ret.push_back({pos[0], pos[1] + 1});

    return ret;
}

long long dikstra(const std::vector<std::string>& rd) {
    size_t h = rd.size();
    size_t w = rd[0].size();

    std::vector<std::array<int, 2>> nb = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    long long inf = std::numeric_limits<long long>::max();

    std::vector<long long> ab(h * w, inf);
    ab[0] = 0;

    std::vector<long long> anchestors(h * w, -1);

    std::vector<std::array<size_t, 2>> q(1, {0, 0});

    while (ab.back() == inf) {

        std::sort(q.begin(), q.end(), [&] (const std::array<size_t, 2>& a, const std::array<size_t, 2>& b) {
            return ab[a[0] * w + a[1]] > ab[b[0] * w + b[1]];
        });

        auto u = q.back();
        q.pop_back();

        for (const auto& v : neighbours(u, h, w)) {
            if (ab[v[0] * w + v[1]] == inf) {
                long long alt_d = ab[u[0] * w + u[1]] + rd[v[0]][v[1]] - '0';
                if (alt_d < ab[v[0] * w + v[1]]) {
                    ab[v[0] * w + v[1]] = alt_d;
                    anchestors[v[0] * w + v[1]] = u[0] * w + u[1];
                    q.push_back(v);
                }
            }
        }
    }

    return ab.back();
}

answer solve_day15(input& in) {

    answer a;

    a.part1 = std::to_string(dikstra(in));

    // part 2
    a.part2 = std::to_string(dikstra(resize(in)));

    return a;
}
