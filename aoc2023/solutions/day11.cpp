#include <numeric>
#include <iostream>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"


long long calc_distances(const std::vector<unsigned int>& coords, unsigned int expansion_factor = 2) {
    long long sum = 0;

    unsigned int visited_galaxies_count = 0;
    unsigned int unvisited_galaxies_count = std::accumulate(coords.cbegin(), coords.cend(), 0);
    unsigned int cur_expansions_count = 0;

    // find the first coord with galaxies as the start of our algorithm
    size_t cur_coord = 0;
    while (cur_coord < coords.size() && coords[cur_coord] == 0) {
        cur_coord++;
    }

    for (size_t next_coord = cur_coord + 1; next_coord < coords.size(); ++next_coord) {

        // this means there were no galaxie in the row/column, so increase the expansion_count by 1 for this cooord
        if (coords[next_coord] == 0) {
            cur_expansions_count++;
            continue;
        }

        // adjust visited / unvisited galaxies counts
        visited_galaxies_count += coords[cur_coord];
        unvisited_galaxies_count -= coords[cur_coord];

        // visited_galaxies_count * unvisited_galaxies_count is the amount the segment between cur_coord and next_coord
        // needs to be added to the sum
        sum += visited_galaxies_count * unvisited_galaxies_count * (next_coord - cur_coord + cur_expansions_count * (expansion_factor - 1));

        cur_coord = next_coord;
        cur_expansions_count = 0;
    }

    return sum;
}

aoch::Result solve_day11(aoch::Input& in) {
    aoch::Result a;

    std::vector<unsigned int> galaxies_x_coords(in[0].size(), 0);
    std::vector<unsigned int> galaxies_y_coords(in.size(), 0);

    for (size_t y = 0; y < in.size(); ++y) {
        for (size_t x = 0; x < in[y].size(); ++x) {
            if (in[y][x] == '#') {
                galaxies_x_coords[x]++;
                galaxies_y_coords[y]++;
            }
        }
    }

    long long sum1 = calc_distances(galaxies_x_coords);
    sum1 += calc_distances(galaxies_y_coords);

    long long sum2 = calc_distances(galaxies_x_coords, 1e6);
    sum2 += calc_distances(galaxies_y_coords, 1e6);

    a.part1 = std::to_string(sum1);
    a.part2 = std::to_string(sum2);

    return a;
}
