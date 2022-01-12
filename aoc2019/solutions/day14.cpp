#include <unordered_map>
#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "../aoc.hpp"


size_t search_after_end_token(std::string s, size_t start) {
    size_t after_end = start;

    while (s[after_end] != ',' && s[after_end] != ' ') {
        after_end++;
    }

    return after_end;
}

answer solve_day14(input& in) {

    answer a;
    in.pop_back();

    std::unordered_map<std::string, unsigned int> chemical_ids;
    std::unordered_map<unsigned int, std::string> chemical_names;
    std::map<int, std::vector<std::pair<int, int>>> reactions;
    std::map<int, int> product_amount;

    unsigned int next_id = 0;
    for (auto& l : in) {

        std::vector<std::pair<int, int>> reaction;
        size_t p = 0;

        while (true) {
            size_t x = 0;
            int amount = std::stoi(l.substr(p), &x);
            p += x + 1;
            size_t ae = search_after_end_token(l, p);
            std::string chemical = l.substr(p, ae - p);
            unsigned int chemical_id = next_id;
            p = ae;

            if (chemical_ids.count(chemical)) {
                chemical_id = chemical_ids[chemical];
            }
            else {
                chemical_names[chemical_id] = chemical;
                chemical_ids[chemical] = next_id++;
            }

            reaction.push_back(std::make_pair(chemical_id, amount));

            if (l[p+1] == '=') break;

            p++;
            if (l[p] == ' ') p++;
        }
        p += 4;
        size_t x = 0;
        int amount = std::stoi(l.substr(p), &x);

        p += x + 1;

        std::string chemical = l.substr(p);
        unsigned int chemical_id = next_id;

        if (chemical_ids.count(chemical)) {
            chemical_id = chemical_ids[chemical];
        }
        else {
            chemical_names[chemical_id] = chemical;
            chemical_ids[chemical] = next_id++;
        }

        reactions[chemical_id] = reaction;
        product_amount[chemical_id] = amount;
    }

    std::unordered_map<unsigned int, unsigned long long> goals;
    goals[chemical_ids["FUEL"]] = 1;
    std::vector<unsigned long long> extra_chems(chemical_ids.size(), 0);

    int lc = 0;
    while (goals.size()) {

        auto it = goals.begin();
        while (it->first == chemical_ids["ORE"]) ++it;
        auto next = *it;


        goals.erase(next.first);

        if (extra_chems[next.first] >= next.second) {
            extra_chems[next.first] -= next.second;
            continue;
        }
        else {
            next.second -= extra_chems[next.first];
            extra_chems[next.first] = 0;
        }
        unsigned long long multiplier = 1;
        unsigned long long prod_amount = product_amount[next.first];

        while (multiplier * prod_amount < next.second) multiplier++;
        extra_chems[next.first] += multiplier * prod_amount - next.second;
        auto& reaction = reactions[next.first];

        for (auto c : reaction) {
            if (!goals.count(c.first)) {
                goals[c.first] = c.second * multiplier;
            }
            else {
                goals[c.first] += c.second * multiplier;
            }
        }

        if (goals.size() == 1 && (*goals.begin()).first == chemical_ids["ORE"]) break;

        lc++;
    }

    unsigned int min_ore_per_fuel = (*goals.begin()).second;

    a.part1 = std::to_string(min_ore_per_fuel);

    extra_chems = std::vector<unsigned long long>(chemical_ids.size(), 0);
    unsigned int ore_id = chemical_ids["ORE"];
    unsigned int fuel_id = chemical_ids["FUEL"];
    extra_chems[ore_id] = 1e12;
    bool change = true;
    unsigned long long produced_fuel = 0;
    goals.clear();

    while (change) {
        unsigned long long producable = extra_chems[ore_id] / min_ore_per_fuel;
        if (producable == 0) producable = 1;
        produced_fuel += producable;
        goals[fuel_id] = producable;
        while (goals.size()) {

            auto it = goals.begin();
            auto next = *it;


            goals.erase(next.first);

            if (extra_chems[next.first] >= next.second) {
                extra_chems[next.first] -= next.second;
                continue;
            }
            else {
                next.second -= extra_chems[next.first];
                extra_chems[next.first] = 0;
            }

            if (next.first == ore_id) {
                change = false;
                produced_fuel -= producable;
                break;
            }

            unsigned long long multiplier = 1;
            unsigned long long prod_amount = product_amount[next.first];
            if (next.second % prod_amount != 0) {
                multiplier = next.second / prod_amount + 1;
            }
            else {
                multiplier = next.second / prod_amount;
            }

            extra_chems[next.first] += multiplier * prod_amount - next.second;
            auto& reaction = reactions[next.first];

            for (auto c : reaction) {
                if (!goals.count(c.first)) {
                    goals[c.first] = c.second * multiplier;
                }
                else {
                    goals[c.first] += c.second * multiplier;
                }
            }
        }
    }

    a.part2 = std::to_string(produced_fuel);

    return a;
}
