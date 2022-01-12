#include <cassert>
#include <iostream>
#include <algorithm>
#include <string>
#include <thread>
#include <chrono>

#include "../aoc.hpp"

#include "IntCodeVM.hpp"


char int2char(int i) {
    switch (i) {
        case 0:
            return ' ';
        case 1:
            return '|';
        case 2:
            return 'X';
        case 3:
            return '^';
        case 4:
            return 'O';
        default:
            return '?';
    }
}

answer solve_day13(input& in) {

    answer a;
    in.pop_back();

    memory mem;
    size_t ptr = 0;
    while (true) {
        mem.push_back(std::stoll(in[0]));
        ptr = in[0].find(',');
        if (ptr == std::string::npos) break;
        in[0] = in[0].substr(ptr+1);

    }


    // part 1
    IntCodeVM arcade("arcade", mem, {});
    arcade.run();

    int count = 0;
    long long max_x = 0;
    long long max_y = 0;
    assert(arcade.out_args.size() % 3 == 0);
    for (size_t i = 0; i < arcade.out_args.size()/3; ++i) {
        if (arcade.out_args[3 * i + 2] == 2) count++;

        max_x = std::max(max_x, arcade.out_args[3*i + 0]);
        max_y = std::max(max_y, arcade.out_args[3*i + 1]);
    }

    a.part1 = std::to_string(count);


    // part 2
    std::string empty_line(max_x + 1, ' ');
    std::vector<std::string> screen(max_y + 1, empty_line);
    mem[0] = 2;
    arcade = IntCodeVM("arcade", mem, {});
    arcade.run();

    int input = 0;
    long long score = 0;

    auto ball_pos = std::make_pair(0, 0);
    auto paddle_pos = std::make_pair(0, 0);
    while (arcade.state != IntCodeVM::vm_status::FINISHED) {

        arcade.run();
        for (size_t i = 0; i < arcade.out_args.size()/3; ++i) {

            int x = arcade.out_args[3*i + 0];
            int y = arcade.out_args[3*i + 1];
            int t = arcade.out_args[3*i + 2];

            if (x == -1) {
                score = t;
                continue;
            }

            screen[y][x] = int2char(t);
            if (t == 3) {
                paddle_pos.first = x;
                paddle_pos.second = y;
            }
            else if (t == 4) {
                ball_pos.first = x;
                ball_pos.second = y;
            }
        }

        arcade.out_args.clear();

        if (ball_pos.first < paddle_pos.first) {
            input = -1;
        }
        else if (ball_pos.first > paddle_pos.first) {
            input = 1;
        }
        else {
            input = 0;
        }

        arcade.in_args.push_back(input);
    }

    a.part2 = std::to_string(score);

    return a;
}
