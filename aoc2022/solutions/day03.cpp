#include "aoch/AOCSolutionTypes.hpp"

int priority(char x) {
    if (x <= 'Z') {
        return static_cast<int>(x - 'A' + 27);
    } else {
        return static_cast<int>(x - 'a' + 1);
    }
}

aoch::Result solve_day03(aoch::Input& in) {

    aoch::Result a;

    int sum = 0;

    for (const auto& l : in) {
        for (size_t i = 0; i < l.length() / 2; ++i) {
            if (l.find(l[i], l.length() / 2) != std::string::npos) {
                sum += priority(l[i]);
                break;
            }
        }

    }

    a.part1 = std::to_string(sum);
    sum = 0;

    for (size_t i = 0; i < in.size(); i += 3) {
        for (auto c : in[i]) {
            if (in[i+1].find(c) != std::string::npos && in[i+2].find(c) != std::string::npos) {
                sum += priority(c);
                break;
            }
        }
    }

    a.part2 = std::to_string(sum);

    return a;
}
