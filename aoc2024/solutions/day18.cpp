#include <array>
#include <complex>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>
#include <aoch/string.hpp>


using Point = std::complex<int>;

template<>
struct std::hash<Point> {
    size_t operator()(const Point& p) const {
        return static_cast<size_t>(p.real()) << 32 | p.imag();
    }
};

aoch::Result solve_day18(aoch::Input& in) {
    aoch::Result a;

    const int n = 71;
    const size_t wanted_sim_steps = 1024;

    std::vector<std::string> grid(n, std::string(n, '.'));
    std::vector<Point> points;
    points.reserve(in.size());

    const auto is_inbound = [n] (const Point& p) {
        return p.imag() >= 0 && p.imag() < n && p.real() >= 0 && p.real() < n;
    };

    const auto get = [&grid] (const Point& p) -> char& {
        return grid[p.imag()][p.real()];
    };

    for (auto& l: in) {
        auto t = aoch::split(l, ',');

        points.emplace_back(std::stoi(t[0]), std::stoi(t[1]));
    }

    for (size_t i = 0; i < wanted_sim_steps; ++i) {
        get(points[i]) = '#';
    }

    auto find_goal = [&get, &is_inbound, n] (const Point& start, const Point& goal, std::unordered_set<Point>& path_set) {
        static const std::array<Point, 4> dirs {{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};
        const Point false_value = {n, n};

        static std::vector<Point> cur_front;
        static std::vector<Point> next_front;
        static std::vector<Point> visited;

        cur_front.clear();
        next_front.clear();


        visited.resize(n * n);
        std::fill(visited.begin(), visited.end(), false_value);

        cur_front.push_back(start);

        int step_count = -1;
        bool reached_goal = false;

        do {
            step_count++;

            while (!cur_front.empty()) {
                Point cur_pos = cur_front.back();
                cur_front.pop_back();

                if (cur_pos == goal) {
                    reached_goal = true;
                    break;
                }

                for (const auto& d : dirs) {
                    const auto next_pos = cur_pos + d;

                    if (is_inbound(next_pos) && visited[next_pos.imag() * n + next_pos.real()] == false_value && get(next_pos) != '#') {
                        next_front.push_back(next_pos);
                        visited[next_pos.imag() * n + next_pos.real()] = cur_pos;
                    }
                }

            }

            std::swap(cur_front, next_front);
        } while (!cur_front.empty() && !reached_goal);

        if (reached_goal) {
            path_set.insert({start, goal});

            Point cur = visited[goal.imag() * n + goal.real()];

            while (cur != start) {
                path_set.insert(cur);
                cur = visited[cur.imag() * n + cur.real()];
            }
        }

        return reached_goal ? step_count : -1;
    };

    const Point goal{n-1, n-1};
    const Point start{0, 0};

    std::unordered_set<Point> path_set;

    a.part1 = std::to_string(find_goal(start, goal, path_set));

    for (size_t i = wanted_sim_steps; i < points.size(); ++i) {
        get(points[i]) = '#';

        if (path_set.count(points[i]) != 0) {
            path_set.clear();

            if (find_goal(start, goal, path_set) == -1) {
                a.part2 = std::to_string(points[i].real()) + "," + std::to_string(points[i].imag());
                break;
            }
        }
    }

    return a;
}
