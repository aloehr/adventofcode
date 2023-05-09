#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


struct Range {
    int start, end;

    Range(const std::string& r) {
        auto x = aoch::split(r, '-');

        this->start = std::stoi(x[0]);
        this->end = std::stoi(x[1]);
    }

    bool contains(Range o) {
        if (this->start <= o.start && this->end >= o.end) return true;
        else return false;
    }

    bool overlaps(Range o) {
        if (this->start <= o.end && this->end >= o.start) return true;
        else return false;
    }

};

aoch::Result solve_day04(aoch::Input& in) {

    aoch::Result a;

    int count = 0;
    int count2 = 0;

    for (const auto& l : in) {
        auto ranges = aoch::split(l, ',');

        Range a(ranges[0]);
        Range b(ranges[1]);

        if (a.contains(b) || b.contains(a)) {
            count++;
        }

        if (a.overlaps(b)) {
            count2++;
        }
    }

    a.part1 = std::to_string(count);
    a.part2 = std::to_string(count2);

    return a;
}
