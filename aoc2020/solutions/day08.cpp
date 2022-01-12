#include <iostream>
#include <map>
#include <vector>
#include <set>

#include "../aoc.hpp"


int execute(const std::vector<std::pair<int, int>>& instr, bool& looping) {

    std::vector<bool> executed(instr.size(), false);
    looping = false;
    size_t instr_idx = 0;
    int acc = 0;

    while (instr_idx < instr.size()) {
        if (executed[instr_idx]) {
            looping = true; 
            break;
        }

        executed[instr_idx] = true;

        if (instr[instr_idx].first == 2) {
            instr_idx++; 
        }
        else if (instr[instr_idx].first == 0) {
            acc += instr[instr_idx].second;
            instr_idx++;
        }
        else {
            instr_idx += instr[instr_idx].second;
        }
    }

    return acc;
}

// 0 - acc
// 1 - jmp
// 2 - nop
answer solve_day08(input& in) {

    answer a;
    in.pop_back();

    std::vector<std::pair<int, int>> instr;
    instr.reserve(in.size());

    for (auto& l : in) {
        if (l[0] == 'a') {
            instr.push_back(std::make_pair(0, std::stoi(l.substr(4))));
        }
        else if (l[0] == 'j') {
            instr.push_back(std::make_pair(1, std::stoi(l.substr(4))));
        }
        else {
            instr.push_back(std::make_pair(2, std::stoi(l.substr(4))));
        }
    }

    bool looping = false;
    a.part1 = std::to_string(execute(instr, looping));

    for (size_t i = 0; i < instr.size(); ++i) {
        if (instr[i].first) {
      
            int old_instr = instr[i].first;
            instr[i].first = (old_instr == 2) ? 1 : 2;

            looping = false;
            int acc = execute(instr, looping);

            if (!looping) {
                a.part2 = std::to_string(acc);
                break;
            }
            else {
                instr[i].first = (old_instr == 2) ? 2 : 1;
            }
        }
    }

    return a;
}
