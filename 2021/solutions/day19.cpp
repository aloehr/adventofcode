#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <cassert>
#include <map>
#include <set>

#include "../aoc.hpp"

using pos_t = std::array<int, 3>;
using dir_t = std::array<int, 3>;
using rot_t = std::array<int, 3>;

int sign(int n) {
    int ret = n / std::abs(n);

    assert(std::abs(ret) == 1);

    return ret;
}

std::array<int, 3> rotate(const pos_t& p, const rot_t& r) {
    pos_t ret;

    for (size_t i = 0; i < 3; ++i) {
        ret[i] = sign(r[i]) * p[std::abs(r[i])- 1];
    }

    return ret;
}

struct scanner {

    std::vector<std::array<int, 3>> rotations = {
    // z up
    // a b c -> a c -b -> 1 -3 2j
    { 1,  3, -2}, // 1 -3 2
    {-2,  3, -1}, // -3 -1 2jkj
    {-1,  3,  2},
    { 2,  3,  1},
    // z down
    { 1, -3,  2},
    {-2, -3,  1},
    {-1, -3, -2},
    { 2, -3, -1},
    // x up
    { 2,  1, -3},
    { 3,  1,  2},
    {-2,  1,  3},
    {-3,  1, -2},
    // x down
    { 2, -1,  3},
    { 3, -1, -2},
    {-2, -1, -3},
    {-3, -1,  2},
    // y up
    { 1,  2,  3},
    { 3,  2, -1},
    {-1,  2, -3},
    {-3,  2,  1},
    // y down
    { 1, -2, -3},
    { 3, -2,  1},
    {-1, -2,  3},
    {-3, -2, -1},
/*
    // z up
    x  y  z
    1  3 -2
   -2  3 -1
   -1  3  2
    2  3  1

    // z up
    x  y  z
    1 -3  2
   -2 -3  1
   -1 -3 -2
    2 -3 -1

    // x up
    x   y   z
    2   1  -3
    3   1   2
   -2   1   3
   -3   1  -2

    // x down
    x   y   z
    2  -1   3
    3  -1  -2
   -2  -1  -3
   -3  -1   2

    // y up
    x   y   z
    1   2   3
    3   2  -1
   -1   2  -3
   -3   2   1

   // y down
    1  -2  -3
    3  -2   1
   -1  -2   3
   -3  -2  -1

 */

    };

    size_t id = 0;
    pos_t pos = {0, 0, 0};
    rot_t rot = {1, 2, 2};

    std::vector<pos_t> beacons;
    //std::map<std::array<size_t, 2>, dir_t> bd;

    std::string to_string() {
        std::stringstream ss;

        ss << "------" << id << "-------\n";

        for (auto& b : beacons) {
            for (int i = 0; i < 3; ++i) {
                ss << b[i] << ",";
            }
            ss << "\n";
        }

        return ss.str();
    }

    /*static dir_t calc_bd(pos_t b1, pos_t b2) {
        dir_t ret;

        for (size_t i = 0; i < 3; ++i) {
            ret[i] = b2[i] - b1[i];
        }

        return ret;
    }*/

};

bool equal(pos_t a, pos_t b) {
    for (size_t i = 0; i < 3; ++i) {
        if (a[i] != b[i]) return false;
    }

    return true;
}

std::string pos_str(pos_t p) {
    std::stringstream ss;

    ss << "[" << p[0] << ", " << p[1] << ", " << p[2] << "]";
    return ss.str();
}

bool match_scanners(const scanner& a, scanner& b) {

    ////assert(b.id == 1);
    //std::cout << "trying to match scanners " << a.id << " " << b.id << std::endl;
    for (size_t i = 0; i < b.beacons.size(); ++i) {
        for (size_t j = 0; j < a.beacons.size(); ++j) {
            for (const auto& r : a.rotations) {
                //std::cout << "selected beacon in scanner b: " << pos_str(b.beacons[i]) << std::endl;
                //std::cout << "selected beacon in scanner a: " << j << " " << pos_str(a.beacons[j]) << std::endl;
                //std::cout << "with rotation matrix: " << pos_str(r) << std::endl;
                auto first_b = rotate(b.beacons[i], r);
                auto first_a = rotate(a.beacons[j], a.rot);

                //first_a[0] += a.pos[0];
                //first_a[1] += a.pos[1];
                //first_a[2] += a.pos[2];
                //std::cout << "rotated b: " << pos_str(first_b) << std::endl;

                pos_t transl = {
                    first_a[0] - first_b[0],
                    first_a[1] - first_b[1],
                    first_a[2] - first_b[2]
                };


                //std::cout << "translation: " << pos_str(transl) << std::endl;
                //std::cout << "-------------------" << std::endl;

                // figure out translation from i to j
                //
                int c = 1;
                for (size_t k = 0; k < b.beacons.size(); ++k) {
                    for (size_t l = 0; l < a.beacons.size(); ++l) {
                        if (k == i || l == j) continue;


                        auto cur_b = rotate(b.beacons[k], r);

                        cur_b[0] += transl[0];
                        cur_b[1] += transl[1];
                        cur_b[2] += transl[2];


                        // translate here
                        //
                        auto rot_a = rotate(a.beacons[l], a.rot);

                        //rot_a[0] += a.pos[0];
                        //rot_a[1] += a.pos[1];
                        //rot_a[2] += a.pos[2];

                        if (equal(cur_b, rot_a)) {
                            c++;
                            break;
                        }
                    }

                    //if (c >= 12) break;
                }
                //if (c > 1 ) std::cout << "found matches this iter: " << c << std::endl;
                if (c >= 12) {
                    std::cout << "found match between scanner a (" << a.id << ") and scanner b (" << b.id << ")" << std::endl;
                    std::cout << "a pos: " << pos_str(a.pos) << std::endl;
                    std::cout << "transl: " << pos_str(transl) << std::endl;
                    b.rot = r;
                    b.pos = transl;
                    b.pos[0] += a.pos[0];
                    b.pos[1] += a.pos[1];
                    b.pos[2] += a.pos[2];
                    return true;
                }
            }



        }
    }

    return false;
}

answer solve_day19(input& in) {



    in.push_back("");
    answer a;


    size_t scanner_id = 0;
    std::vector<scanner> scanners;

    scanner scan;
    scan.id = scanner_id++;
    for (auto& l : in) {
        if (l.empty()) {
            scanners.push_back(scan);
            scan.beacons.clear();
            scan.id = scanner_id++;
            continue;
        }

        if (l[1] == '-') continue;

        auto tmp = split(l, ',');

        std::cout << scanner_id << " " << l << std::endl;
        std::array<int, 3> b;
        std::cout << tmp.size() << std::endl;
        assert(tmp.size() == 3);

        for (size_t i = 0; i < tmp.size(); ++i) {
            b[i] = std::stoi(tmp[i]);
        }

        scan.beacons.push_back(b);
    }

    std::vector<size_t> unregistered;
    for (size_t i = 1; i < scanners.size(); ++i) {
        //std::cout << scanners[i].to_string() << std::endl;
        unregistered.push_back(i);
    }

    //return a;
/*
selected beacon in scanner b: [553, 889, -390]
selected beacon in scanner a: 5 [-485, -357, 347]
with rotation matrix: [-1, 2, -3]
rotated b: [-553, 889, 390]
translation: [68, -1246, -43]
-------------------

-391,539,-444
r: 391, 539, 444
t: 451, -707, 401
*/

    std::vector<size_t> registered;
    registered.push_back(0);
    scanners[0].rot = {1, 2, 3};

    /*for (auto& s : scanners) {
        std::cout << s.to_string() << std::endl;

        for (size_t i = 0; i < s.beacons.size(); ++i) {
            for (size_t j = 0; j < s.beacons.size(); ++j) {
                if (i == j) continue;


                s.bd[{i, j}] = scanner::calc_bd(s.beacons[i], s.beacons[j]);
            }
        }
    }*/

    while (unregistered.size()) {
        bool found_match = false;
        for (auto i : registered) {
            for (size_t j = 0; j < unregistered.size(); ++j) {
                if (match_scanners(scanners[i], scanners[unregistered[j]])) {
                //if (true) {
                    found_match = true;
                    registered.push_back(unregistered[j]);
                    unregistered.erase(unregistered.begin() + j);
                    break;
                }
            }

            if (found_match) {
                std::cout << "found " << registered.size() << std::endl;
                break;
            }
        }

        std::cout << "registered count: " << registered.size() << std::endl;
    }

    std::set<pos_t> unique_beacons;

    for (auto& s : scanners) {

        std::cout << s.id << " " << pos_str(s.pos) << std::endl;
        for (auto& b : s.beacons) {
            pos_t cur = rotate(b, s.rot);
            cur[0] += s.pos[0];
            cur[1] += s.pos[1];
            cur[2] += s.pos[2];

            unique_beacons.insert(cur);
        }

    }
    a.part1 = std::to_string(unique_beacons.size());


    // part 2
    int largest = 0;

    for (size_t i = 0; i < scanners.size(); ++i) {
        for (size_t j = 0; j < scanners.size(); ++j) {
            if (i == j) continue;

            int c = 0;
            c += std::abs(scanners[i].pos[0] - scanners[j].pos[0]);
            c += std::abs(scanners[i].pos[1] - scanners[j].pos[1]);
            c += std::abs(scanners[i].pos[2] - scanners[j].pos[2]);

            largest = std::max(c, largest);
        }
    }


    a.part2 = std::to_string(largest);

    return a;
}
