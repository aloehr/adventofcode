#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"


void increment_pw(std::string& pw) {

    bool carry = false;

    for (size_t i = pw.size()-1; i < pw.size(); --i) {

        char old_c = pw[i];

        pw[i] = ((pw[i] - 'a' + 1) % 26) + 'a';
        carry = old_c == 'z';

        if (!carry) break;
    }

    if (carry) {
        std::cout << "new pw would have exceeded 8 letters..." << std::endl;
    }

    return;
}

bool has_increasing_straight(const std::string& pw) {
    for (size_t i = 0; i < pw.size() - 2; ++i) {
        if (pw[i+1] == pw[i+0] + 1 && pw[i+2] == pw[i+1] + 1) {
            return true;
        }
    }

    return false;
}

bool has_forbidden_chars(const std::string& pw) {
    for (auto c : pw) {
        if (c == 'i' || c == 'l' || c == 'o') return true;
    }

    return false;
}

bool has_two_letter_pairs(const std::string& pw) {
    bool found_first_pair = false;
    char first_pair_char = 'X';

    size_t i = 0;

    while (i < pw.size() - 1) {
        if (pw[i] == pw[i+1]) {
            if (!found_first_pair) {
                found_first_pair = true;
                first_pair_char = pw[i];
                i += 2;
            } else {
                if (pw[i] != first_pair_char) {
                    return true;
                } else {
                    i += 2;
                }
            }
        } else {
            i++;
        }
    }

    return false;
}

bool pw_ok(const std::string& pw) {

    if (!has_increasing_straight(pw)) {
        return false;
    } else if (has_forbidden_chars(pw)) {
        return false;
    } else if (!has_two_letter_pairs(pw)) {
        return false;
    }

    return true;
}

aoch::Result solve_day11(aoch::Input& in) {
    aoch::Result r;

    std::string pw_candidate = in[0];
    increment_pw(pw_candidate);

    while (!pw_ok(pw_candidate)) {
        increment_pw(pw_candidate);
    }


    r.part1 = pw_candidate;

    // part 2

    increment_pw(pw_candidate);

    while (!pw_ok(pw_candidate)){
        increment_pw(pw_candidate);
    }

    r.part2 = pw_candidate;

    return r;
}
