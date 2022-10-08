#include <algorithm>
#include <iostream>
#include <string>
#include <set>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/math.hpp"


// @TODO This solution needs major refactoring...

void xxx(const std::vector<int>& nums, int items_to_pick, int goal, std::vector<std::vector<int>>& out, std::vector<int> current = {}, size_t idx = 0) {

    if (items_to_pick == 0) {
        if (aoch::sum(current) == goal) {
            out.push_back(current);
        }

        return;
    }

    for (size_t i = idx; i < nums.size() - items_to_pick + 1; ++i) {
        current.push_back(nums[i]);
        xxx(nums, items_to_pick - 1, goal, out, current, i + 1);
        current.pop_back();
    }

    return;
}

bool xxx2(const std::vector<int>& nums, int items_to_pick, int goal, int current = 0, size_t idx = 0) {
    if (current == goal) return true;
    if (current > goal) return false;
    if (items_to_pick == 0) {
        if (current == goal) return true;
        else return false;
    }

    for (size_t i = idx; i < nums.size() - items_to_pick + 1; ++i) {
        if (current + nums[i] > goal) continue;

        bool ret = xxx2(nums, items_to_pick - 1, goal, current + nums[i], i + 1);

        if (ret == true) return true;
    }

    return false;
}

long long solve(const std::vector<int>& numbers) {

    int total = aoch::sum(numbers);

    std::vector<std::vector<int>> results;

    for (size_t i = 1; i <= numbers.size(); ++i) {
        xxx(numbers, i, total/3, results);
        if (results.size()) {
            break;
        }
    }

    std::sort(results.begin(), results.end(), [] (std::vector<int> a, std::vector<int> b) {
        long long  a_prod = 1;
        for (const auto n : a) a_prod *= n;

        long long  b_prod = 1;
        for (const auto n : b) b_prod *= n;

        return a_prod < b_prod;
    });

    for (const auto& nums : results) {
        std::set<int> xxxx(nums.begin(), nums.end());
        std::vector<int> tmp;
        for (auto n : numbers) if (xxxx.count(n) == 0) tmp.push_back(n);

        for (size_t i = 1; i <= tmp.size(); ++i) {
            if (xxx2(tmp, i, total/3)) {
                long long x = 1;
                for (auto n : nums) x *= n;

                return x;
            }
        }
    }

    return -1;
}

aoch::Result solve_day24(aoch::Input& in) {
    aoch::Result r;

    std::vector<int> numbers;
    numbers.reserve(in.size());

    for (auto str : in) {
        numbers.push_back(std::stoi(str));
    }

    r.part1 = std::to_string(solve(numbers));

    int total = aoch::sum(numbers);
    int goal = total / 4;

    std::vector<std::vector<int>> results;

    for (size_t i = 1; i <= numbers.size(); ++i) {
        xxx(numbers, i, goal, results);
        if (results.size()) break;
    }

    std::sort(results.begin(), results.end(), [] (std::vector<int> a, std::vector<int> b) {
        long long  a_prod = 1;
        for (const auto n : a) a_prod *= n;

        long long  b_prod = 1;
        for (const auto n : b) b_prod *= n;

        return a_prod < b_prod;

    });

    for (auto& num : results) {
        std::set<int> x(num.begin(), num.end());
        std::vector<int> xx;
        for (auto n : numbers) if (x.count(n) == 0) xx.push_back(n);

        long long  s = solve(xx);

        if (s != -1) {
            long long xxxxx = 1;
            for (auto n : num) xxxxx *= n;
            r.part2 = std::to_string(xxxxx);
            break;
        }
    }

    return r;
}
