#include <iostream>

#include "aoc.hpp"

std::vector<std::string> split(const std::string& str, char delimiter, bool keep_empty_splits) {

    std::vector<std::string> ret;

    size_t idx = 0;
    size_t cur_del_pos = 0;
    while (cur_del_pos != std::string::npos && idx < str.size()) {
        cur_del_pos = str.find(delimiter, idx);

        if (keep_empty_splits || idx != cur_del_pos) {
            ret.push_back(str.substr(idx, cur_del_pos - idx));
        }

        idx = cur_del_pos + 1;
    }

    // special case if last char is seperator or if string is empty
    if (keep_empty_splits && (str.empty() || str[str.size()-1] == delimiter))
        ret.push_back("");

    return ret;
}

template <>
sum_type<bool>::type sum<bool>(const std::vector<bool>& in) {
    unsigned int sum = 0;

    for (size_t i = 0; i < in.size(); ++i)
        sum += in[i];

    return sum;
}
