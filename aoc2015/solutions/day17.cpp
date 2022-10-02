#include <iostream>
#include <map>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"

int recursive(std::vector<int> c, int left_to_fill) {
    if (left_to_fill == 0) return 1;
    if (c.empty()) return 0;

    int cur = c.back();
    c.pop_back();

    int ret = recursive(c, left_to_fill);

    if (cur <= left_to_fill) {
        ret += recursive(c, left_to_fill - cur);
    }

    return ret;
}

void recursive2(std::vector<int> c, int left_to_fill, std::map<int, int>& counts, int c_count = 0) {
    if (left_to_fill == 0) {
        if (!counts.count(c_count)) counts[c_count] = 1;
        else counts[c_count]++;
        return;
    }

    if (c.empty()) return;

    int cur = c.back();
    c.pop_back();

    recursive2(c, left_to_fill, counts, c_count);

    if (cur <= left_to_fill) {
        recursive2(c, left_to_fill - cur, counts, c_count + 1);
    }
}

aoch::Result solve_day17(aoch::Input& in) {
    aoch::Result r;

    std::vector<int> containers;
    containers.reserve(in.size());

    for (auto& l : in) {
        containers.push_back(std::stoi(l));
    }

    r.part1 = std::to_string(recursive(containers, 150));

    // part 2
    std::map<int, int> counts;
    recursive2(containers, 150, counts);

    int min_c_count = 999999;
    for (auto kv : counts) {
        if (kv.first < min_c_count) {
            min_c_count = kv.first;
        }
    }

    r.part2 = std::to_string(counts[min_c_count]);
    return r;
}
