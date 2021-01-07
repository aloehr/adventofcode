#include <iostream>
#include <cassert>

#include "../aoc.hpp"


answer solve_day08(input& in) {

    answer a;

    unsigned int img_width  = 25;
    unsigned int img_height =  6;
    unsigned int img_size = img_width * img_height;

    unsigned int layer_count = in[0].size() / img_size;


    assert(in[0].size() % img_size == 0);


    // part 1
    unsigned int best_zero_count = img_size;
    unsigned int best_zero_layer_one_count = 0;
    unsigned int best_zero_layer_two_count = 0;

    for (size_t l = 0; l < layer_count; ++l) {
        unsigned int cur_layer_zero_count = 0;
        unsigned int cur_layer_one_count = 0;
        unsigned int cur_layer_two_count = 0;
        for (size_t i = 0; i < img_size; ++i) {
            char c = in[0][l * layer_count + i];
            if (c == '0') cur_layer_zero_count++;
            if (c == '1') cur_layer_one_count++;
            if (c == '2') cur_layer_two_count++;
        }

        if (cur_layer_zero_count < best_zero_count) {
            best_zero_count = cur_layer_zero_count;
            best_zero_layer_one_count = cur_layer_one_count;
            best_zero_layer_two_count = cur_layer_two_count;
        }
    }

    a.part1 = std::to_string(best_zero_layer_one_count * best_zero_layer_two_count);


    // part 2
    std::string img(img_size, 'X');

    for (size_t i = 0; i < img_size; ++i) {
        unsigned int cur_layer = 0;

        while (cur_layer < layer_count && in[0][cur_layer * img_size + i] == '2') {
            cur_layer++;
        }

        img[i] = in[0][cur_layer * img_size + i];
        if (img[i] == '0') img[i] = ' ';
        if (img[i] == '1') img[i] = '0';
    }

    std::string answ = "0000 000    00  00  000     0 0  0    0 0  0 0  0   0  000     0 0  0 000   0   0  0    0 0000 0  0 0    0  0 0  0 0  0 0  0 0000 000   00  0  0 000  ";

    if (img == answ) {
        a.part2 = "ZBJAB";
    }
    else {
        std::cout << "day 08 part 2:" << '\n';
        for (size_t i = 0; i < img_height; ++i) {
            std::cout << img.substr(i * img_width, img_width) << '\n';
        }

        a.part2 = "answer above table";
    }

    return a;
}
