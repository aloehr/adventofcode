#include <algorithm>

#include <iostream>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"


bool is_digit(char x) {
    return x >= '0' && x <= '9';
}

int compare(const std::string& l, uint idx) {
    std::vector<std::string> num_strs = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    for (uint i = 0; i < num_strs.size(); ++i) {

        if (idx + num_strs[i].length() > l.length()) continue;

        bool valid = true;

        for (uint j = 0; j < num_strs[i].length(); ++j) {
            if (num_strs[i][j] != l[idx + j]) {
                valid = false;
                break;
            }
        }

        if (valid) {
            return i + 1;
        }
    }

    return 0;
}

aoch::Result solve_day01(aoch::Input& in) {

    aoch::Result a;

    int sum = 0;

    for (const auto& l : in) {
        int n = 0;

        for (uint i = 0; i < l.length(); ++i) {
            if (is_digit(l[i])) {
                n = 10 * ((int) l[i] - (int) '0');
                break;
            }
        }

        for (uint i = l.length(); i > 0; --i) {
            if (is_digit(l[i-1])) {
                n += ((int) l[i-1] - (int) '0');
                break;
            }
        }

        sum += n;
    }

    a.part1 = std::to_string(sum);


    // part 2
    sum = 0;

    for (const auto& l : in) {
        int n = 0;

        for (uint i = 0; i < l.length(); ++i) {
            if (is_digit(l[i])) {
                n = 10 * ((int) l[i] - (int) '0');
                break;
            } else if (int x = compare(l, i)) {
                n = 10 * x;
                break;
            }
        }

        for (uint i = l.length(); i > 0; --i) {
            if (is_digit(l[i-1])) {
                n += ((int) l[i-1] - (int) '0');
                break;
            } else if (int x = compare(l, i-1)) {
                n += x;
                break;
            }
        }

        sum += n;
    }

    a.part2 = std::to_string(sum);

    return a;
}
