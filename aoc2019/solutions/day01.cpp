#include <vector>
#include <algorithm>

#include "../aoc.hpp"


int required_fuel(int mass) {
    return mass / 3 - 2;
}

int needed_fuel(const std::vector<int>& modules) {
    int sum = 0;

    for (const auto mass : modules) {
        sum += required_fuel(mass);
    }

    return sum;
}

int needed_fuel2(const std::vector<int>& modules) {
    int sum = 0;

    for (const auto mass: modules) {
        int fuel_for_mod = std::max(required_fuel(mass), 0);
        sum += fuel_for_mod;

        int fuel_for_fuel = std::max(required_fuel(fuel_for_mod), 0);
        
        do {
            sum += fuel_for_fuel;
            fuel_for_fuel = std::max(required_fuel(fuel_for_fuel), 0);
        } while (fuel_for_fuel > 0);

    }

    return sum;
}

answer solve_day01(input& in) {

    answer a;
    in.pop_back();

    std::vector<int> modules;
    modules.reserve(in.size());

    for (const auto& l : in) {
        modules.push_back(std::stoi(l));
    }

    a.part1 = std::to_string(needed_fuel(modules));
    a.part2 = std::to_string(needed_fuel2(modules));

    return a;
}
