#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <set>

#include "../aoc.hpp"

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

    a.part1 = std::to_string(fold({fold_lines[0]}, dots).size());

    // part 2
    dots = fold(fold_lines, dots);

    int max_x = 0;
    int max_y = 0;

    for (auto a : dots) {
        max_x = std::max(max_x, a[0]);
        max_y = std::max(max_y, a[1]);
    }

    std::stringstream ss;

    for (int y = 0; y <= max_y; ++y) {
        for (int x = 0; x <= max_x; ++x) {
            if (dots.count({x, y})) ss << "#";
            else ss << ".";
        }
        ss << "\n";
    }

    std::string rslt =
".##..###..####.#....###..####.####.#...\n\
#..#.#..#....#.#....#..#.#.......#.#...\n\
#....#..#...#..#....#..#.###....#..#...\n\
#....###...#...#....###..#.....#...#...\n\
#..#.#....#....#....#....#....#....#...\n\
.##..#....####.####.#....#....####.####\n";

    if (ss.str() != rslt) {
        std::cout << "Reslult day 12 part 2:" << std::endl;
        std::cout << ss.str() << std::endl;
        a.part2 = "look above table";
    } else {
        a.part2 = "CPZLPFZL";
    }


    return a;
}
