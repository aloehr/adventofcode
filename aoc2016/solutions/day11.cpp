#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


enum class ItemType {CHIP, GEN};

struct Item {
    ItemType t;
    size_t material_id;
    size_t floor;
};

struct State {
    std::vector<Item> items;
    size_t elevator;
    int steps;
    int heuristic;

    // this should be a consistent heuristic
    void update_heuristic() {
        heuristic = steps;

        int item_count = 0;
        for (size_t i = 0; i < 3; ++i) {
            for (const auto& it : items) {
                if (it.floor == i) {
                    item_count++;
                }
            }

            if (item_count && i < elevator) {
                heuristic += 1;
            }

            if (item_count) {
                heuristic += item_count < 3 ? 1 : 3 + (item_count - 3) * 2;
            }
        }
    }

    // we want the smallest item on top of the pq and not the biggest
    bool operator<(const State& o) const {
        return heuristic > o.heuristic;
    }
};

bool is_valid_state(const State& s) {

    // the elevator needs at least one item on his floor so he can operate.
    // (this should actually never happen unless it is in the initial state because on next state creation
    // we always move at least one item with us)
    bool elevator_floor_empty = true;
    for (const auto& i : s.items) {
        if (i.floor == s.elevator) {
            elevator_floor_empty = false;
            break;
        }
    }

    if (elevator_floor_empty) return false;

    for (const auto& i : s.items) {
        if (i.t == ItemType::CHIP) {
            bool chip_ok = false;

            // 1. it has its gen on the floor
            for (const auto& i2 : s.items) {
                if (i2.t == ItemType::GEN
                    && i2.material_id == i.material_id
                    && i2.floor == i.floor
                ) {
                    chip_ok = true;
                    break;
                }
            }

            if (chip_ok) continue;

            // 2. the current chip's generator counterpart is not on this floor, so
            // there better be no other gen on this floor or otherwise this state is invalid.
            for (const auto& i2 : s.items) {
                if (i2.t == ItemType::GEN && i2.floor == i.floor) {
                    return false;
                }
            }
        }
    }

    return true;
}

// Because gen, chip pairs are all exchangable and you will still have the same state in terms of distance to
// the end state, the hash makes sure that even with any combination of swapped pairs your get the same hash.
// This is probably the most important optimization for this problem.
std::string hash_state(const State& s) {
    std::vector<std::pair<size_t, std::vector<size_t>>> pair_pos(s.items.size() / 2);

    for (const auto& i : s.items) {

        pair_pos[i.material_id].second.push_back(i.floor);
        pair_pos[i.material_id].first = i.material_id;

        if (pair_pos[i.material_id].second.size() == 2) {
            if (pair_pos[i.material_id].second[0] > pair_pos[i.material_id].second[1]) {
                std::swap(pair_pos[i.material_id].second[0], pair_pos[i.material_id].second[1]);
            }
        }
    }

    auto& sorted_mats = pair_pos;

    std::sort(sorted_mats.begin(), sorted_mats.end(), [] (const auto& a, const auto& b) {
        if (a.second[0] != b.second[0]) return a.second[0] < b.second[0];
        return a.second[1] < b.second[1];
    });

    std::vector<char> mat_to_let(sorted_mats.size(), 'Z');

    char next_char = 'A';

    for (const auto& i : sorted_mats) {
        mat_to_let[i.first] = next_char++;
    }

    std::string hash;

    for (size_t i = 0; i < 4; ++i) {
        std::vector<Item> solos;
        int pairs = 0;

        for (size_t j = 0; j < s.items.size(); ++j) {
            if (s.items[j].floor != i) continue;

            bool found_pair = false;

            for (size_t k = 0; k < s.items.size(); ++k) {
                if (s.items[k].floor != i) continue;
                if (j == k) continue;

                if (s.items[k].material_id == s.items[j].material_id) {
                    if (s.items[j].t == ItemType::CHIP) {
                        pairs++;
                    }

                    found_pair = true;
                    break;
                }
            }

            if (!found_pair) {
                solos.push_back(s.items[j]);
            }
        }

        std::sort(solos.begin(), solos.end(), [&] (const auto& a, const auto& b) {
            return mat_to_let[a.material_id] < mat_to_let[b.material_id];
        });

        if (pairs) {
            hash += std::to_string(pairs);
        }

        for (const auto& it : solos) {
            hash += (it.t == ItemType::CHIP ? "C" : "G") +
                    std::string(1, mat_to_let[it.material_id]);
        }

        hash += ";";
    }

    hash += std::to_string(s.elevator);

    return hash;
}

bool is_end_state(const State& s, size_t last_floor) {

    if (s.elevator != last_floor) {
        return false;
    }

    for (const auto& item : s.items) {
        if (item.floor != last_floor) {
            return false;
        }
    }

    return true;
}

// a* search with pruning, the heuristic is consistent
int solve(State init_state, size_t last_floor, int steps = 0) {
    static std::unordered_set<std::string> mem;

    if (steps == 0) {
        mem.clear();

        init_state.update_heuristic();
        mem.insert(hash_state(init_state));
    }

    std::priority_queue<State> pq;
    pq.push(init_state);

    while (pq.size()) {
        auto s = pq.top();
        pq.pop();

        if (is_end_state(s, last_floor)) {
            return s.steps;
        }

        s.steps++;
        for (int m = 1; m >= -1; m -= 2) {

            // can't move below first floor or above last floor
            if ((m ==  1 && s.elevator == last_floor) ||
                (m == -1 && s.elevator == 0)) {
                continue;
            }

            // don't move down if all floors below you are empty
            if (m == -1) {
                bool all_floors_below_empty = true;
                for (const auto& it : s.items) {
                    if (it.floor < s.elevator) {
                        all_floors_below_empty = false;
                        break;
                    }
                }

                if (all_floors_below_empty) continue;
            }

            for (size_t i = 0; i < s.items.size(); ++i) {
                if (s.items[i].floor != s.elevator) continue;

                for (size_t j = i+1; j <= s.items.size(); ++j) {
                    if (j < s.items.size() && s.items[j].floor != s.elevator) continue;

                    // don't move 2 items back down, only 1
                    if (j < s.items.size() && m == -1) continue;

                    s.items[i].floor += m;
                    s.elevator += m;
                    if (j < s.items.size()) {
                        s.items[j].floor += m;
                    }


                    if (is_valid_state(s)) {
                        std::string hash = hash_state(s);

                        if (mem.count(hash) == 0) {
                            mem.insert(hash);
                            s.update_heuristic();
                            pq.push(s);
                        }

                    }

                    s.items[i].floor -= m;
                    s.elevator -= m;
                    if (j < s.items.size()) {
                        s.items[j].floor -= m;
                    }
                }
            }
        }
    }

    return -1;
}

aoch::Result solve_day11(aoch::Input& in) {
    aoch::Result r;

    std::vector<Item> items;
    std::unordered_map<std::string, size_t> material_to_id;

    size_t next_id = 0;
    size_t last_floor = in.size() - 1;

    for (size_t i = 0; i < in.size(); ++i) {
        auto tokens = aoch::split(in[i]);

        if (tokens[4] != "nothing") {
            size_t idx = 4;

            while (idx < tokens.size()) {
                if (tokens[idx] == "and") {
                    idx++;
                }

                Item it;
                it.t = tokens[idx + 2][0] == 'm' ? ItemType::CHIP : ItemType::GEN;

                std::string mat;

                if (it.t == ItemType::CHIP) {
                    mat = aoch::split(tokens[idx + 1], '-')[0];
                } else {
                    mat = tokens[idx + 1];
                }

                if (material_to_id.count(mat) == 0) {
                    material_to_id[mat] = next_id++;
                }

                it.material_id = material_to_id[mat];
                it.floor = i;

                items.push_back(it);

                idx += 3;
            }
        }
    }

    r.part1 = std::to_string(solve({items, 0, 0}, last_floor));

    for (size_t i = 0; i < 2; ++i) {
        items.push_back({ItemType::CHIP, next_id, 0});
        items.push_back({ItemType::GEN,  next_id++, 0});
    }

    r.part2 = std::to_string(solve({items, 0, 0}, last_floor));
    return r;
}
