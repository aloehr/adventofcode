#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


struct disc {
    int n;
    int rem;
};

// chinese remainder, sieving method
int cr(const std::vector<disc>& discs) {

    int add = 1;
    int x = discs[0].rem;

    for (size_t i = 0; i < discs.size()-1; ++i) {
        add *= discs[i].n;
        while (x % discs[i+1].n != discs[i+1].rem) {
            x += add;
        }
    }

    return x;
}

aoch::Result solve_day15(aoch::Input& in) {
    aoch::Result r;

    std::vector<disc> discs;
    discs.reserve(in.size());

    for (const auto& l : in) {

        auto tokens = aoch::split(l);
        disc d;

        d.n = std::stoi(tokens[3]);
        int id = std::stoi(tokens[1].substr(1));
        int pos_at_t0 = std::stoi(tokens[11]);

        d.rem = (d.n - ((pos_at_t0 + id) % d.n)) % d.n;
        discs.push_back(d);
    }

    r.part1 = std::to_string(cr(discs));

    int rem = (11 - (discs.size() + 1) % 11) % 11;
    discs.push_back({11, rem});

    r.part2 = std::to_string(cr(discs));

    return r;
}
