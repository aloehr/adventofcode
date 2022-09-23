#include <string>
#include <iostream>

#include "aoch/AOCSolutionTypes.hpp"

aoch::Result solve_day08(aoch::Input& in) {
    aoch::Result r;

    int total_length = 0;

    for (const auto& str : in) {
        total_length += str.size();
    }

    int evaluated_length = 0;

    for (const auto& str : in) {

        int l = str.size() - 2;
        size_t i = 1;

        while (i < str.size() - 1) {

            if (str[i] != '\\') {
                i++;
            } else {
                if (str[i+1] == '\"') {
                    l -= 1;
                    i += 2;
                } else if (str[i+1] == '\\') {
                    l -= 1;
                    i += 2;
                } else if (str[i+1] == 'x') {
                    l -= 3;
                    i += 4;
                } else {
                    std::cout << "we should never reach this..." << std::endl;
                }
            }
        }

        evaluated_length += l;
    }

    r.part1 = std::to_string(total_length - evaluated_length);

    // part 2

    int added_length = 0;

    for (const auto& str : in) {
        int l = str.size() + 2;
        for (auto c : str) {
            if (c == '\\' || c == '\"') l++;
        }

        added_length += l;
    }

    r.part2 = std::to_string(added_length - total_length);

    return r;
}
