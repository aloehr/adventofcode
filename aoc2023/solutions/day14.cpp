#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"


enum DIR {
    NORTH, EAST, SOUTH, WEST
};

void tilt(std::vector<std::string>& data, DIR d) {
    if (d == DIR::NORTH) {
        for (size_t x = 0; x < data[0].size(); ++x) {
            unsigned int rock_pillar_count = 0;
            for (size_t y = data.size() - 1; y < data.size(); --y) {
                if (data[y][x] == 'O') {
                    rock_pillar_count++;
                } else if(data[y][x] == '#') {
                    rock_pillar_count = 0;
                } else if (rock_pillar_count){
                    data[y][x] = 'O';
                    data[y + rock_pillar_count][x] = '.';
                }
            }
        }
    } else if (d == DIR::EAST) {
        for (size_t y = 0; y < data.size(); ++y) {
            unsigned int rock_pillar_count = 0;
            for (size_t x = 0; x < data[0].size(); ++x) {
                if (data[y][x] == 'O') {
                    rock_pillar_count++;
                } else if(data[y][x] == '#') {
                    rock_pillar_count = 0;
                } else if (rock_pillar_count) {
                    data[y][x] = 'O';
                    data[y][x - rock_pillar_count] = '.';
                }
            }
        }
    } else if (d == DIR::SOUTH) {
        for (size_t x = 0; x < data[0].size(); ++x) {
            unsigned int rock_pillar_count = 0;
            for (size_t y = 0; y < data.size(); ++y) {
                if (data[y][x] == 'O') {
                    rock_pillar_count++;
                } else if(data[y][x] == '#') {
                    rock_pillar_count = 0;
                } else if (rock_pillar_count) {
                    data[y][x] = 'O';
                    data[y - rock_pillar_count][x] = '.';
                }
            }
        }
    } else if (d == DIR::WEST) {
        for (size_t y = 0; y < data.size(); ++y) {
            unsigned int rock_pillar_count = 0;
            for (size_t x = data[0].size() - 1; x < data[0].size(); --x) {
                if (data[y][x] == 'O') {
                    rock_pillar_count++;
                } else if(data[y][x] == '#') {
                    rock_pillar_count = 0;
                } else if (rock_pillar_count) {
                    data[y][x] = 'O';
                    data[y][x + rock_pillar_count] = '.';
                }
            }
        }
    }
}

int calc_score(const std::vector<std::string>& data) {
    int total_score = 0;
    for (size_t y = 0; y < data.size(); ++y) {
        for (auto c : data[y]) {
            if (c == 'O') {
                total_score += data.size() - y;
            }
        }
    }

    return total_score;
}

void update_key(const std::vector<std::string>& data, std::string& key) {
    key.clear();

    for (const auto& l : data) {
        key += l;
    }
}

aoch::Result solve_day14(aoch::Input& in) {
    aoch::Result a;

    std::map<std::string, size_t> data_to_id;
    std::map<size_t, std::pair<std::vector<std::string>, unsigned int>> id_to_data;
    std::string key(in.size() * in[0].size(), 'X');
    unsigned int cycle_count = 0;

    auto store_cycle_data = [&data_to_id, &id_to_data, &in](const std::string& key, unsigned int cycle_count) {
        static unsigned int next_free_id = 0;
        data_to_id[key] = next_free_id;
        id_to_data[next_free_id++] = std::make_pair(in, cycle_count);
    };

    update_key(in, key);
    store_cycle_data(key, cycle_count);

    tilt(in, NORTH);

    a.part1 = std::to_string(calc_score(in));


    // part 2
    tilt(in, WEST);
    tilt(in, SOUTH);
    tilt(in, EAST);
    cycle_count++;

    update_key(in, key);
    store_cycle_data(key, cycle_count);

    unsigned int max_cycles = 1e9;

    while (cycle_count < max_cycles) {
        tilt(in, NORTH);
        tilt(in, WEST);
        tilt(in, SOUTH);
        tilt(in, EAST);
        cycle_count++;

        update_key(in, key);

        // we found the cycle if we are in here
        if (data_to_id.count(key)) {
            size_t id = data_to_id[key];
            auto& pair = id_to_data[id];

            unsigned int period = cycle_count - pair.second;

            if (max_cycles - cycle_count > period)
                cycle_count += period * ((max_cycles - cycle_count) / period);

            a.part2 = std::to_string(calc_score(id_to_data[id + max_cycles - cycle_count].first));
            break;
        } else {
            store_cycle_data(key, cycle_count);
        }
    }

    return a;
}
