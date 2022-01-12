#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

#include "../aoc.hpp"


answer solve_day10(input& in) {

    answer a;
    in.pop_back();

    std::vector<int> data;
    data.reserve(in.size());
    
    for (const auto& l : in) {
        data.push_back(std::stoi(l));
    }

    std::sort(data.begin(), data.end());

    data.push_back(data.back() + 3);
    int c = 0;

    int c1 = 0;
    int c2 = 0;
    int c3 = 0;

    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] - c == 1) c1++;
        else if (data[i] - c == 3) c3++;
        else if (data[i] - c == 2) c2++;
        else {
            std::cout << "unexpected joltage difference... " << (data[i] - c) << std::endl;

        }

        c = data[i];
    }

    std::vector<unsigned long long> dp(data.size(), 0);
    dp[0] = 1;
    dp[1] = data[1] <= 3 ? 2 : 1;
    dp[2] = dp[1] + (data[2] - data[0] <= 3) + (data[2] <= 3);

    for (size_t i = 3; i < dp.size(); ++i) {
        dp[i] = dp[i-1];

        if (data[i] - data[i-2] <= 3) dp[i] += dp[i-2];
        if (data[i] - data[i-3] <= 3) dp[i] += dp[i-3];
    }

    //for (auto x : data) std::cout << x << "\t";
    //std::cout << '\n';

    //for (auto x : dp) std::cout << x << "\t";
    //std::cout << '\n';

    a.part1 = std::to_string(c1 * c3);
    a.part2 = std::to_string(dp.back());

    return a;
}
