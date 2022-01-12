#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <map>

#include "../aoc.hpp"


using world_slice = std::vector<std::string>;
using world = std::vector<world_slice>;
using hyper_world = std::vector<world>;

bool is_active(const world& w, int z, int y, int x) {
    if (x < 0 || y < 0 || z < 0) return false;
    if (x >= (int) w[0][0].size() || y >= (int) w[0].size() || z >= (int) w.size()) return false;

    return w[z][y][x] == '#';
}

bool is_active2(const hyper_world& w, int e, int z, int y, int x) {
    if (e < 0 || x < 0 || y < 0 || z < 0) return false;
    if (e >= (int) w.size() || x >= (int) w[0][0][0].size() || y >= (int) w[0][0].size() || z >= (int) w[0].size()) return false;

    return w[e][z][y][x] == '#';
}

int count_nb(const world& w, int z, int y, int x) {
    int count = 0;

    for (int i = z-1; i <= z+1; ++i) {
        for (int j = y-1; j <= y+1; ++j) {
            for (int k = x-1; k <= x+1; ++k) {
                if (i == z && j == y && k == x) continue;

                count += is_active(w, i, j, k);
            }
        }
    }

    return count;
}

int count_nb2(const hyper_world& w, int e, int z, int y, int x) {
    int count = 0;

    for (int a = e-1; a <= e+1; ++a) {
    for (int i = z-1; i <= z+1; ++i) {
        for (int j = y-1; j <= y+1; ++j) {
            for (int k = x-1; k <= x+1; ++k) {
                if (a == e && i == z && j == y && k == x) continue;

                count += is_active2(w, a, i, j, k);
            }
        }
    }
    }

    return count;
}

world expand(const world& w) {
    int old_width = w[0][0].size();
    int old_height = w[0].size();

    std::string empty_line(old_width + 2 , '.');
    world_slice empty_layer(old_height + 2, empty_line);

    world expanded_w;

    expanded_w.push_back(empty_layer);

    for (size_t i = 0; i < w.size(); ++i) {
        world_slice expanded_layer;
        expanded_layer.push_back(empty_line);

        for (size_t j = 0; j < w[0].size(); ++j) {
            std::string expanded_line = '.' + w[i][j] + '.';
            expanded_layer.push_back(expanded_line);
        }

        expanded_layer.push_back(empty_line);

        expanded_w.push_back(expanded_layer);
    }

    expanded_w.push_back(empty_layer);

    return expanded_w;
}

hyper_world expand2(const hyper_world& w) {
    int old_width = w[0][0][0].size();
    int old_height = w[0][0].size();
    int old_depth = w[0].size();

    std::string empty_line(old_width + 2 , '.');
    world_slice empty_layer(old_height + 2, empty_line);
    world empty_world(old_depth + 2, empty_layer);

    hyper_world expanded_hw;
    expanded_hw.push_back(empty_world);

    for (size_t e = 0; e < w.size(); ++e) {

        world exp_w;
        exp_w.push_back(empty_layer);

        for (size_t i = 0; i < w[0].size(); ++i) {
            world_slice expanded_layer;
            expanded_layer.push_back(empty_line);

            for (size_t j = 0; j < w[0][0].size(); ++j) {
                std::string expanded_line = '.' + w[e][i][j] + '.';
                expanded_layer.push_back(expanded_line);
            }

            expanded_layer.push_back(empty_line);
            exp_w.push_back(expanded_layer);
        }

        exp_w.push_back(empty_layer);
        expanded_hw.push_back(exp_w);
    }

    expanded_hw.push_back(empty_world);

    return expanded_hw;
}

void print_world(const world& w) {
    for (size_t i = 0; i < w.size(); ++i) {
        for (const auto& l : w[i]) {
            std::cout << l << '\n'; 
        }

            std::cout << '\n';
        }
}

char calc_next_state(const world& w, int z, int y, int x) {
    int cnb = count_nb(w, z, y, x);

    if (is_active(w, z, y, x) && (cnb != 2 && cnb != 3)) {
        return '.';
    }
    else if(!is_active(w, z, y, x) && cnb == 3) {
        return '#';
    }

    return w[z][y][x];
}

char calc_next_state2(const hyper_world& w, int e, int z, int y, int x) {
    int cnb = count_nb2(w, e, z, y, x);

    if (is_active2(w, e, z, y, x) && (cnb != 2 && cnb != 3)) {
        return '.';
    }
    else if(!is_active2(w, e, z, y, x) && cnb == 3) {
        return '#';
    }

    return w[e][z][y][x];
}

world execute(const world& w) {
    world next_w = w;

    for (int i = 0; i < (int) w.size(); ++i) {
        for (int j = 0; j < (int) w[i].size(); ++j) {
            for (int k = 0; k < (int) w[i][j].size(); ++k) {
                next_w[i][j][k] = calc_next_state(w, i, j, k);
            }
        }
    }

    return next_w;
}

hyper_world execute2(const hyper_world& w) {
    hyper_world next_w = w;

    for (int e = 0; e < (int) w.size(); ++e) {
    for (int i = 0; i < (int) w[e].size(); ++i) {
        for (int j = 0; j < (int) w[e][i].size(); ++j) {
            for (int k = 0; k < (int) w[e][i][j].size(); ++k) {
                next_w[e][i][j][k] = calc_next_state2(w, e, i, j, k);
            }
        }
    }
    }

    return next_w;
}

int count_active(const world& w) {
    int c = 0;

    for (int i = 0; i < (int) w.size(); ++i) {
        for (int j = 0; j < (int) w[i].size(); ++j) {
            for (int k = 0; k < (int) w[i][j].size(); ++k) {
                c += is_active(w, i, j, k);
            }
        }
    }

    return c;
}

int count_active2(const hyper_world& w) {
    int c = 0;

    for (int e = 0; e < (int) w.size(); ++e) {
    for (int i = 0; i < (int) w[e].size(); ++i) {
        for (int j = 0; j < (int) w[e][i].size(); ++j) {
            for (int k = 0; k < (int) w[e][i][j].size(); ++k) {
                c += is_active2(w, e, i, j, k);
            }
        }
    }
    }

    return c;
}

answer solve_day17(input& in) {

    answer a;
    in.pop_back();

    world current_state(1);
    current_state[0] = in;

    world copy_world = current_state;

    size_t cycle_count = 6;

    while (cycle_count--) {
        current_state = expand(current_state);
        current_state = execute(current_state);
    }

    hyper_world cur(1, copy_world);
    cycle_count = 6;

    while (cycle_count--) {
        cur = expand2(cur);
        cur = execute2(cur);
    }

    a.part1 = std::to_string(count_active(current_state));
    a.part2 = std::to_string(count_active2(cur));

    return a;
}
