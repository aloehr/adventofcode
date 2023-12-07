#include <algorithm>
#include <iostream>
#include <map>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


struct Hand {

    enum HAND_TYPE { FIVE_OF_A_KIND, FOUR_OF_A_KIND, FULL_HOUSE, THREE_OF_A_KIND, TWO_PAIR, ONE_PAIR, HIGH_CARD };

    static std::map<HAND_TYPE, std::string> HAND_TYPE_STRINGS;
    static std::map<char, int> CARD_STRENGTH;
    static int c;

    std::string cards;
    int bid_amount;
    HAND_TYPE type;
    bool use_jokers;


    static int get_card_strength(char c, bool use_jokers = false) {
        if (use_jokers && c == 'J') {
            return CARD_STRENGTH['2'] + 1;
        }

        return CARD_STRENGTH[c];
    }


    Hand(const std::string& cards, int bid_amount, bool use_jokers = false) : cards(cards), bid_amount(bid_amount), use_jokers(use_jokers){
        c++;

        this->type = this->determine_hand_type();
    }

    void set_use_jokers(bool val) {
        this->use_jokers = val;

        this->type = this->determine_hand_type();
    }

    HAND_TYPE determine_hand_type() {
        std::map<char, int> cards_counts;

        for (auto c : this->cards) {
            if (cards_counts.count(c)) {
                cards_counts[c]++;
            } else {
                cards_counts[c] = 1;
            }
        }

        if (cards_counts.size() == 1) {
            return FIVE_OF_A_KIND;
        } else if (cards_counts.size() == 2) {
            // FOUR_OF_A_KIND or FULL_HOUSE
            int tmp = 1;

            for (const auto& kv : cards_counts) {
                tmp *= kv.second;
            }

            if (this->use_jokers && cards_counts.count('J')) {
                return FIVE_OF_A_KIND;
            }

            if (tmp == 4) {
                return FOUR_OF_A_KIND;
            } else {
                return FULL_HOUSE;
            }
        } else if (cards_counts.size() == 3) {
            // THREE_OF_A_KIND or TWO_PAIR
            int tmp = 1;

            for (const auto& kv : cards_counts) {
                tmp *= kv.second;
            }

            if (tmp == 3) {
                if (this->use_jokers && cards_counts.count('J')) {
                    return FOUR_OF_A_KIND;
                }

                return THREE_OF_A_KIND;
            } else {
                if (this->use_jokers && cards_counts.count('J') && cards_counts['J'] == 1) {
                    return FULL_HOUSE;
                } else if (this->use_jokers && cards_counts.count('J') && cards_counts['J'] == 2) {
                    return FOUR_OF_A_KIND;
                }

                return TWO_PAIR;
            }
        } else if (cards_counts.size() == 4) {
            if (this->use_jokers && cards_counts.count('J')) {
                return THREE_OF_A_KIND;
            }

            return ONE_PAIR;
        } else {
            if (this->use_jokers && cards_counts.count('J')) {
                return ONE_PAIR;
            }

            return HIGH_CARD;
        }
    }

    bool operator<(const Hand& o) {
        if (this->type != o.type) {
            return this->type < o.type;
        } else {
            for (uint i = 0; i < this->cards.size(); ++i) {
                if (this->cards[i] == o.cards[i]) continue;

                return Hand::get_card_strength(this->cards[i], this->use_jokers) < Hand::get_card_strength(o.cards[i], o.use_jokers);
            }
        }

        // if we reach this both hands have the same cards in the same order
        return true;

    }
};

int Hand::c = 0;

std::map<Hand::HAND_TYPE, std::string> Hand::HAND_TYPE_STRINGS {
    {FIVE_OF_A_KIND, "FIVE_OF_A_KIND"},
    {FOUR_OF_A_KIND, "FOUR_OF_A_KIND"},
    {FULL_HOUSE, "FULL_HOUSE"},
    {THREE_OF_A_KIND, "THREE_OF_A_KIND"},
    {TWO_PAIR, "TWO_PAIR"},
    {ONE_PAIR, "ONE_PAIR"},
    {HIGH_CARD, "HIGH_CARD"}
};

std::map<char, int> Hand::CARD_STRENGTH {
    {'A', 0},
    {'K', 1},
    {'Q', 2},
    {'J', 3},
    {'T', 4},
    {'9', 5},
    {'8', 6},
    {'7', 7},
    {'6', 8},
    {'5', 9},
    {'4', 10},
    {'3', 11},
    {'2', 12},
};

aoch::Result solve_day07(aoch::Input& in) {
    aoch::Result a;

    std::vector<Hand> hands;

    for (const auto& l : in) {
        auto tokens = aoch::split(l);

        hands.push_back(Hand(tokens[0], std::stoi(tokens[1])));
    }

    std::sort(hands.begin(), hands.end());

    int total_winnings = 0;
    for (uint i = 0; i < hands.size(); ++i) {
        total_winnings += (hands.size() - i) * hands[i].bid_amount;
    }

    a.part1 = std::to_string(total_winnings);


    // part 2
    for (auto& h : hands) {
        h.set_use_jokers(true);
    }

    std::sort(hands.begin(), hands.end());

    total_winnings = 0;
    for (uint i = 0; i < hands.size(); ++i) {
        total_winnings += (hands.size() - i) * hands[i].bid_amount;
    }

    a.part2 = std::to_string(total_winnings);

    return a;
}
