#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"

unsigned char calc_hash(const std::string& str) {
    unsigned char ret = 0;
    for (unsigned char c : str) {
        ret += c;
        ret *= 17;
    }

    return ret;
}

aoch::Result solve_day15(aoch::Input& in) {
    aoch::Result a;

    auto tokens = aoch::split(in[0], ',');
    size_t sum = 0;

    for (const auto& hash : tokens) {
        sum += calc_hash(hash);
    }

    a.part1 = std::to_string(sum);


    // part 2
    std::vector<std::vector<std::pair<std::string, int>>> boxes(256);

    for (const auto& step : tokens) {
        if (step.back() == '-') {
            auto label = step.substr(0, step.size() - 1);
            size_t hash = calc_hash(label);
            auto& cur_box = boxes[hash];

            auto search_result = std::find_if(cur_box.cbegin(), cur_box.cend(), [&label](const auto& a) {
                return a.first == label;
            });

            if (search_result != cur_box.cend()) {
                cur_box.erase(search_result);
            }
        } else {
            auto step_tokens = aoch::split(step, '=');

            size_t hash = calc_hash(step_tokens[0]);
            unsigned int focal_length = std::stoi(step_tokens[1]);

            auto& cur_box = boxes[hash];

            auto search_result = std::find_if(cur_box.begin(), cur_box.end(), [&step_tokens](const auto& a) {
                return a.first == step_tokens[0];
            });

            if (search_result == cur_box.end()) {
                cur_box.push_back(std::make_pair(step_tokens[0], focal_length));
            } else {
                search_result->second = focal_length;
            }
        }
    }

    unsigned long long focusing_power = 0;

    for (size_t i = 0; i < boxes.size(); ++i) {
        const auto& box = boxes[i];

        for (size_t j = 0; j < box.size(); ++j) {
            focusing_power += (i+1) * (j+1) * box[j].second;
        }
    }

    a.part2 = std::to_string(focusing_power);

    return a;
}
