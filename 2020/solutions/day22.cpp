#include <vector>
#include <unordered_set>
#include <list>
#include <algorithm>

#include "../aoc.hpp"


int calc_score(const std::list<char>& l) {
    int mult = 1;
    int res = 0;

    for (auto it = l.rbegin(); it != l.rend(); ++it) {
        res += *it * mult++;
    }

    return res;
}

std::string create_identifier(const std::list<char>& p1, const std::list<char>& p2) {
    std::string s(p1.size() + p2.size() + 1, 'z');

    int j = 0;
    for (auto i : p1) {
        s[j++] = i;
    }

    j++;
    for (auto i : p2) {
        s[j++] = i;
    }

    return s;
}

std::pair<bool, int> play_game(std::list<char> p1, std::list<char> p2, bool recursive = false, bool score_needed = true) {

    std::unordered_set<std::string> memo;
    std::pair<bool, int> ret;

    while (!p1.empty() && !p2.empty()) {

        if (recursive) {
            if (!memo.insert(create_identifier(p1, p2)).second) {
                ret.first = true;
                if (score_needed)
                    ret.second = calc_score(p1);
                return ret;
            }
        }

        int p1_card = p1.front();
        int p2_card = p2.front();

        p1.pop_front();
        p2.pop_front();

        bool p1_won_round = true;
        if (p2_card > p1_card) p1_won_round = false;

        if (recursive && (int) p1.size() >= p1_card && (int) p2.size() >= p2_card) {

            auto p1_end = p1.begin();
            auto p2_end = p2.begin();
            for (int i = 0; i < p1_card; ++i) ++p1_end;
            for (int i = 0; i < p2_card; ++i) ++p2_end;

            char max_p1 = *std::max_element(p1.begin(), p1_end);
            char max_p2 = *std::max_element(p2.begin(), p2_end);

            if (max_p1 > max_p2) {
                p1_won_round = true; 
            }
            else {
                std::list<char> p1_copy(p1.begin(), p1_end);
                std::list<char> p2_copy(p2.begin(), p2_end);

                auto p = play_game(std::move(p1_copy), std::move(p2_copy), true, false);
                p1_won_round = p.first;
            }
        }

        if (p1_won_round) {
            p1.push_back(p1_card);
            p1.push_back(p2_card);
        }
        else {
            p2.push_back(p2_card);
            p2.push_back(p1_card);
        }
    }

    std::list<char>& winner = p1.empty() ? p2 : p1;

    ret.first = !p1.empty();
    if (score_needed)
        ret.second = calc_score(winner);

    return ret; 
}

answer solve_day22(input& in) {

    answer a;
    in.pop_back();

    std::list<char> p1;
    std::list<char> p2;

    bool player2_cards = false;
    for (size_t i = 1; i < in.size(); ++i) {
        
        if (in[i].empty()) {
            i++;
            player2_cards = true;
            continue; 
        } 

        if (!player2_cards) {
            p1.push_back(std::stoi(in[i]));
        }
        else {
            p2.push_back(std::stoi(in[i]));
        }
    }
    
    a.part1 = std::to_string(play_game(p1, p2).second);
    a.part2 = std::to_string(play_game(p1, p2, true).second);

    return a;
}
