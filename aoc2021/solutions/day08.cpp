#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


bool has_chars(std::string str, std::string chars) {
    for (auto c : chars) {
        if (str.find(c) == std::string::npos)
            return false;
    }

    return true;
}

aoch::Result solve_day08(aoch::Input& in) {

    aoch::Result a;

    int count = 0;
    int sum = 0;

    for (auto& l : in) {
        auto t = aoch::split(l, '|');
        auto t1 = aoch::split(t[0]);
        auto t2 = aoch::split(t[1]);


        for (const auto& a : t2) {
            if (a.size() == 2 || a.size() == 3 || a.size() == 4 || a.size() == 7)
                count++;
        }


        for (auto& a : t1) std::sort(a.begin(), a.end());
        for (auto& a : t2) std::sort(a.begin(), a.end());

        std::map<std::string, int> digits;
        std::string zero, one, two, three, four, five, six, seven, eight, nine;

        for (const auto& a : t1) {
            if (a.size() == 2) {
                digits[a] = 1;
                one = a;
            }
            else if (a.size() == 3) {
                digits[a] = 7;
                seven = a;
            }
            else if (a.size() == 4) {
                digits[a] = 4;
                four = a;
            }
            else if (a.size() == 7) {
                digits[a] = 8;
                eight = a;
            }
        }

        for (const auto& a : t1) {
            if (a.size() == 6) {
                if (has_chars(a, four)) {

                    digits[a] = 9;
                    nine = a;
                }
                else if (!has_chars(a, one)) {
                    digits[a] = 6;
                    six = a;
                }
                else {
                    digits[a] = 0;
                    zero = a;
                }
            }
        }

        for (const auto& a : t1) {
            if (a.size() == 5) {
                if (a.find_first_not_of(six) == std::string::npos) {
                    digits[a] = 5;
                    five = a;
                }
                else if (a.find_first_not_of(nine) != std::string::npos) {
                    digits[a] = 2;
                    two = a;
                }
                else {
                    digits[a] = 3;
                    three = a;
                }
            }
        }


        int number = 0;

        for (const auto& a : t2) {
            number = number * 10 + digits[a];
        }

        sum += number;

    }

    a.part1 = std::to_string(count);
    a.part2 = std::to_string(sum);

    return a;
}
