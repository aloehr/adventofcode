#include <algorithm>
#include <cassert>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"

int wrapping_paper_size(int l, int w, int h) {

    int r = 2 * (l*w + w*h + h*l);

    r += std::min(l*w, std::min(w*h, h*l));

    return r;
}

int ribbon_size(int l, int w, int h) {

    int r = std::min(2*l + 2*w, std::min(2*l + 2*h, 2*w + 2*h));

    r += l*w*h;

    return r;
}

aoch::Result solve_day02(aoch::Input& in) {
    aoch::Result r;

    int total_wrapping_needed = 0;
    int total_ribbon_needed = 0;

    for (auto& line : in) {
        auto s = aoch::split(line, 'x');

        assert(s.size() == 3);

        int l = std::stoi(s[0]);
        int w = std::stoi(s[1]);
        int h = std::stoi(s[2]);

        total_wrapping_needed += wrapping_paper_size(l, w, h);
        total_ribbon_needed += ribbon_size(l, w, h);
    }

    r.part1 = std::to_string(total_wrapping_needed);
    r.part2 = std::to_string(total_ribbon_needed);

    return r;
}
