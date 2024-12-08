#include <algorithm>
#include <complex>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>


using Point = std::complex<int>;


template <>
struct std::hash<Point> {
    size_t operator() (const Point& p) const noexcept {
        return (static_cast<long long>(p.real()) << 32) | p.imag();
    }
};

aoch::Result solve_day08(aoch::Input& in) {
    aoch::Result a;

    const int n = in.size();
    const int m = in[0].size();

    auto in_bound = [n, m] (const Point& p) {
        if (p.real() < 0 || p.real() >= m) return false;
        if (p.imag() < 0 || p.imag() >= n) return false;

        return true;
    };

    std::unordered_map<char, std::vector<Point>> antennas;

    for (int y = 0; y < n; ++y) {
        for (int x = 0; x < m; ++x) {
            if (in[y][x] != '.') {
                antennas[in[y][x]].emplace_back(x, y);
            }
        }
    }

    std::unordered_set<Point> antinodes;
    std::unordered_set<Point> antinodes2;

    for (const auto& kv : antennas) {
        for (auto& p : kv.second) {
            antinodes2.insert(p);
        }
    }

    for (const auto& kv : antennas) {
        for (size_t i = 0; i < kv.second.size() - 1; ++i) {
            const auto& ant1 = kv.second[i];

            for (size_t j = i + 1; j < kv.second.size(); ++j) {
                const auto& ant2 = kv.second[j];

                Point dir = ant1 - ant2;

                Point antinode = ant1 + dir;

                if (in_bound(antinode)) {
                    antinodes.insert(antinode);
                    antinodes2.insert(antinode);
                }

                antinode += dir;

                while (in_bound(antinode)) {
                    antinodes2.insert(antinode);
                    antinode += dir;
                }

                antinode = ant2 - dir;

                if (in_bound(antinode)) {
                    antinodes.insert(antinode);
                    antinodes2.insert(antinode);
                }

                antinode-= dir;

                while (in_bound(antinode)) {
                    antinodes2.insert(antinode);
                    antinode -= dir;
                }
            }
        }
    }

    a.part1 = std::to_string(antinodes.size());
    a.part2 = std::to_string(antinodes2.size());

    return a;
}
