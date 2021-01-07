#include <vector>
#include <string>
#include <unordered_map>

#include "../aoc.hpp"


unsigned int count_orbits(const std::vector<unsigned int>& orbits, unsigned int obj_id, std::vector<unsigned int>& memo) {

    static std::vector<unsigned int> buffer;

    if (memo[obj_id] != memo.size()) {
        return memo[obj_id];
    }

    unsigned int count = 0;

    while (obj_id != orbits.size()) {
        if (memo[obj_id] != orbits.size()) {
            count = memo[obj_id]+1;
            break;
        }
        buffer.push_back(obj_id);
        obj_id = orbits[obj_id];
    }

    for (size_t i = buffer.size(); i > 1; --i) {
        memo[buffer[i-1]] = count++;
    }

    memo[buffer[0]] = count;
    buffer.clear();

    return count;
}

answer solve_day06(input& in) {

    // parsing
    answer a;
    in.pop_back();

    std::unordered_map<std::string, unsigned int> obj_ids;
    std::vector<unsigned int> orbits(in.size() + 1, in.size() + 1);
    std::vector<unsigned int> memo(orbits.size(), orbits.size());

    unsigned int next_id = 0;

    for (const auto& l : in) {


        size_t delim = l.find(')');
        std::string obj_a = l.substr(0, delim);
        if (!obj_ids.count(obj_a)) {
            obj_ids[obj_a] = next_id++;
        }

        std::string obj_b = l.substr(delim+1);
        if (!obj_ids.count(obj_b)) {
            obj_ids[obj_b] = next_id++;
        }

        orbits[obj_ids[obj_b]] = obj_ids[obj_a];
    }

    // part 1
    unsigned int total = 0;
    for (size_t i = 0; i < orbits.size(); ++i) {
        total += count_orbits(orbits, i, memo);
    }

    a.part1 = std::to_string(total);


    // part 2
    std::vector<unsigned int> travel_dist(orbits.size(), orbits.size());


    unsigned int cur_orbit = orbits[obj_ids["YOU"]];
    unsigned int dist = 0;

    while (cur_orbit != orbits.size()) {
        travel_dist[cur_orbit] = dist++;
        cur_orbit = orbits[cur_orbit];
    }

    cur_orbit = orbits[obj_ids["SAN"]];

    dist = 0;

    while (travel_dist[cur_orbit] == orbits.size()) {
        dist++;
        cur_orbit = orbits[cur_orbit];
    }

    a.part2 = std::to_string(dist + travel_dist[cur_orbit]);

    return a;
}
