#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


void combinations(const std::string& m, std::map<std::string, std::vector<std::string>>& rules, std::set<std::string>& creations, size_t idx = 0) {

    if (idx >= m.size()) return;

    for (auto& kv : rules) {
        if (kv.first == m.substr(idx, kv.first.size())) {
            std::string prefix = m.substr(0, idx);
            std::string postfix = m.substr(idx + kv.first.size());

            for (const auto& r : kv.second) {
                creations.insert(prefix + r + postfix);
            }
        }
    }

    combinations(m, rules, creations, idx + 1);
}

int greedy(const std::string& m, const std::vector<std::string>& sorted_keys, std::map<std::string, std::vector<std::string>>& rules, const std::string& goal, int depth = 0) {

    if (m == goal) return depth;

    for (const auto& k : sorted_keys) {
        for (const auto& v : rules[k]) {
            size_t p = m.find(k);
            if (p != std::string::npos) {
                std::string prefix = m.substr(0, p);
                std::string postfix = m.substr(p + k.size());
                int ret = greedy(prefix + v + postfix, sorted_keys, rules, goal, depth + 1);

                if (ret >= 0) return ret;
            }
        }
    }

    return -1;
}

aoch::Result solve_day19(aoch::Input& in) {
    aoch::Result r;

    std::map<std::string, std::vector<std::string>> rules;
    std::map<std::string, std::vector<std::string>> rules_reversed;

    for (auto& l : in) {
        if (l == "") break;

        auto tokens = aoch::split(l);

        rules[tokens[0]].push_back(tokens[2]);
        rules_reversed[tokens[2]].push_back(tokens[0]);
    }

    std::string molecule = in.back();
    std::set<std::string> creations;

    combinations(molecule, rules, creations);

    r.part1 = std::to_string(creations.size());

    // part 2
    std::vector<std::string> sorted_keys;
    sorted_keys.reserve(rules_reversed.size());

    for (auto& kv : rules_reversed) {
        sorted_keys.push_back(kv.first);
    }

    std::sort(sorted_keys.rbegin(), sorted_keys.rend());

    r.part2 = std::to_string(greedy(molecule, sorted_keys, rules_reversed, "e"));

    return r;
}
