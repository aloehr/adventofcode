#include <iostream>
#include <map>
#include <utility>

#include "../aoc.hpp"

#include "IntCodeVM.hpp"


void run_robot(memory rob_mem, std::map<std::pair<int, int>, int>& area, int start_color) {

    std::pair<int, int> rob_pos = std::make_pair(0, 0);
    area[rob_pos] = start_color;
    IntCodeVM vm("robot", rob_mem, {start_color});
    int rob_dir = 0;

    while (vm.state != IntCodeVM::vm_status::FINISHED) {

        vm.run();

        int turn = vm.out_args.back();
        vm.out_args.pop_back();
        int color = vm.out_args.back();
        vm.out_args.pop_back();

        area[rob_pos] = color;

        if (turn) {
            rob_dir = (rob_dir + 1) % 4;
        }
        else {
            rob_dir = rob_dir - 1 < 0 ? 3 : rob_dir - 1;
        }

        if (rob_dir == 0) {
            rob_pos.second++;
        }
        else if (rob_dir == 1) {
            rob_pos.first++;
        }
        else if (rob_dir == 2) {
            rob_pos.second--;
        }
        else {
            rob_pos.first--;
        }

        if (area.count(rob_pos)) {
            vm.in_args.push_back(area[rob_pos]);
        }
        else {
            vm.in_args.push_back(0);
        }
    }
}

answer solve_day11(input& in) {

    answer a;

    memory mem;
    std::string& data = in[0];

    size_t ptr = 0;
    do {

        data = data.substr(ptr);
        mem.push_back(std::stoll(data));
        ptr = data.find(',');

        if (ptr != std::string::npos)
            ptr++;

    } while (ptr != std::string::npos);

    std::map<std::pair<int, int>, int> area;

    run_robot(mem, area, 0);
    a.part1 = std::to_string(area.size());

    std::map<std::pair<int, int>, int> area2;
    run_robot(mem, area2, 1);

    bool first_round = true;
    int min_x = 0;
    int max_x = 0;
    int min_y = 0;
    int max_y = 0;

    for (const auto& p : area2) {
        if (first_round) {
            min_x = max_x = p.first.first;
            min_y = max_y = p.first.second;
            first_round = false;
        }
        min_x = std::min(min_x, p.first.first);
        min_y = std::min(min_y, p.first.second);

        max_x = std::max(max_x, p.first.first);
        max_y = std::max(max_y, p.first.second);
    }

    int img_width = std::abs(min_x - max_x) + 1;
    int img_height = std::abs(min_y - max_y) + 1;

    std::string empty_line(img_width, ' ');
    std::vector<std::string> img(img_height, empty_line);


    for (const auto& p : area2) {
        if (p.second) {
            size_t y = img_height - (p.first.second + (-1) * min_y) - 1;
            size_t x = p.first.first + (-1) * min_x;
            img[y][x] = 'O';
        }
    }

    std::vector<std::string> ans {
        {" OOO  OOOO OOOO  OO   OO  O  O  OO  OOOO   "},
        {" O  O O    O    O  O O  O O  O O  O O      "},
        {" OOO  OOO  OOO  O  O O    OOOO O  O OOO    "},
        {" O  O O    O    OOOO O OO O  O OOOO O      "},
        {" O  O O    O    O  O O  O O  O O  O O      "},
        {" OOO  O    OOOO O  O  OOO O  O O  O O      "}
    };

    if (img == ans) {
        a.part2 = "BFEAGHAF";
    }
    else {
        a.part2 = "look above table";
        std::cout << "day 11 part 2 answer:" << std::endl;
        for (auto& l : img) {
            std::cout << "'" <<  l << "'" << std::endl;
        }
    }

    return a;
}
