#include <algorithm>
#include <array>
#include <complex>
#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>
#include <aoch/string.hpp>


using Point = std::complex<int>;

template <>
struct std::hash<Point> {
    size_t operator()(const Point& p) const {
        return (static_cast<size_t>(p.imag()) << 32) | p.real();
    }
};

enum dir {N, E, S, W};

aoch::Result solve_day16(aoch::Input& in) {
    aoch::Result a;

//     in = {
// "###############",
// "#.......#....E#",
// "#.#.###.#.###.#",
// "#.....#.#...#.#",
// "#.###.#####.#.#",
// "#.#.#.......#.#",
// "#.#.#####.###.#",
// "#...........#.#",
// "###.#.#####.#.#",
// "#...#.....#.#.#",
// "#.#.#.###.#.#.#",
// "#.....#...#.#.#",
// "#.###.#.#.#.#.#",
// "#S..#.....#...#",
// "###############",
//     };

    const size_t w = in[0].size();
    const size_t h = in.size();
    auto get = [&in] (const Point& p) -> char& {
        return in[p.imag()][p.real()];
    };

    Point start_pos, goal_pos;
    dir start_dir = E;
    for (int y = 0; y < static_cast<int>(in.size()); ++y) {
        for (int x = 0; x < static_cast<int>(in[0].size()); ++x) {
            if (get({x, y}) == 'S') {
                start_pos = {x, y};
                get({x,y}) = '.';
            }
            if (get({x, y}) == 'E') {
                goal_pos = {x, y};
                get({x,y}) = '.';
            }
        }
    }

    std::vector<std::pair<int, std::unordered_set<Point>>> visited(w * h * 4);

    // std::cout << start_pos << " " << goal_pos << std::endl;

    // for (auto& l : in) std::cout << l << std::endl;

    using pq_item = std::tuple<int, Point, dir, std::vector<Point>>;

    auto cmp = [] (const pq_item& a, const pq_item& b) {
        return std::get<0>(a) > std::get<0>(b);
    };


    const std::array<Point, 4> dirs {{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};

    std::priority_queue<pq_item, std::vector<pq_item>, decltype(cmp)> pq(cmp);
    pq.emplace(0, start_pos, start_dir, std::vector<Point>{start_pos});

    auto calc_idx = [w, h] (const Point& p, dir d) -> size_t {
        return p.imag() * w * 4 + p.real() * 4 + d;
    };


    std::unordered_set<Point> ms;
    std::optional<int> best = std::nullopt;

    // return a;
    while (pq.size()) {
        auto [val, pos, d, p] = pq.top();
        pq.pop();

        if (best && val > *best) continue;

        // std::cout << val << " " << pos << " " << d << std::endl;


        size_t idx = calc_idx(pos, d);
        if (visited[idx].second.size() && val > visited[idx].first) {
            continue;
        } else if (visited[idx].second.size() == 0) {
            visited[idx].first = val;
            visited[idx].second.insert(p.begin(), p.end());
        } else  {
            visited[idx].second.insert(p.begin(), p.end());
        }

        if (pos == goal_pos) {
            if (!best || *best == val) {
                if (!best) {
                    a.part1 = std::to_string(val);
                    best = val;
                }

                ms.insert(p.begin(), p.end());
            }
        }


        // if (visited[calc_idx(pos, d)]) continue;
        // visited[calc_idx(pos, d)] = true;;

        Point next_pos = pos + dirs[d];

        if (get(next_pos) == '.') {
            p.push_back(next_pos);
            pq.emplace(val + 1, next_pos, d, p);
            p.pop_back();
        }

        if (get(pos + dirs[(d + 1) % 4]) != '#')
            pq.push(std::make_tuple(val + 1000, pos, static_cast<dir>((d + 1) % 4), p));

        if (get(pos + dirs[(d + 3) % 4]) != '#')
            pq.push({val + 1000, pos, static_cast<dir>((d + 3) % 4), p});
    }

    // for (auto& p : ms) get(p) = 'O';
    // for (auto& l : in) std::cout << l << std::endl;

    a.part2 = std::to_string(ms.size());

    return a;
}
