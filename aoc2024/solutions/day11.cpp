#include <iostream>
#include <string>
#include <unordered_map>

#include <aoch/AOCSolutionTypes.hpp>
#include <aoch/string.hpp>


aoch::Result solve_day11(aoch::Input& in) {
    aoch::Result a;

    std::unordered_map<long long, long long> stones;

    auto tokens = aoch::split(in[0]);

    for (auto& t : tokens) stones[std::stol(t)]++;


    auto count_stones = [&stones] () {
        long long count = 0;

        for (const auto& kv : stones) {
            count += kv.second;
        }

        return count;
    };

    std::unordered_map<long long, long long> next;

    int blink_count = 0;

    while (blink_count < 75) {

        // std::cout << c  << " " << stones.size() << std::endl;

        for (const auto& kv : stones) {
            if (kv.first == 0) {
                next[1] += kv.second;
                continue;
            }

            std::string x = std::to_string(kv.first);
            if (x.size() % 2 == 0) {
                auto a = x.substr(0, x.size() / 2);
                auto b = x.substr(x.size() / 2);

                next[std::stol(a)] += kv.second;
                next[std::stol(b)] += kv.second;
                continue;
            }

            next[kv.first * 2024] += kv.second;
        }

        std::swap(stones, next);
        next.clear();

        blink_count++;

        if (blink_count == 25) {
            a.part1 = std::to_string(count_stones());
        }
    }

    a.part2 = std::to_string(count_stones());

    return a;
}
