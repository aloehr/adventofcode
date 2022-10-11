#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"


char button_to_press(const std::string& instr) {

    static int c_row = 1;
    static int c_column = 1;

    for (auto c : instr) {
        switch (c) {
            case 'D':
                c_row++;
                break;
            case 'U':
                c_row--;
                break;
            case 'L':
                c_column--;
                break;
            case 'R':
                c_column++;
                break;
            default:
                break;
        }
        c_row = std::min(2, std::max(0, c_row));
        c_column = std::min(2, std::max(0, c_column));
    }

    return '0' + c_row * 3 + c_column + 1;
}

char button_to_press2(const std::string& instr) {

    static int c_row = 2;
    static int c_column = 0;

    for (auto c : instr) {
        switch (c) {
            case 'D':
                if (c_column > 0 && c_column < 4) {
                    c_row++;
                }
                break;
            case 'U':
                if (c_column > 0 && c_column < 4) {
                    c_row--;
                }
                break;
            case 'L':
                if (c_row > 0 && c_row < 4) {
                    c_column--;
                }
                break;
            case 'R':
                if (c_row > 0 && c_row < 4) {
                    c_column++;
                }
                break;
            default:
                std::cout << "invalid input, unknown direction: '" << c << "'." << std::endl;
                break;
        }

        if (c_row == 2) {
            c_column = std::min(4, std::max(0, c_column));
        } else {
            c_column = std::min(3, std::max(1, c_column));
        }

        if (c_column == 2) {
            c_row = std::min(4, std::max(0, c_row));
        } else {
            c_row = std::min(3, std::max(1, c_row));
        }
    }

    const std::vector<std::string> key_pad = {
        {"XX1XX"},
        {"X234X"},
        {"56789"},
        {"XABCX"},
        {"XXDXX"},
    };

    return key_pad[c_row][c_column];
}

aoch::Result solve_day02(aoch::Input& in) {
    aoch::Result r;

    std::string code;

    for (const auto& l : in) {
        code += button_to_press(l);
    }

    r.part1 = code;

    // part 2
    std::string code2;

    for (const auto& l : in) {
        code2 += button_to_press2(l);
    }

    r.part2 = code2;
    return r;
}
