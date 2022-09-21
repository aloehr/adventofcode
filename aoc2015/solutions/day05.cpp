#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"

bool has_double_consecutive_letter(const std::string& str) {
    for (size_t i = 0; i < str.size() - 1; ++i) {
        if (str[i] == str[i+1]) {
            return true;
        }
    }

    return false;
}

bool has_forbidden_str(const std::string& str) {
    if (str.find("ab") != std::string::npos) {
        return true;
    } else if (str.find("cd") != std::string::npos) {
        return true;
    } else if (str.find("pq") != std::string::npos) {
        return true;
    } else if (str.find("xy") != std::string::npos) {
        return true;
    }

    return false;
}

bool has_required_vowel_count(const std::string& str) {
    int count = 0;
    for (auto c : str) {
        if ((c == 'a') | (c == 'e') | (c == 'i') | (c == 'o') | (c == 'u')) {
            count++;
        }

    }

    if (count >= 3) return true;
    else return false;
}

bool is_nice(const std::string& str) {
    if (!has_double_consecutive_letter(str)) {
        return false;
    } else if (has_forbidden_str(str)) {
        return false;
    } else if (!has_required_vowel_count(str)) {
        return false;
    }

    return true;
}

bool has_same_letter_with_one_inbetween(const std::string& str) {
    for (size_t i = 0; i < str.size() - 2; ++i) {
        if (str[i] == str[i + 2]) {
            return true;
        }
    }

    return false;
}
bool has_pair_twice_without_overlapp(const std::string& str) {
    for (size_t i = 0; i < str.size() - 3; ++i) {

        if (str.find(str.c_str() + i, i+2, 2) != std::string::npos) {
            return true;
        }
    }

    return false;
}

bool is_nice2(const std::string& str) {
    if (!has_same_letter_with_one_inbetween(str)) {
        return false;
    } else if (!has_pair_twice_without_overlapp(str)) {
        return false;
    }

    return true;
}

aoch::Result solve_day05(aoch::Input& in) {
    aoch::Result r;

    int nice_count = 0;
    int nice2_count = 0;

    for (const auto& str : in) {
        if (is_nice(str)) nice_count++;
        if (is_nice2(str)) nice2_count++;
    }

    r.part1 = std::to_string(nice_count);
    r.part2 = std::to_string(nice2_count);

    return r;
}
