#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>


template <>
struct std::hash<std::pair<int, int>> {
    size_t operator() (const std::pair<int, int>& p) const noexcept {
        return (static_cast<long long>(p.first) << 32) | p.second;
    }
};

aoch::Result solve_day08(aoch::Input& in) {
    aoch::Result a;

    const int n = in.size();
    const int m = in[0].size();

    auto in_bound = [n, m] (int x_pos, int y_pos) {
        if (x_pos < 0 || x_pos >= m) return false;
        if (y_pos < 0 || y_pos >= n) return false;

        return true;
    };

    std::unordered_map<char, std::vector<std::pair<int, int>>> antennas;

    for (size_t y = 0; y < n; ++y) {
        for (size_t x = 0; x < m; ++x) {
            if (in[y][x] != '.') {
                antennas[in[y][x]].emplace_back(x, y);
            }
        }
    }

    std::unordered_set<std::pair<int, int>> antinodes;

    for (const auto& kv : antennas) {
        for (size_t i = 0; i < kv.second.size() - 1; ++i) {
            const auto& ant1 = kv.second[i];

            for (size_t j = i + 1; j < kv.second.size(); ++j) {
                const auto& ant2 = kv.second[j];

                int x_dir = ant1.first - ant2.first;
                int y_dir = ant1.second - ant2.second;

                int antinode_x = ant1.first + x_dir;
                int antinode_y = ant1.second + y_dir;

                if (in_bound(antinode_x, antinode_y)) {
                    antinodes.insert({antinode_x, antinode_y});
                }

                antinode_x = ant2.first - x_dir;
                antinode_y = ant2.second - y_dir;

                if (in_bound(antinode_x, antinode_y)) {
                    antinodes.insert({antinode_x, antinode_y});
                }
            }
        }
    }

    a.part1 = std::to_string(antinodes.size());


    // part 2
    std::unordered_set<std::pair<int, int>> antinodes2;

    for (const auto& kv : antennas) {
        for (size_t i = 0; i < kv.second.size() - 1; ++i) {
            const auto& ant1 = kv.second[i];

            for (size_t j = i + 1; j < kv.second.size(); ++j) {
                const auto& ant2 = kv.second[j];

                int x_dir = ant1.first - ant2.first;
                int y_dir = ant1.second - ant2.second;

                int antinode_x = ant1.first;
                int antinode_y = ant1.second;


                while (in_bound(antinode_x, antinode_y)) {
                    antinodes2.insert({antinode_x, antinode_y});

                    antinode_x += x_dir;
                    antinode_y += y_dir;
                }

                antinode_x = ant2.first;
                antinode_y = ant2.second;

                while (in_bound(antinode_x, antinode_y)) {
                    antinodes2.insert({antinode_x, antinode_y});

                    antinode_x -= x_dir;
                    antinode_y -= y_dir;
                }
            }
        }
    }

    a.part2 = std::to_string(antinodes2.size());

    return a;
}
