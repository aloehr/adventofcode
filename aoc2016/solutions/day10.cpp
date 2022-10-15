#include <iostream>
#include <map>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


enum class dest_type {
    BOT,
    OUTPUT
};

struct bot {
    size_t id;

    dest_type low_type;
    size_t low_id;

    dest_type high_type;
    size_t high_id;

    std::vector<int> vals;
};

aoch::Result solve_day10(aoch::Input& in) {
    aoch::Result r;

    std::map<size_t, bot> bots;
    std::map<size_t, std::vector<int>> outs;

    for (auto& l : in) {
        auto tokens = aoch::split(l);

        if (tokens[0] == "bot") {
            size_t bot_id = std::stoul(tokens[1]);

            bots[bot_id].low_type = tokens[5] == "bot" ? dest_type::BOT : dest_type::OUTPUT;
            bots[bot_id].low_id = std::stoul(tokens[6]);

            bots[bot_id].high_type = tokens[10] == "bot" ? dest_type::BOT : dest_type::OUTPUT;
            bots[bot_id].high_id = std::stoul(tokens[11]);
        } else {
            int val = std::stoi(tokens[1]);
            size_t bot_id = std::stoul(tokens[5]);

            bots[bot_id].vals.push_back(val);
        }
    }

    bool changed = true;

    while (changed) {
        changed = false;

        for (auto& kv : bots) {
            if (kv.second.vals.size() >= 2) {
                changed = true;

                int low_val = std::min(kv.second.vals[0], kv.second.vals[1]);
                int high_val = std::max(kv.second.vals[0], kv.second.vals[1]);

                if (low_val == 17 && high_val == 61) {
                    r.part1 = std::to_string(kv.first);
                }

                kv.second.vals.erase(kv.second.vals.begin(), kv.second.vals.begin() + 2);

                if (kv.second.low_type == dest_type::BOT) {
                    bots[kv.second.low_id].vals.push_back(low_val);
                } else {
                    outs[kv.second.low_id].push_back(low_val);
                }

                if (kv.second.high_type == dest_type::BOT) {
                    bots[kv.second.high_id].vals.push_back(high_val);
                } else {
                    outs[kv.second.high_id].push_back(high_val);
                }
            }
        }
    }

    r.part2 = std::to_string(outs[0][0] * outs[1][0] * outs[2][0]);

    return r;
}
