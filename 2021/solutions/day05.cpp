#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../aoc.hpp"

struct line {

    int x1, y1;
    int x2, y2;

    line(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}

    bool is_horizontal() const {
        return y1 == y2;
    }

    bool is_vertical() const {
        return x1 == x2;
    }

    void print() const {
        std::cout << x1 << "," << y1 << " -> " << x2 << "," << y2 << std::endl;
    }
};

std::pair<int, int> find_max_coords(const std::vector<line>& lines) {

    auto ret = std::make_pair(0, 0);

    for (const auto& l : lines) {
        ret.first = std::max(l.x1, ret.first);
        ret.first = std::max(l.x2, ret.first);
        ret.second = std::max(l.y1, ret.second);
        ret.second = std::max(l.y2, ret.second);
    }

    return ret;
}

answer solve_day05(input& in) {

    answer a;

    std::vector<line> lines;
    lines.reserve(in.size());

    for (auto& l : in) {
        auto tmp = split(l);

        assert(tmp.size() == 3);

        auto line_start = split(tmp[0], ',', false);
        auto line_end = split(tmp[2], ',', false);

        assert(line_start.size() == 2);
        assert(line_end.size() == 2);


        lines.push_back(line(
            std::stoi(line_start[0]),
            std::stoi(line_start[1]),
            std::stoi(line_end[0]),
            std::stoi(line_end[1])
        ));
    }

    auto max_vals = find_max_coords(lines);
    std::vector<std::vector<int>> field(max_vals.second+1, std::vector<int>(max_vals.first+1, 0));

    int count = 0;

    for (auto& l : lines) {
        if (l.is_horizontal()) {
            unsigned int s = std::min(l.x1, l.x2);
            unsigned int e = std::max(l.x1, l.x2);

            for (unsigned int i = s; i <= e; ++i) {
                field[l.y1][i]++;

                if (field[l.y1][i] == 2) count++;
            }
        }
        else if (l.is_vertical()) {
            unsigned int s = std::min(l.y1, l.y2);
            unsigned int e = std::max(l.y1, l.y2);

            for (unsigned int i = s; i <= e; ++i) {
                field[i][l.x1]++;

                if (field[i][l.x1] == 2) count++;
            }
        }
    }

    a.part1 = std::to_string(count);

    // part 2

    for (auto& l : lines) {
        if (!l.is_vertical() && !l.is_horizontal()) {
            int dir_x = (l.x1 < l.x2) ? 1 : -1;
            int dir_y = (l.y1 < l.y2) ? 1 : -1;

            int c_x = l.x1;
            int c_y = l.y1;

            while (c_x != l.x2 + dir_x) {
                field[c_y][c_x]++;

                if (field[c_y][c_x] == 2) count++;

                c_x += dir_x;
                c_y += dir_y;
            }

            assert(c_y == l.y2 + dir_y);
        }
    }

    a.part2 = std::to_string(count);

    return a;
}
