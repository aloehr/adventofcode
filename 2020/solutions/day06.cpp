#include <iostream>
#include <unordered_set>
#include <vector>

#include "../aoc.hpp"

answer solve_day06(input& in) {

    answer a;

    int sum = 0;
    int sum2 = 0;
    
    std::string data;
    std::vector<std::string> data2;

    for (auto& l : in) {
        if (l.empty()) {
            std::unordered_set<char> d;
            for (char c : data) d.insert(c);
            sum += d.size();

            std::string first = data2[0];

            for (char c : first) {
                bool found = true;
                for (std::string& s : data2) {
                    if (s.find(c) == std::string::npos) {
                        found = false;
                        break;
                    }
                }

                if (found) {
                    sum2 += 1;
                }
            }

            data.clear();
            data2.clear();
        }
        else {
            data += l;
            data2.push_back(l);
        }
    }

    a.part1 = std::to_string(sum);
    a.part2 = std::to_string(sum2);

    return a;
}
