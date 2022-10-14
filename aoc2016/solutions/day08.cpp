#include <array>
#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


aoch::Result solve_day08(aoch::Input& in) {
    aoch::Result r;

    std::array<std::string, 6> screen;
    screen.fill(std::string(50, ' '));

    for (auto& l : in) {
        auto tokens = aoch::split(l);

        if (tokens[0] == "rect") {
            auto size_tokens = aoch::split(tokens[1], 'x');

            size_t width = std::stoul(size_tokens[0]);
            size_t height = std::stoul(size_tokens[1]);

            for (size_t h = 0; h < height; ++h) {
                for (size_t w = 0; w < width; w++) {
                    screen[h][w] = '#';
                }
            }
        } else {
            size_t idx = std::stoul(tokens[2].substr(2));
            size_t shift_amount = std::stoul(tokens[4]);

            if (tokens[1] == "row") {
                std::string tmp = screen[idx];

                for (size_t i = 0; i < 50; ++i) {
                    screen[idx][(i + shift_amount) % 50] = tmp[i];
                }
            } else {
                std::string tmp(6, ' ');

                for (size_t i = 0; i < 6; ++i) {
                    tmp[i] = screen[i][idx];
                }

                for (size_t i = 0; i < 6; ++i) {
                    screen[(i + shift_amount) % 6][idx] = tmp[i];
                }
            }
        }
    }

    int count_on = 0;
    for (auto& l : screen) {
        for (auto c : l) {
            if (c == '#') count_on++;
        }
    }

    r.part1 = std::to_string(count_on);

    std::array<std::string, 6> solution = {
        "#### #### #  # ####  ### ####  ##   ##  ###   ##  ",
        "   # #    #  # #    #    #    #  # #  # #  # #  # ",
        "  #  ###  #### ###  #    ###  #  # #    #  # #  # ",
        " #   #    #  # #     ##  #    #  # # ## ###  #  # ",
        "#    #    #  # #       # #    #  # #  # #    #  # ",
        "#### #    #  # #    ###  #     ##   ### #     ##  "
    };

    if (screen == solution) {
        r.part2 = "ZFHFSFOGPO";
    } else {
        r.part2 = "see above table";

        std::cout << "Day 8 part 2 solution:" << '\n';

        for (auto& l : screen) {
            std::cout << l << '\n';
        }
    }

    return r;
}
