#include <string>
#include <vector>

#include "../aoc.hpp"


void play_game(const std::vector<unsigned int>& cups, std::vector<unsigned int>& cup_list, int num_moves) {
    for (unsigned int i = 0; i < cups.size()-1; ++i)  {
        cup_list[cups[i]] = cups[i+1]; 
    }

    if (cup_list.size() == cups.size()) {
        cup_list[cups[cups.size()-1]] = cups[0];
    }
    else {
        for (unsigned int i = cups.size(); i < cup_list.size()-1; ++i) {
            cup_list[i] = i+1;
        }

        cup_list[cups[cups.size()-1]] = cups.size();
        cup_list[cup_list.size()-1] = cups[0];
    }

    int cur_cup = cups[0];

    while (num_moves--) {
        int a = cup_list[cur_cup];
        int b = cup_list[a];
        int c = cup_list[b];
        
        int dest_cup = cur_cup ? cur_cup - 1 : cup_list.size() - 1;

        while (dest_cup == a || dest_cup == b || dest_cup == c) {
            dest_cup = dest_cup ? dest_cup - 1 : cup_list.size() - 1;
        }

        cur_cup = cup_list[cur_cup] = cup_list[c];
        cup_list[c] = cup_list[dest_cup];
        cup_list[dest_cup] = a;
    }
    
}


answer solve_day23(input& in) {

    answer a;
    in.pop_back();

    std::vector<unsigned int> cups;
    cups.reserve(in.size());

    for (const auto& l : in) {
        for (auto c : l) {
            cups.push_back(c - '0' - 1); 
        }
    }

    std::vector<unsigned int> cup_list(cups.size());
    play_game(cups, cup_list, 100);

    int r = 0;
    int cc = cup_list[0];

    while (cc != 0) {
        r *= 10;
        r += cc+1;
        cc = cup_list[cc];
    }
 
    a.part1 = std::to_string(r);

    // part 2
    cup_list = std::vector<unsigned int>(1e6);

    play_game(cups, cup_list, 1e7);

    a.part2 = std::to_string((cup_list[0] + 1) * (cup_list[cup_list[0]] + 1));

    return a;
}
