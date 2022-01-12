#pragma once

#include <vector>
#include <string>

struct answer {
    std::string part1 = "";
    std::string part2 = "";
};

using input = std::vector<std::string>;
using solve_fnc = answer (*) (input&);

answer solve_day01(input&);
answer solve_day02(input&);
answer solve_day03(input&);
answer solve_day04(input&);
answer solve_day05(input&);
answer solve_day06(input&);
answer solve_day07(input&);
answer solve_day08(input&);
answer solve_day09(input&);
answer solve_day10(input&);
answer solve_day11(input&);
answer solve_day12(input&);
answer solve_day13(input&);
answer solve_day14(input&);
answer solve_day15(input&);
answer solve_day16(input&);
answer solve_day17(input&);
answer solve_day18(input&);
answer solve_day19(input&);
answer solve_day20(input&);
answer solve_day21(input&);
answer solve_day22(input&);
answer solve_day23(input&);
answer solve_day24(input&);
answer solve_day25(input&);
