#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"


std::string look_and_say(std::string& str) {
    std::string ret;

    int n = 1;
    char c = str[0];

    for (size_t i = 1; i < str.size(); ++i) {
        if (str[i] == c) {
            n++;
        } else {
            ret.append(std::to_string(n));
            ret.append(1, c);
            n = 1;
            c = str[i];
        }
    }

    ret.append(std::to_string(n));
    ret.append(1, c);

    return ret;
}

aoch::Result solve_day10(aoch::Input& in) {
    aoch::Result r;

    int n = 40;

    std::string str = in[0];

    while (n--) {
        str = look_and_say(str);
    }

    r.part1 = std::to_string(str.size());

    // part 2
    // 10 more for 50 overall
    n = 10;

    while (n--) {
        str = look_and_say(str);
    }

    r.part2 = std::to_string(str.size());

    return r;
}
