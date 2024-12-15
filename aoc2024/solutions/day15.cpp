#include <complex>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>


using Point = std::complex<int>;

int calc_coord_sum(const std::vector<std::string>& map, const char box) {
    int sum = 0;

    for (size_t i = 0; i < map.size(); ++i) {
        for (size_t j = 0; j < map[0].size(); ++j) {
            if (map[i][j] == box) {
                sum += 100 * i + j;
            }
        }
    }

    return sum;
}

aoch::Result solve_day15(aoch::Input& in) {
    aoch::Result a;

    std::vector<std::string> map;
    std::vector<std::string> moves;

    {
        size_t i = 0;

        while (in[i] != "") {
            map.push_back(std::move(in[i++]));
        }

        i++;

        while (i < in.size()) {
            moves.push_back(std::move(in[i++]));
        }
    }

    std::vector<std::string> wmap;
    wmap.reserve(map.size());

    for (auto& l : map) {
        wmap.push_back("");

        for (auto c : l) {
            if (c == '#') wmap.back() += "##";
            if (c == 'O') wmap.back() += "[]";
            if (c == '.') wmap.back() += "..";
            if (c == '@') wmap.back() += "@.";
        }
    }

    std::unordered_map<char, Point> dirs {{'^', {0, -1}}, {'>', {1, 0}}, {'v', {0, 1}}, {'<', {-1, 0}}};

    auto get = [&map] (const Point& p) -> char& {
        return map[p.imag()][p.real()];
    };

    auto find_start_pos = [] (const auto& map) -> Point {
        for (size_t i = 0; i < map.size(); ++i) {
            for (size_t j = 0; j < map[0].size(); ++j) {
                if (map[i][j] == '@') {
                    return {static_cast<int>(j), static_cast<int>(i)};
                }
            }
        }

        return {};
    };

    Point cur = find_start_pos(map);
    for (const auto& l : moves) {
        for (char c : l) {
            const auto& d = dirs[c];

            auto next_pos = cur + d;

            if (get(next_pos) == '.') {
                get(cur) = '.';
                get(next_pos) = '@';
                cur = next_pos;
            } else if (get(next_pos) == 'O') {

                auto x = next_pos;

                while (get(x) == 'O') {
                    x += d;
                }

                if (get(x) == '.') {
                    get(x) = 'O';
                    get(next_pos) = '@';
                    get(cur) = '.';
                    cur = next_pos;
                }
            }
        }
    }

    a.part1 = std::to_string(calc_coord_sum(map, 'O'));


    // part 2
    std::swap(map, wmap);

    std::function<bool(const Point&, const Point&)> can_push = [&map, &get, &can_push] (const Point& p, const Point& d) {
        Point p2 = p + Point{1, 0};

        if (get(p + d) == '#' || get(p2 + d) == '#') return false;

        if (get(p + d) == '.' && get(p2 + d) == '.') return true;
        else if (get(p+d) == '[') {
            return can_push(p+d, d);
        } else {
            bool push_left = true;
            if (get(p+d) == ']') {
                push_left = can_push(p + d - Point{1, 0}, d);
            }

            bool push_right = true;
            if (get(p2+d) == '[') {
                push_right = can_push(p2 + d, d);
            }

            return push_left && push_right;
        }

    };

    std::function<void(const Point&, const Point&)> push = [&get, &push, &map] (const Point& p, const Point& d) {
        Point p2 = p + Point{1, 0};

        if (get(p+d) == '[') {
            push(p+d, d);
        } else {
            if (get(p+d) == ']') {
                push(p + d - Point{1, 0}, d);
            }

            if (get(p2+d) == '[') {
                push(p2 + d, d);
            }
        }

        get(p) = '.';
        get(p2) = '.';
        get(p+d) = '[';
        get(p2+d) = ']';
    };

    cur = find_start_pos(map);

    for (const auto& l : moves) {
        for (const auto c : l) {
            const auto& d = dirs[c];

            auto next_pos = cur + d;

            if (get(next_pos) == '.') {

                get(cur) = '.';
                get(next_pos) = '@';
                cur = next_pos;
            } else if (d.imag() == 0 && (get(next_pos) == '[' || get(next_pos) == ']')) {
                auto x = next_pos;

                while (get(x) == '[' || get(x) == ']') {
                    x += d;
                }

                if (get(x) == '.') {
                    while (x != cur) {
                        get(x) = get(x - d);
                        x -= d;
                    }

                    get(cur) = '.';
                    cur = next_pos;
                }
            } else if (d.real() == 0 && (get(next_pos) == '[' || get(next_pos) == ']')) {
                if (get(next_pos) == '[' && can_push(next_pos, d)) {
                    push(next_pos, d);
                    get(cur) = '.';
                    get(next_pos) = '@';
                    cur = next_pos;
                } else if (get(next_pos) == ']' && can_push(next_pos - Point{1, 0}, d)) {
                    push(next_pos - Point{1, 0}, d);
                    get(cur) = '.';
                    get(next_pos) = '@';
                    cur = next_pos;
                }
            }

        }
    }

    a.part2 = std::to_string(calc_coord_sum(map, '['));

    return a;
}
