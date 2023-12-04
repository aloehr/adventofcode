#include <cmath>
#include <iostream>
#include <string>
#include <set>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


std::set<int> convert(const std::string& str) {
    auto tokens = aoch::split(str);

    std::set<int> ret;

    for (const auto& t : tokens) {
        ret.insert(std::stoi(t));
    }

    return ret;
}

aoch::Result solve_day04(aoch::Input& in) {

    aoch::Result a;

    int sum = 0;

    std::vector<int> cards_winning_numbers_count(in.size(), 0);

    uint idx = 0;

    for (const auto& l : in) {
        auto tokens = aoch::split(aoch::split(l, ':')[1], '|');

        auto winning_numbers = convert(tokens[0]);
        auto my_numbers = convert(tokens[1]);

        int my_winning_numbers_count = 0;

        for (auto n : my_numbers) {
            if (winning_numbers.count(n)) {
                my_winning_numbers_count++;
            }
        }

        cards_winning_numbers_count[idx++] = my_winning_numbers_count;

        if (my_winning_numbers_count) {
            sum += std::pow(2, my_winning_numbers_count - 1);
        }
    }

    a.part1 = std::to_string(sum);


    // part 2
    std::vector<uint> current_cards(in.size(), 1);

    sum = 0;

    for (uint i = 0; i < current_cards.size(); ++i) {
        sum += current_cards[i];

        for (int j = 1; j <= cards_winning_numbers_count[i]; ++j) {
            current_cards[i+j] += current_cards[i];
        }
    }

    a.part2 = std::to_string(sum);

    return a;
}
