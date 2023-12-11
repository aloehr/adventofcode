#include <array>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"


using Pos = std::array<int, 2>;

int distance(const Pos& g1, const Pos& g2, const std::set<size_t>& expanded_rows, const std::set<size_t>& expanded_columns, unsigned int expansion_factor = 2) {
    static std::map<Pos, int> expansion_columns_cache;
    static std::map<Pos, int> expansion_rows_cache;


    Pos sorted_column_idxs = {std::min(g1[0], g2[0]), std::max(g1[0], g2[0])};
    if (!expansion_columns_cache.count(sorted_column_idxs)) {
        int expanded_columns_count = 0;
        for (int x = sorted_column_idxs[0] + 1; x < sorted_column_idxs[1]; ++x) {
            if (expanded_columns.count(x)) expanded_columns_count++;
        }

        expansion_columns_cache[sorted_column_idxs] = expanded_columns_count;
    }

    int expanded_jumps_count = expansion_columns_cache[sorted_column_idxs];


    Pos sorted_row_idxs = {std::min(g1[1], g2[1]), std::max(g1[1], g2[1])};
    if (!expansion_rows_cache.count(sorted_row_idxs)) {
        int expanded_rows_count = 0;
        for (int y = sorted_row_idxs[0] + 1; y < sorted_row_idxs[1]; ++y) {
            if (expanded_rows.count(y)) expanded_rows_count++;
        }

        expansion_rows_cache[sorted_row_idxs] = expanded_rows_count;
    }

    expanded_jumps_count += expansion_rows_cache[sorted_row_idxs];

    return std::abs(g1[0] - g2[0]) + std::abs(g1[1] - g2[1]) + expanded_jumps_count * (expansion_factor - 1);
}

aoch::Result solve_day11(aoch::Input& in) {

    aoch::Result a;

    std::vector<Pos> galaxies;
    std::set<size_t> expanded_rows;
    std::set<size_t> expanded_columns;

    for (size_t y = 0; y < in.size(); ++y) {
        bool found_galaxie_in_row = false;

        for (size_t x = 0; x < in[y].size(); ++x) {
            if (in[y][x] == '#') {
                found_galaxie_in_row = true;
                galaxies.push_back({static_cast<int>(x), static_cast<int>(y)});
            }
        }

        if (!found_galaxie_in_row) {
            expanded_rows.insert(y);
        }
    }

    for (size_t x = 0; x < in[0].size(); ++x) {
        bool found_galaxie_in_column = false;

        for (size_t y = 0; y < in.size(); ++y) {
            if (in[y][x] == '#') {
                 found_galaxie_in_column= true;
            }
        }

        if (!found_galaxie_in_column) {
            expanded_columns.insert(x);
        }
    }

    int sum1 = 0;
    long long sum2 = 0;

    for (size_t i = 0; i < galaxies.size() - 1; ++i) {
        for (size_t j = i+1; j < galaxies.size(); ++j) {
            sum1 += distance(galaxies[i], galaxies[j], expanded_rows, expanded_columns);
            sum2 += distance(galaxies[i], galaxies[j], expanded_rows, expanded_columns, 1000000);
        }
    }

    a.part1 = std::to_string(sum1);
    a.part2 = std::to_string(sum2);

    return a;
}
