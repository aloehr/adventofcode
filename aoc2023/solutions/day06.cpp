#include <cmath>
#include <iostream>
#include <numeric>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


long long calc(double total_time, double record_distance) {
    double total_time_halved = total_time / 2.0;

    double sqrt_result = std::sqrt(total_time_halved * total_time_halved - record_distance);

    long long min_time_for_win = std::floor(total_time_halved - sqrt_result + 1);
    long long max_time_for_win = std::ceil(total_time_halved + sqrt_result - 1);

    return max_time_for_win - min_time_for_win + 1;
}

aoch::Result solve_day06(aoch::Input& in) {
    aoch::Result a;

    auto time_strs = aoch::split(aoch::split(in[0], ':')[1]);
    auto distance_strs = aoch::split(aoch::split(in[1], ':')[1]);

    int result = 1;
    for (uint i = 0; i < time_strs.size(); ++i) {
        int total_time = std::stoi(time_strs[i]);
        int record_distance = std::stoi(distance_strs[i]);
        result *= calc(total_time, record_distance);
    }

    a.part1 = std::to_string(result);


    // part 2
    auto time = std::stoll(std::reduce(time_strs.begin(), time_strs.end()));
    auto distance = std::stoll(std::reduce(distance_strs.begin(), distance_strs.end()));

    a.part2 = std::to_string(calc(time, distance));

    return a;
}
