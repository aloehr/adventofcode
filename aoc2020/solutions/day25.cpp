#include <cmath>
#include <string>
#include <unordered_map>

#include "../aoc.hpp"

int bin_exp_mod(int x, int b, int mod) {
    long long res = 1;
    int start = 31;
    while (!((b >> start) & 1)) start--;

    for (int i = start; i >= 0; --i) {
        res = (res * res) % mod;

        if ((b >> i) & 1) {
            res = (res * x) % mod;
        }
    }

    return res;
}

int baby_giant(int a) {

    int g = 7;
    int z = 20201227;
    int n = z-1;
    int m = std::ceil(std::sqrt(n));

    std::unordered_map<int, int> is;

    int r = 1;

    for (int i = 0; i < m; ++i) {
        is[r] = i;
        r = (r * g) % z;
    }

    r = bin_exp_mod(g, n-m, z);

    int j = 0;
    unsigned long long res = 0;
    unsigned long long b = 1;
    for (; j < m; ++j) {
        res = (a * b) % z;
        if (is.count(res)) {
            return j * m + is[res];
        }

        b = (b * r) % z;
    }

    return -1;
}

answer solve_day25(input& in) {

    answer a;

    int pub_key1 = std::stoi(in[0]);
    int pub_key2 = std::stoi(in[1]);

    int key_ls = baby_giant(pub_key1);

    a.part1 = std::to_string(bin_exp_mod(pub_key2, key_ls, 20201227));

    return a;
}
