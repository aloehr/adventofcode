#include <unordered_map>
#include <vector>
#include <set>

#include "../aoc.hpp"


answer solve_day07(input& in) {

    answer a;
    in.pop_back(); // removes last blank line

    int next_bag_id = 0;
    std::unordered_map<std::string, int> bag_ids;
    std::vector<std::vector<std::pair<int, int>>> data(in.size());

    for (auto& line : in) {
        size_t pos = line.find(' ');
        pos = line.find(' ', pos+1);

        std::string key_str = line.substr(0, pos);
        int key;

        if (bag_ids.count(key_str)) {
            key = bag_ids[key_str];
        }
        else {
            key = next_bag_id++;
            bag_ids[key_str] = key;
        }

        pos = line.find(' ', pos+1);
        pos = line.find(' ', pos+1);
        line = line.substr(pos+1);
        
        std::vector<std::pair<int, int>> value;
        while (true) {

            if (line[0] == 'n') {
                data[key] = std::move(value); 
                break;
            }

            int c = std::stoi(line, &pos);

            line = line.substr(pos+1);
            pos = line.find(' ');
            pos = line.find(' ', pos+1);
            std::string color_str = line.substr(0, pos);
            int color;

            if (bag_ids.count(color_str)) {
                color = bag_ids[color_str];
            }
            else {
                color = next_bag_id++;
                bag_ids[color_str] = color;
            }

            value.push_back(std::make_pair(color, c));

            pos = line.find(',');

            if (pos == std::string::npos) {
                data[key] = std::move(value);
                break;
            }

            line = line.substr(pos+2);
        }
        
    }

    int count_rules = 0;
    std::vector<int> unresolved_rules = {bag_ids["shiny gold"]};
    std::set<int> already_resolved;
    
    while (unresolved_rules.size() != 0) {
        int current_goal = unresolved_rules.back();
        unresolved_rules.pop_back();

        for (size_t i = 0; i < data.size(); ++i) {
            if (already_resolved.count(i)) continue;
            for (auto& p2 : data[i]) {
                if (p2.first == current_goal) {
                    count_rules++;
                    unresolved_rules.push_back(i);
                    already_resolved.insert(i);
                    break;
                }
            }
        }
    }

    std::vector<std::pair<int, int>> entries = {std::make_pair(bag_ids["shiny gold"], 1)};

    int needed_bags = 0;
    while (entries.size()) {
        auto p = entries.back();
        entries.pop_back();

        needed_bags += p.second;

        auto& sub_rules = data[p.first];
        for (auto& p2 : sub_rules) {
            entries.push_back(std::make_pair(p2.first, p2.second * p.second));
        }
    }

    a.part1 = std::to_string(count_rules);
    a.part2 = std::to_string(needed_bags - 1); // -1 because we counted the shiny gold bag as well

    return a;
}
