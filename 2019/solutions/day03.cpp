#include <map>
#include <vector>
//#include <algorithm>
#include <cassert>

#include "../aoc.hpp"


enum class dir : int {
    up = 0,
    right = 1,
    down = 2,
    left = 3, 
    unknown = 4,
};

dir char2dir(char c) {
    switch(c) {
        case 'U': return dir::up;
        case 'R': return dir::right;
        case 'D': return dir::down;
        case 'L': return dir::left;
        default: return dir::unknown;
    };
}

char dir2char(dir d) {
    switch (d) {
        case dir::up: return 'U';
        case dir::right: return 'R';
        case dir::down: return 'D';
        case dir::left: return 'L';
        default: return '?';
    };
}

struct relative_path {
    dir d = dir::unknown;
    int l = 0;

    std::string to_string() {
        return "<" + std::string(1, dir2char(this->d)) + " " + std::to_string(this->l) + ">";
    }
};

int manhattan_distance(std::pair<int, int> p) {
    return std::abs(p.first) + std::abs(p.second);
}

int count_steps(std::pair<int, int> goal, const std::vector<relative_path>& wp) {

    int steps = 0;
    auto cur_loc = std::make_pair(0, 0);

    while (true) {
        for (const auto& rp : wp) {
            for (int i = 0; i < rp.l; ++i) {
                switch (rp.d) {
                    case dir::up:
                        cur_loc.second++;
                        break;
                    case dir::right:
                        cur_loc.first++;
                        break;
                    case dir::down:
                        cur_loc.second--;
                        break;
                    case dir::left:
                        cur_loc.first--;
                        break;
                    default:
                        assert(false);
                        break;
                }

                steps++;

                if (cur_loc == goal)
                    return steps;
            }
        }
    }
}

answer solve_day03(input& in) {

    answer a;
    in.pop_back();

    std::vector<std::vector<relative_path>> wire_pathes;

    for (auto l : in) {

        std::vector<relative_path> wire_path;
        size_t ptr = 0;
        while (ptr != l.size()) {
            
            if (l[ptr] == ',') ptr++;
            if (ptr) l = l.substr(ptr);
            relative_path cur;
            cur.d = char2dir(l[0]);
            l = l.substr(1);
            cur.l = std::stoi(l, &ptr);

            wire_path.push_back(cur);
        } 
        wire_pathes.push_back(wire_path);
    }

    std::map<std::pair<int, int>, int> map;
    int mask = 0;

    assert(wire_pathes.size() == 2); 

    for (auto& p : wire_pathes) {
        mask = mask ? 2 : 1; 

        auto cur_coords = std::make_pair(0, 0);
        for (auto& rp : p) {
            for (int i = 0; i < rp.l; ++i) {
                switch (rp.d) {
                    case dir::up:
                        cur_coords.second++;
                        break;
                    case dir::right:
                        cur_coords.first++;
                        break;
                    case dir::down:
                        cur_coords.second--;
                        break;
                    case dir::left:
                        cur_coords.first--;
                        break;
                    default:
                        assert(false);
                        break;
                }

                if (map.count(cur_coords) && ((map[cur_coords] & mask) ^ mask))
                    map[cur_coords] |= mask;
                else
                    map[cur_coords] = mask;
            }
        }
    }

    int closest_distance = -1;
    for (auto& p : map) {
        if (p.second == 3) {

            if (closest_distance < 0) 
                closest_distance = manhattan_distance(p.first);
            else 
                closest_distance = std::min(closest_distance, manhattan_distance(p.first));
        }
    }

    int needed_steps = -1;
    for (auto& p : map) {
        if (p.second == 3) {

            if (needed_steps < 0) 
                needed_steps = count_steps(p.first, wire_pathes[0]) + count_steps(p.first, wire_pathes[1]);
            else 
                needed_steps = std::min(needed_steps, count_steps(p.first, wire_pathes[0]) + count_steps(p.first, wire_pathes[1]));
        }
    }
    
    a.part1 = std::to_string(closest_distance);
    a.part2 = std::to_string(needed_steps);

    return a;
}
