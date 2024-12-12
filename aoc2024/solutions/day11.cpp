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

    auto digit_count = [] (long long n) {
        int c = 1;

        while (n > 9) {
            c++;
            n /= 10;
        }

        return c;
    };

    std::unordered_map<long long, long long> next;

    int blink_count = 0;

    while (blink_count < 75) {

        for (const auto& kv : stones) {
            if (kv.first == 0) {
                next[1] += kv.second;
                continue;
            }

            int dc = digit_count(kv.first);

            if (dc % 2 == 0) {
                dc /= 2;

                long long a = kv.first;
                long long b = 0;
                long long multi = 1;

                while (dc) {
                    b += multi * (a % 10);
                    a /= 10;
                    multi *= 10;
                    dc--;
                }

                next[a] += kv.second;
                next[b] += kv.second;
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
