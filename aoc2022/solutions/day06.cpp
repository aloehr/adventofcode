#include <set>

#include "aoch/AOCSolutionTypes.hpp"


bool all_different(size_t p, size_t n, const std::string& s) {

    std::set<char> chars;

    for (size_t i = 0; i < n; ++i) {
        chars.insert(s[p-i]);
    }

    return chars.size() == n;
}

aoch::Result solve_day06(aoch::Input& in) {

    aoch::Result a;

    size_t ptr = 3;

    while (ptr < in[0].size()) {
        if (all_different(ptr++, 4, in[0])) {
            break;
        }

    }

    a.part1 = std::to_string(ptr);

    ptr = 13;

    while (ptr < in[0].size()) {
        if (all_different(ptr++, 14, in[0])) {
            break;
        }

    }

    a.part2 = std::to_string(ptr);

    return a;
}
