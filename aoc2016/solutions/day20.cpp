#include <algorithm>
#include <iostream>
#include <limits>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


using uint = unsigned int;

struct Range {

    uint start;
    uint end;

    bool combinable(const Range& o) const {
        if (std::max(start, o.end) - std::min(start, o.end) == 1 ||
            std::max(o.start, end) - std::min(o.start, end) == 1)
        {
            return true;
        }

        return !(o.start > (end) || o.end < (start));
    }

    void combine(const Range& o) {
        start = std::min(start, o.start);
        end = std::max(end, o.end);
    }

    bool operator<(const Range& o) const {
        return start < o.start;
    }
};

aoch::Result solve_day20(aoch::Input& in) {
    aoch::Result r;

    std::vector<Range> rs;
    rs.reserve(in.size());

    for (const auto& l : in) {
        auto tokens = aoch::split(l, '-');

        uint s = std::stoul(tokens[0]);
        uint e = std::stoul(tokens[1]);

        rs.push_back({s, e});
    }

    std::vector<Range> no_collisions;
    std::vector<Range> next;

    bool combined;

    while (rs.size()) {
        Range cur = rs.back();
        rs.pop_back();

        do {
            combined = false;
            for (size_t i = 0; i < rs.size(); ++i) {
                if (cur.combinable(rs[i])) {
                    combined = true;
                    cur.combine(rs[i]);
                } else {
                    next.push_back(rs[i]);
                }
            }

            rs.clear();
            std::swap(rs, next);
        } while (combined);

        no_collisions.push_back(cur);
    }

    std::sort(no_collisions.begin(), no_collisions.end());

    uint res = 0;
    if (no_collisions[0].start == 0) {
        res = no_collisions[0].end + 1;
    }

    r.part1 = std::to_string(res);


    // part 2
    unsigned long long num_allowed_ips = static_cast<unsigned long long>(std::numeric_limits<uint>::max()) + 1;

    for (const auto& ra : no_collisions) {
        num_allowed_ips -=  ra.end - ra.start + 1;
    }

    r.part2 = std::to_string(num_allowed_ips);

    return r;
}
