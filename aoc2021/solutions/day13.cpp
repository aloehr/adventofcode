#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <set>

#include "../aoc.hpp"

size_t fold_count(
    const std::array<int, 2>& fl,
    const std::set<std::array<int, 2>>& dots
) {

    size_t count = dots.size();
    for (auto p : dots) {

        if (p[fl[0]] >= fl[1]) {
            p[fl[0]] = fl[1] - (p[fl[0]] - fl[1]);

            if (dots.count(p)) {
                count--;
            }
        }
    }

    return count;
}

std::set<std::array<int, 2>> fold(
    const std::vector<std::array<int, 2>>& fold_lines,
    const std::set<std::array<int, 2>>& dots
) {

    std::set<std::array<int, 2>> ret;

    for (auto p : dots) {
        for (const auto& fl : fold_lines) {

            if (p[fl[0]] >= fl[1]) {
                p[fl[0]] = fl[1] - (p[fl[0]] - fl[1]);
            }
        }

        ret.insert(std::move(p));
    }

    return ret;
}

answer solve_day13(input& in) {

    answer a;

    std::set<std::array<int, 2>> dots;
    std::vector<std::array<int, 2>> fold_lines;

    size_t del_pos = 0;
    bool parse_dots = true;

    for (const auto& l : in) {
        if (l.empty()) {
            parse_dots = false;
            continue;
        }

        if (parse_dots) {
            dots.insert( {std::stoi(l, &del_pos), std::stoi(l.substr(del_pos+1))} );
        } else {
            del_pos = l.find('=');
            fold_lines.push_back( {l[del_pos-1] == 'x' ? 0 : 1, std::stoi(l.substr(del_pos+1))} );
        }
    }

    a.part1 = std::to_string(fold_count(fold_lines[0], dots));

    // part 2
    dots = fold(fold_lines, dots);

    int max_x = 0;
    int max_y = 0;

    for (auto a : dots) {
        max_x = std::max(max_x, a[0]);
        max_y = std::max(max_y, a[1]);
    }

    std::string str((max_x + 2) * (max_y + 1), '.');

    for (const auto& d : dots) {
        str[(max_x + 2) * d[1] + d[0]] = '#';
    }

    for (int y = 0; y <= max_y; ++y) {
        str[(max_x + 2) * y + max_x + 1] = '\n';
    }

    const std::string rslt =
".##..###..####.#....###..####.####.#...\n\
#..#.#..#....#.#....#..#.#.......#.#...\n\
#....#..#...#..#....#..#.###....#..#...\n\
#....###...#...#....###..#.....#...#...\n\
#..#.#....#....#....#....#....#....#...\n\
.##..#....####.####.#....#....####.####\n";

    if (str != rslt) {
        std::cout << "Reslult day 12 part 2:" << std::endl;
        std::cout << str << std::endl;
        a.part2 = "look above table";
    } else {
        a.part2 = "CPZLPFZL";
    }

    return a;
}
