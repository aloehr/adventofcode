#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/math.hpp"


std::string calc_most_common_bits(const aoch::Input& data, std::vector<bool> *filtered = nullptr) {

    std::string ret(data[0].size(), '0');
    std::vector<int> one_count(data[0].size(), 0);
    int max_count = data.size();

    if (filtered)
        max_count -= aoch::sum(*filtered);

    for (size_t j = 0; j < data.size(); ++j) {
        if (filtered && (*filtered)[j]) continue;
        for (unsigned int i = 0; i < data[j].size(); ++i) {
            if (data[j][i] == '1') {
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



int bit_criteria_filter(const aoch::Input& data, bool most_common_bit = true) {

    std::vector<bool> filtered(data.size(), false);
    int unfiltered_count = filtered.size();
    for (unsigned int i = 0; i < data[0].size(); ++i) {
        if (unfiltered_count <= 1) break;


        std::string comparison = calc_most_common_bits(data, &filtered);

        for (size_t j = 0; j < data.size(); ++j) {
            if (filtered[j]) continue;
            if (data[j][i] == comparison[i]) {
                if (!most_common_bit) {
                    filtered[j] = true;
                    unfiltered_count--;
                }
            } else {
                if (most_common_bit) {
                    filtered[j] = true;
                    unfiltered_count--;
                }
            }
        }
    }

    assert(unfiltered_count == 1);

    size_t i = 0;
    while (i < filtered.size()) {
        if (!filtered[i]) {
            break;
        }
        ++i;
    }

    return std::stoi(data[i], nullptr, 2);
}

aoch::Result solve_day03(aoch::Input& in) {

    aoch::Result a;

    std::string gamma_rate_str = calc_most_common_bits(in);
    std::string epsilon_rate_str(gamma_rate_str.size(), '0');

    for (unsigned int i = 0; i < gamma_rate_str.size(); ++i) {
        if (gamma_rate_str[i] == '1') {
            epsilon_rate_str[i] = '0';
        } else {
            epsilon_rate_str[i] = '1';
        }
    }

    int gamma_rate = std::stoi(gamma_rate_str, nullptr, 2);
    int epsilon_rate = std::stoi(epsilon_rate_str, nullptr, 2);

    int power_consumption = gamma_rate * epsilon_rate;

    a.part1 = std::to_string(power_consumption);

    // part 2

    int oxygen_generator_rating = bit_criteria_filter(in, true);
    int co2_scrubber_rating = bit_criteria_filter(in, false);

    int life_support_rating = oxygen_generator_rating * co2_scrubber_rating;

    a.part2 = std::to_string(life_support_rating);

    return a;
}
