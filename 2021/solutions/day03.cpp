#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../aoc.hpp"

std::string calc_most_common_bits(input& data) {
    std::string ret(data[0].size(), '0');

    std::vector<int> one_count(data[0].size(), 0);

    int max_count = data.size();

    for (auto& l : data) {
        for (unsigned int i = 0; i < l.size(); ++i) {
            if (l[i] == '1') {
                one_count[i]++;
            }
        }
    }

    for (unsigned int i = 0; i < one_count.size(); ++i) {
        if (one_count[i] >= (max_count - one_count[i])) {
            ret[i] = '1';
        }
    }

    return ret;
}

int bit_criteria_filter(input data, bool most_common_bit = true) {

    for (unsigned int i = 0; i < data[0].size(); ++i) {
        if (data.size() <= 1) break;


        std::string comparison = calc_most_common_bits(data);

        unsigned int j = 0;

        while (j < data.size()) {
            if (data[j][i] == comparison[i]) {
                if (most_common_bit)
                    j++;
                else
                    data.erase(data.begin()+j);
            } else {
                if (most_common_bit)
                    data.erase(data.begin()+j);
                else
                    j++;
            }
        }
    }

    if (data.size() == 1) {
        return std::stoi(data[0], 0, 2);
    } else {
        std::cout << "not left with single data entry, actual count: " << data.size() << std::endl;
        return -1;
    }
}

answer solve_day03(input& in) {

    answer a;

    // remove empty line
    in.pop_back();

    std::string gamma_rate_str = calc_most_common_bits(in);
    std::string epsilon_rate_str(gamma_rate_str.size(), '0');

    for (unsigned int i = 0; i < gamma_rate_str.size(); ++i) {
        if (gamma_rate_str[i] == '1') {
            epsilon_rate_str[i] = '0';
        } else {
            epsilon_rate_str[i] = '1';
        }
    }

    int gamma_rate = std::stoi(gamma_rate_str, 0, 2);
    int epsilon_rate = std::stoi(epsilon_rate_str, 0, 2);

    int power_consumption = gamma_rate * epsilon_rate;

    a.part1 = std::to_string(power_consumption);

    // part 2

    int oxygen_generator_rating = bit_criteria_filter(in, true);
    int co2_scrubber_rating = bit_criteria_filter(in, false);

    int life_support_rating = oxygen_generator_rating * co2_scrubber_rating;

    a.part2 = std::to_string(life_support_rating);

    return a;
}
