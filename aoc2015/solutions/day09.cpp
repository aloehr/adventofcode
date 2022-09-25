#include <array>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


void sepa(std::vector<size_t>& in) {
    size_t key_field = in.size();

    for (size_t i = in.size() - 1; i > 0; --i) {
        if (in[i-1] < in[i]) {
            key_field = i-1;
            break;
        }
    }

    // we are at last permutation (reverse order)
    if (key_field == in.size()) return;

    size_t smallest_in_tail_bigger_than_key = 0;
    for (size_t i = in.size(); i > 0; --i) {
        // because the tail is sorted, the first from the right bigger than the key is the smallest one
        if (in[i-1] > in[key_field]) {
            smallest_in_tail_bigger_than_key = i-1;
            break;
        }
    }

    std::swap(in[key_field], in[smallest_in_tail_bigger_than_key]);

    // sort the tail (because tail still in reverse sorted order swapping is sufficient)
    for (size_t i = 0; i < (in.size() - key_field - 1)/2; ++i) {
        std::swap(in[key_field + 1 + i], in[in.size() - 1 - i]);
    }
}

int route_distance(const std::vector<size_t>& r, std::map<std::array<size_t, 2>, int>& d) {

    int total_distance = 0;

    for (size_t i = 0; i < r.size()-1; ++i) {
        size_t first_id = std::min(r[i], r[i+1]);
        size_t second_id = std::max(r[i], r[i+1]);

        total_distance += d[{first_id, second_id}];
    }

    return total_distance;
}

aoch::Result solve_day09(aoch::Input& in) {
    aoch::Result r;

    std::map<std::string, size_t> location_ids;
    std::map<std::array<size_t, 2>, int> distances;
    std::vector<std::string> locations;

    size_t next_id = 0;

    for (auto& l : in) {
        auto tokens = aoch::split(l);

        if (!location_ids.count(tokens[0])) {
            location_ids[tokens[0]] = next_id++;
        }

        if (!location_ids.count(tokens[2])) {
            location_ids[tokens[2]] = next_id++;
        }

        size_t first_id = std::min(location_ids[tokens[0]], location_ids[tokens[2]]);
        size_t second_id = std::max(location_ids[tokens[0]], location_ids[tokens[2]]);
        distances[{first_id, second_id}] = std::stoi(tokens[4]);
    }

    // inital route
    std::vector<size_t> route;
    for (size_t i = 0; i < location_ids.size(); ++i) {
        route.push_back(i);
    }

    auto last_route = route;
    int shortest_path = std::numeric_limits<int>::max();
    int longest_path = 0;

    do {

        int distance_current_route = route_distance(route, distances);
        shortest_path = std::min(distance_current_route, shortest_path);
        longest_path = std::max(distance_current_route, longest_path);

        last_route = route;
        sepa(route);

    } while (route != last_route);

    r.part1 = std::to_string(shortest_path);
    r.part2 = std::to_string(longest_path);

    return r;
}
