#include <iostream>

#include "aoc.hpp"


template <>
sum_type<bool>::type sum<bool>(const std::vector<bool>& in) {
    unsigned int sum = 0;

    for (size_t i = 0; i < in.size(); ++i)
        sum += in[i];

    return sum;
}
