#include <algorithm>
#include <string>
#include <vector>

#include "../aoc.hpp"


bool has_same_digit_adjacent(const std::string& pw, bool part2_rule = false) {

    char repeated_c = 'a';
    bool not_double = false;
    for (size_t i = 0; i < pw.size() - 1; ++i) {
        if (not_double && part2_rule) {
            if (pw[i] == repeated_c) continue;

            not_double = false;
        }
        if (pw[i] == pw[i+1]) {
            if (i+1 == pw.size() - 1) return true;
            else if (pw[i+2] != pw[i]) {
               return true; 
            }
            else {
                if (part2_rule) {
                    not_double = true;
                    repeated_c = pw[i];
                }
            }
        }
    }

    return false;
}

bool is_never_decreasing(const std::string& pw) {
    for (size_t i = 0; i < pw.size() - 1; ++i) {
        if (pw[i] > pw[i+1]) return false;
    }

    return true;
}


bool is_valid_pw(int pw, bool part2_rule = false) {

    std::string pw_str = std::to_string(pw);
    if (!has_same_digit_adjacent(pw_str, part2_rule)) return false;
    else if (!is_never_decreasing(pw_str)) return false;

    return true;
}

answer solve_day04(input& in) {

    answer a;

    size_t delim = 0;
    int low  = std::stoi(in[0], &delim); //254032;
    int high = std::stoi(in[0].substr(delim+1)); //789860;

    int valid_pw_count = 0;
    int valid_pw_count2 = 0;

    for (int i = low; i <= high; ++i) {
        if (is_valid_pw(i)) valid_pw_count++;        
        if (is_valid_pw(i, true)) valid_pw_count2++;        
    }

    a.part1 = std::to_string(valid_pw_count);
    a.part2 = std::to_string(valid_pw_count2);

    return a;
}
