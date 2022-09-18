#include <array>
#include <set>
#include <string>
#include <utility>

#include "aoch/AOCSolutionTypes.hpp"

void move(std::array<int, 2>& pos, char cmd) {
    switch (cmd) {
        case '^':
            pos[1]++;
            break;
        case 'v':
            pos[1]--;
            break;
        case '<':
            pos[0]--;
            break;
        case '>':
        default:
            pos[0]++;
            break;
    }

    return;
}

aoch::Result solve_day03(aoch::Input& in) {
    aoch::Result r;

    std::array<int, 2> pos {0, 0};

    std::set<std::array<int, 2>> map = {{0, 0}};

    for (auto c : in[0]) {
        move(pos, c);

        map.insert(pos);
    }

    r.part1 = std::to_string(map.size());

    // part 2

    std::array<int, 2> pos1 {0, 0};
    std::array<int, 2> pos2 {0, 0};

    map.clear();
    map.insert(pos1);

    for (auto c : in[0]) {
        move(pos1, c);

        map.insert(pos1);
        std::swap(pos1, pos2);
    }

    r.part2 = std::to_string(map.size());

    return r;
}
