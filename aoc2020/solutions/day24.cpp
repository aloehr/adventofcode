#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <algorithm>
#include <unordered_map>
#include <cassert>

#include "../aoc.hpp"


template <typename T>
struct hex_grid {

    std::vector<std::vector<T>> data;
    std::vector<T> def_line;
    int middle_offset;
    T def_val;

    hex_grid(size_t s, T def_val) : def_val(def_val) {

        bool l_odd = s & 1 ? true : false;

        if (!l_odd) s += 1;

        this->def_line = std::vector<T>(s, this->def_val);
        this->data.resize(s, this->def_line);

        this->middle_offset = s/2; 
    }

    void set_tile(int x, int y, T val) {
        //std::cout << "set tile: " << x << " " << y << std::endl;
        //if (y&1) y -= 1;
        this->resize_if_needed(x, y);
        this->data[this->middle_offset + y][this->middle_offset + x] = val;
        //std::cout << "set end" << std::endl;
    }

    T get_tile(size_t x, size_t y) {
        //std::cout << "get tile: " << x << " " << y << std::endl;
        //if (y&1) y -= 1;
        this->resize_if_needed(x, y);
        return this->data[this->middle_offset + y][this->middle_offset + x];
        //std::cout << "set end" << std::endl;
    }

    void resize_if_needed(int x, int y) {
        int s = this->data.size();
        int x_coord = this->middle_offset + x;
        int y_coord = this->middle_offset + y;
        int new_middle_offset = 0;
        while (x_coord < 0 || x_coord >= s || y_coord < 0 || y_coord >= s) {
            s *= 2;
            new_middle_offset = s/2;

            x_coord = new_middle_offset + x;
            y_coord = new_middle_offset + y;
        }

        // size doesn't need to change, return
        if (s == this->data.size()) return;

        this->middle_offset = new_middle_offset;

        int added_length = s - data.size();
        bool odd_added_length = (added_length & 1 ? true : false);
        if (odd_added_length) {
            added_length += 1;
            s += 1;
        }

        int added_length_half = added_length/2;

        this->def_line = std::vector<T>(s, this->def_val);

        std::vector<std::vector<T>> new_vector(s, this->def_line);

        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data[i].size(); ++j) {
                new_vector[added_length_half + i][added_length_half + j] = data[i][j];
            }
        }

        data = std::move(new_vector);
    }

    size_t count_eq(T eq_val) {
        size_t res = 0;

        for (const auto& l : this->data) {
            for (const auto& v : l) {
                res += v == eq_val;
            }
        }

        return res;
    }

    friend void swap(hex_grid<T>& a, hex_grid<T>& b) {
        std::swap(a.def_val, b.def_val);
        std::swap(a.middle_offset, b.middle_offset);
        std::swap(a.def_line, b.def_line);
        std::swap(a.data, b.data);
    }
};

// 0 NE
// 1 E
// 2 SE
// 3 SW
// 4 W
// 5 NW

bool check_border_false(hex_grid<bool>& hg) {

    for (size_t i = 0; i < hg.data.size(); ++i) {
        if (hg.data[0][i]) return false; 
        if (hg.data[hg.data.size() - 1][i]) return false; 
        if (hg.data[i][0]) return false; 
        if (hg.data[i][hg.data.size() - 1]) return false; 

        if (hg.data[1][i]) return false; 
        if (hg.data[hg.data.size() - 2][i]) return false; 
        if (hg.data[i][1]) return false; 
        if (hg.data[i][hg.data.size() - 2]) return false; 
    }

    return true;
}

int count_nb(hex_grid<bool>& hg, int x, int y) {
    //std::cout << x << " " << y << '\n';
    bool odd_row = y & 1;

    int r = 0;

    //std::cout << "h1 " << x+1 << " " << y << '\n';
    r += hg.data[y][x+1]; // E
    //std::cout << "h2 " << x-1 << " " << y << '\n';
    r += hg.data[y][x-1]; // W
    //std::cout << "h3 " << x + odd_row << " " << y-1 << '\n';
    r += hg.data[y-1][x + odd_row]; // NE
    //std::cout << "h4 " << x + odd_row << " " << y+1 << '\n';
    r += hg.data[y+1][x + odd_row]; // SE 
    //std::cout << "h5 " << x - !odd_row << " " << y-1 << '\n';
    r += hg.data[y-1][x - !odd_row]; // NW 
    //std::cout << "h6 " << x - !odd_row << " " << y+1 << '\n';
    r += hg.data[y+1][x - !odd_row]; // SW 
    //std::cout << '\n';

    return r;
}

bool get_next_state(bool state, int nb_count) {
    if (state) {
        if (nb_count == 0 || nb_count > 2) return false;
    }
    else {
        if (nb_count == 2) return true;
    }

    return state;
}

answer solve_day24(input& in) {

    answer a;
    in.pop_back();

    std::vector<std::vector<int>> sequences;
    sequences.reserve(in.size());
    
    for (auto& l : in) {

        std::vector<int> sequence;
        size_t i = 0;
        while (i < l.size()) {
            if (l[i] == 's') {
                if (l[i+1] == 'e') {
                    sequence.push_back(2);
                }
                else {
                    sequence.push_back(3);
                }
                i += 2;
            }
            else if (l[i] == 'n') {
                if (l[i+1] == 'e') {
                    sequence.push_back(0);
                }
                else {
                    sequence.push_back(5);
                }
                
                i += 2;
            }
            else if (l[i] == 'w') {
                sequence.push_back(4);
                i += 1;
            }
            else {
                sequence.push_back(1);
                i += 1;
            }
        }

        sequences.emplace_back(std::move(sequence));
    }
    
    // there is a bug with gridsize 16,23,24,29 and whatever else for part 2...
    hex_grid<bool> hg(32, false);

// 0 NE
// 1 E
// 2 SE
// 3 SW
// 4 W
// 5 NW

    for (const auto& seq : sequences) {
        int offset_x = 0;
        int offset_y = 0;

        for (const auto dir : seq) {

            bool odd_row = (offset_y & 1);
            if (dir == 4) { // W
                offset_x -= 1;
            }
            else if (dir == 1) { // E
                offset_x += 1;
            }
            else if (dir == 5) { // NW
                if (!odd_row) offset_x--;
                offset_y--;
            }
            else if (dir == 0) { // NE
                offset_y--;
                if (odd_row) offset_x++;
            }
            else if (dir == 3) { // SW
                offset_y++;
                if (!odd_row) offset_x--;
            }
            else { // SE
                offset_y++;
                if (odd_row) offset_x++;
            }
        }

        hg.set_tile(offset_x, offset_y, !hg.get_tile(offset_x, offset_y));
    }

    a.part1 = std::to_string(hg.count_eq(true));

    hex_grid<bool> next_hg(hg.data.size(), false);

    for (int i = 0; i < 100; ++i) {
        bool border_false = check_border_false(hg);

        if (!border_false) {
            hg.get_tile(hg.middle_offset * 2 + 1, 0);
            next_hg = hex_grid<bool>(hg.data.size(), false);
        }

        for (size_t y = 2; y < next_hg.data.size()-2; ++y) {
            for (size_t x = 2; x < next_hg.data.size()-2; ++x) {
                next_hg.data[y][x] = false; 
            }
        }

        for (size_t y = 0; y < hg.data.size(); ++y) {
            for (size_t x = 0; x < hg.data.size(); ++x) {
                if (hg.data[y][x]) {
                    
                    bool odd_row = y & 1;

                    // current field
                    int nb_count = count_nb(hg, x, y);
                    next_hg.data[y][x] = get_next_state(true, nb_count);

                    // E nb
                    nb_count = count_nb(hg, x+1, y);
                    next_hg.data[y][x+1] = get_next_state(hg.data[y][x+1], nb_count);

                    // W nb
                    nb_count = count_nb(hg, x-1, y);
                    next_hg.data[y][x-1] = get_next_state(hg.data[y][x-1], nb_count);

                    // NW nb
                    nb_count = count_nb(hg, x-!odd_row, y-1);
                    next_hg.data[y-1][x-!odd_row] = get_next_state(hg.data[y-1][x-!odd_row], nb_count);

                    // SW nb
                    nb_count = count_nb(hg, x-!odd_row, y+1);
                    next_hg.data[y+1][x-!odd_row] = get_next_state(hg.data[y+1][x-!odd_row], nb_count);

                    // NE nb
                    nb_count = count_nb(hg, x+odd_row, y-1);
                    next_hg.data[y-1][x+odd_row] = get_next_state(hg.data[y-1][x+odd_row], nb_count);

                    // SE nb
                    nb_count = count_nb(hg, x+odd_row, y+1);
                    next_hg.data[y+1][x+odd_row] = get_next_state(hg.data[y+1][x+odd_row], nb_count);
                }
            }
        }

        swap(hg, next_hg);
    }

    a.part2 = std::to_string(hg.count_eq(true));

    return a;
}
