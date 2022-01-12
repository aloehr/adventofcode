#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <cassert>

#include "../aoc.hpp"


enum class side {
    up    = 0, 
    right = 1, 
    down  = 2, 
    left  = 3
};

side other_side(side s) {
    if (s == side::left) {
        return side::right;
    }
    else if (s == side::right) {
        return side::left;
    }
    else if(s == side::up) {
        return side::down;
    }
    else if(s == side::down) {
        return side::up;
    }
    else {
        assert(false);
        return side::down;
    }
}

struct tile {
    int id;
    std::vector<std::string> img_data;

    tile *nb[4] = {nullptr, nullptr, nullptr, nullptr};
    bool border[4] = {false, false, false, false};

    side up = side::up;

    bool flipped_v = false;
    bool flipped_h = false;


    bool is_oriented() {
        for (int i = 0; i < 4; ++i) {
            if (nb[i]) return true;
            if (border[i]) return true;
        }

        return false;
    }
    
    int free_border_count() {
        int c = 0;
        for (int i = 0; i < 4; ++i) {
            if (this->is_free_border((side) i)) c++;
        }

        return c;
    }

    side get_free_border_side() {
        for (int i = 0; i < 4; ++i) {
            if (this->is_free_border((side) i))
                return (side) i;
        }

        assert(false);
        return (side) 5;
    }

    bool has_free_border() {
        for (int i = 0; i < 4; ++i) {
            if (this->is_free_border((side) i)) {
                return true;
            }
        }

        return false;
    }

    bool is_free_border(side s) {
        int si = (int) s;
        return !(this->border[si] || this->nb[si]);
    }

    side local_side(side s) {
        side l = (side) (((int) s + (int) this->up) % 4);

        if (s == side::down || s == side::up) {
            if (this->flipped_v) 
                return other_side(l);
            else
                return l;
        }
        else if (s == side::left || s == side::right) {
            if (this->flipped_h) 
                return other_side(l);
            else
                return l;
        }

        return l;
    }

    std::string get_row(int r) {
        side s = this->local_side(side::up); 

        bool fh = this->flipped_h;
        bool fv = this->flipped_v;

        bool read_bw_h = false;
        bool read_bw_v = false;

        if (this->up == side::right) {
            read_bw_h = true;
            fv = this->flipped_h;
            fh = this->flipped_v;
        }
        else if (this->up == side::down) {
            read_bw_h = true;
            read_bw_v = true;
        }
        else if (this->up == side::left) {
            read_bw_v = true;
            fv = this->flipped_h;
            fh = this->flipped_v;
        }

        if (s == side::left || s == side::right) {
            int column = r; 

            if (s == side::right)
                column = this->img_data[0].size() - 1 - r;
            
            std::string r; 

            for (size_t i = 0; i < this->img_data.size(); ++i) {
                r += this->img_data[i][column];
            }
            
            if (fv ^ read_bw_v) {
                r = std::string(r.rbegin(), r.rend());
            }

            return r;
        }
        else {
            int row = r;
            
            if (s == side::down)
                row = this->img_data.size() - 1 - r;
                

            std::string r = this->img_data[row];

            if (fh ^ read_bw_h) {
                r = std::string(r.rbegin(), r.rend());
            }

            return r;
        }
    }

    std::vector<std::string> get_rotated_img() {
        std::vector<std::string> r;

        for (size_t i = 0; i < this->row_count(); ++i) {
            r.push_back(this->get_row(i));
        }
        
        return r;
    }

    size_t row_count() {
        return img_data.size();
    }
    
    std::string get_border(side s) {

        s = this->local_side(s);
        bool fh = this->flipped_h;
        bool fv = this->flipped_v;

        bool read_bw_h = false;
        bool read_bw_v = false;

        if (this->up == side::right) {
            read_bw_h = true;
            fv = this->flipped_h;
            fh = this->flipped_v;
        }
        else if (this->up == side::down) {
            read_bw_h = true;
            read_bw_v = true;
        }
        else if (this->up == side::left) {
            read_bw_v = true;
            fv = this->flipped_h;
            fh = this->flipped_v;
        }

        if (s == side::left || s == side::right) {
            int column = 0; 

            if (s == side::right)
                column = this->img_data[0].size() - 1;
            
            std::string r; 

            for (size_t i = 0; i < this->img_data.size(); ++i) {
                r += this->img_data[i][column];
            }
            
            if (fv ^ read_bw_v) {
                r = std::string(r.rbegin(), r.rend());
            }

            return r;
        }
        else {
            int row = 0;
            
            if (s == side::down)
                row = this->img_data.size() - 1;
                

            std::string r = this->img_data[row];

            if (fh ^ read_bw_h) {
                r = std::string(r.rbegin(), r.rend());
            }

            return r;
        }
    }

    std::string to_string() {
        std::stringstream ss;
        ss << "Tile " << this->id << " up " << (int) this->up << " fh " << this->flipped_h << " fv " << this->flipped_v << ":" << '\n';

        for (size_t i = 0; i < this->row_count(); ++i) {

            ss << this->get_row(i) << '\n';
        }

        return ss.str();
    }
};

std::vector<std::string> remove_border(std::vector<std::string> s, size_t bl = 1) {
    
    std::vector<std::string> r;
    r.resize(s.size() - 2);

    for (size_t i = 1; i < s.size() - 1; ++i) {
        r[i-1] = s[i].substr(bl,s[i].size() - bl * 2);
    }

    return r;
}

bool mark_monster(std::vector<std::string>& in, const std::vector<std::string>& m, size_t s_i, size_t s_j) {

    for (size_t i = 0; i < m.size(); ++i) {
        for (size_t j = 0; j < m[0].size(); ++j) { 
            if (m[i][j] == '#' && in[s_i + i][s_j + j] != '#') {
                return false;
            }
        }
    }

    for (size_t i = 0; i < m.size(); ++i) {
        for (size_t j = 0; j < m[0].size(); ++j) { 
            if (m[i][j] == '#') {
                in[s_i + i][s_j + j] = 'O';
            }
        }
    }

    return true;
}

bool check_monster(std::vector<std::string>& in) {

    std::vector<std::string> monster(3);
    monster[0] = "                  # ";
    monster[1] = "#    ##    ##    ###";
    monster[2] = " #  #  #  #  #  #   ";

    bool found_monster = false;

    for (size_t i = 0; i < in.size() - monster.size() + 1; ++i) {
        for (size_t j = 0; j < in[0].size() - monster[0].size() + 1; ++j) {
            if (mark_monster(in, monster, i, j)) {
                found_monster = true;
            }
        }
    }

    return found_monster;
}

answer solve_day20(input& in) {

    answer a;
    in.pop_back();

    std::vector<tile*> tiles_processing;
    bool read_tile_id_next = true;

    for (const auto& l: in) {
        if (l.empty()) {
            read_tile_id_next = true; 
            continue;
        }

        if (read_tile_id_next) {
            tiles_processing.push_back(new tile);
            tiles_processing.back()->id = std::stoi(l.substr(5));

            read_tile_id_next = false;
        }
        else {

            tiles_processing.back()->img_data.push_back(l);
        }
    }

    std::vector<tile*> tiles;

    unsigned long long sum = 1;
    while (!tiles_processing.empty()) {
        
        tile *ct = nullptr; 
        for (size_t i = 0; i < tiles_processing.size(); ++i) {
            if (tiles_processing[i]->free_border_count() < 4) {
                ct = tiles_processing[i];
                tiles_processing.erase(tiles_processing.begin() + i);
                break;
            }
        }

        if (!ct) {
            ct = tiles_processing.back();
            tiles_processing.pop_back();
        }

        while (ct->has_free_border()) {
            side free_s = ct->get_free_border_side(); 
            bool found_match = false;

            std::string b = ct->get_border(free_s);
            //std::cout << "testing border for " << ct->id << ": " << '\n' << b << " free border: " << (int) free_s << " local: " << (int) ct->local_side(free_s) << '\n';
            //std::cout << " u: " << (int) ct->up << " fv: " << ct->flipped_v << " fh: " << ct->flipped_h << std::endl;

            for (auto t : tiles_processing) {
                if (t->is_oriented()) {
                    if (t->is_free_border(other_side(free_s))) {

                        std::string ob = t->get_border(other_side(free_s));

                        if (b == ob) {
                            found_match = true;
                            //std::cout << "found match " << t->id << std::endl;
                            //std::cout << ob << " " << t->id << " u: " << (int) t->up << " fv: " << t->flipped_v << " fh: " << t->flipped_h << std::endl;
                        }
                    }
                }
                else {
                    for (int i = 0; i < 4; ++i) {
                        t->up = (side) i;

                        t->flipped_v = false;
                        t->flipped_h = false;

                        std::string ob = t->get_border(other_side(free_s));
                        //std::cout << ob << " " << t->id << " u: " << (int) t->up << " fv: " << t->flipped_v << " fh: " << t->flipped_h << std::endl;

                        if (ob == b) {
                            found_match = true;
                            //std::cout << "found match " << t->id << std::endl;
                            //std::cout << ob << " " << t->id << " u: " << (int) t->up << " fv: " << t->flipped_v << " fh: " << t->flipped_h << std::endl;
                            break;
                        }

                        t->flipped_v = true;
                        t->flipped_h = false;

                        ob = t->get_border(other_side(free_s));
                        //std::cout << ob << " " << t->id << " u: " << (int) t->up << " fv: " << t->flipped_v << " fh: " << t->flipped_h << std::endl;

                        if (ob == b) {
                            found_match = true;
                            //std::cout << "found match " << t->id << std::endl;
                            //std::cout << ob << " " << t->id << " u: " << (int) t->up << " fv: " << t->flipped_v << " fh: " << t->flipped_h << std::endl;
                            break;
                        }

                        t->flipped_v = false;
                        t->flipped_h = true;

                        ob = t->get_border(other_side(free_s));
                        //std::cout << ob << " " << t->id << " u: " << (int) t->up << " fv: " << t->flipped_v << " fh: " << t->flipped_h << std::endl;

                        if (ob == b) {
                            found_match = true;
                            //std::cout << "found match " << t->id << " local: " << (int) t->local_side(other_side(free_s)) << std::endl;
                            //std::cout << ob << " " << t->id << " u: " << (int) t->up << " fv: " << t->flipped_v << " fh: " << t->flipped_h << std::endl;
                            break;
                        }
                    }
                }
                if (found_match) {
                    //std::cout << "setting ct  " << ct->id << " " << (int) free_s << std::endl;
                    //std::cout << "setting oth " << t->id  << " " << (int) other_side(free_s) << std::endl;
                    ct->nb[(int) free_s] = t;
                    t->nb[(int) other_side(free_s)] = ct;
                    break;
                }
            }

            if (!found_match) {
                ct->border[(int) free_s] = true;
            }
        }

        int bc = 0;
        for (int i = 0; i < 4; ++i) {
            if (ct->border[i]) bc++;
        }

        assert(bc <= 2);

        if (bc == 2) {
            sum *= ct->id;
        }

        tiles.push_back(ct);
    }

    tile *t = tiles.back();

    while (t->nb[(int) side::left]) {
        if (t->nb[(int) side::left]) t = t->nb[(int) side::left];
    }
    

    int img_count = 1;
    while (t->nb[(int) side::right]) {
        t = t->nb[(int) side::right];
        img_count++;
    }

    int sub_img_length = t->get_row(0).size() - 2;
    int img_length = img_count * sub_img_length;


    while (t->nb[(int) side::left] || t->nb[(int) side::up]) {
        if (t->nb[(int) side::left]) t = t->nb[(int) side::left];
        if (t->nb[(int) side::up])  t = t->nb[(int) side::up];
    }

    tile big_tile;
    std::string r(img_length, 'O');
    big_tile.img_data.resize(img_length, r);

    tile *row_ptr = t;

    int i = 0; 
    while (row_ptr) {

        tile *col_ptr = row_ptr;

        int j = 0;
        while (col_ptr) {

            for (size_t k = 1; k < col_ptr->row_count() - 1; ++k) {
                std::string d = col_ptr->get_row(k);
                for (size_t l = 1; l < d.size() - 1; ++l) {
                    big_tile.img_data[i*sub_img_length + (k-1)][j*sub_img_length + (l-1)] = d[l];
                }
            }

            j++;
            col_ptr = col_ptr->nb[(int) side::right];
        }


        i++;
        row_ptr = row_ptr->nb[(int) side::down];
    }

    //std::cout << big_tile.to_string() << std::endl;

    std::vector<std::string> res;
    bool fh = false;
    bool fv = false;
    for (int i = 0; i < 4 * 3; ++i) {

        if (i == 4) {
            fh = true;
        }
        else if (i == 8) {
            fh = false;
            fv = true;
        }

        big_tile.flipped_v = fv;
        big_tile.flipped_h = fh;

        big_tile.up = (side) (i % 4);

        res = big_tile.get_rotated_img();

        if (check_monster(res)) {
            //std::cout << "found monsters" << std::endl;
            //std::cout << big_tile.to_string() << std::endl;
            break;
        }

    }
    //std::cout << "after:" << '\n';
    //for (const auto& r : res) {
    //    std::cout << r << '\n';
    //}   
    int cr = 0;
    for (auto& r : res) {
        for (auto c : r) {
            if (c == '#') cr++;
        }
    }

    a.part1 = std::to_string(sum);
    a.part2 = std::to_string(cr);

    return a;
}
