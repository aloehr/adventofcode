#include <map>
#include <vector>
#include <set>
#include <algorithm>

#include "../aoc.hpp"


int adds_up_to(std::vector<unsigned long long>& data, size_t idx, unsigned long long n) {
    int r = 0;

    unsigned long long s = 0;
    for (; idx < data.size(); ++idx) {
        s += data[idx];
        r += 1;
        if (s == n) return r;

        if (s > n) return 0;
    }

    return 0;
}

bool contains(std::vector<unsigned long long>& data, unsigned long long n, unsigned long c, size_t idx) {

    for (size_t i = idx - c; i < idx - 1; ++i) {
        for (size_t j = i + 1; j < idx; ++j) {
            if (data[i] + data[j] == n) return true;
        }
    }

    return false;
}

answer solve_day09(input& in) {

    answer a;
    in.pop_back();

    std::vector<unsigned long long> data;
    data.reserve(in.size());

    for (const auto& l : in) {
        data.push_back(std::stoull(l));
    }

    unsigned long long invalid_nr = 0;
    for (size_t i = 25; i < data.size(); ++i) {
        if (!contains(data, data[i], 25, i)) {
            invalid_nr = data[i];
            break;
        }
    }

    int c = 0;
    size_t idx = 0;

    for (idx = 0; idx < data.size(); ++idx) {
        if ((c = adds_up_to(data, idx, invalid_nr))) {
            break;
        }
    }

    std::sort(data.begin() + idx, data.begin() + idx + c);

    a.part1 = std::to_string(invalid_nr);
    a.part2 = std::to_string(data[idx] + data[idx + c - 1]);

    return a;
}
