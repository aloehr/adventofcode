#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <set>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


using pos_t = std::array<int, 3>;
using rot_t = std::array<int, 3>;

std::array<int, 3> rotate(const pos_t& p, const rot_t& r) {
    pos_t ret;

    for (size_t i = 0; i < 3; ++i) {
        int sign = r[i] & (1 << ((8 * sizeof(int)) - 1)) ? -1 : 1;
        ret[i] = sign * p[std::abs(r[i]) - 1];
    }

    return ret;
}

struct scanner {

    std::vector<std::array<int, 3>> rotations = {
        // z up
        { 1,  3, -2},
        {-2,  3, -1},
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
    };

    size_t id = 0;
    pos_t pos = {0, 0, 0};
    rot_t rot = {1, 2, 3};

    std::vector<pos_t> beacons;

    std::string to_string() const {
        std::stringstream ss;

        ss << "------" << id << "------\n";

        for (auto& b : beacons) {
            for (int i = 0; i < 3; ++i) {
                ss << b[i] << ",";
            }
            ss << "\n";
        }

        return ss.str();
    }
};

bool equal(const pos_t& a, const pos_t& b) {
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
    for (size_t i = 0; i < b.beacons.size(); ++i) {
        for (size_t j = 0; j < a.beacons.size(); ++j) {
            for (const auto& r : a.rotations) {

                auto first_b = rotate(b.beacons[i], r);
                auto first_a = rotate(a.beacons[j], a.rot);

                pos_t transl = {
                    first_a[0] - first_b[0],
                    first_a[1] - first_b[1],
                    first_a[2] - first_b[2]
                };

                int c = 1;
                bool not_found = false;
                for (size_t k = 0; k < b.beacons.size(); ++k) {

                    if (b.beacons.size() - k < 12 - ((size_t)c)) {
                        // we don't have enough beacons left in b to get more than 11 matches
                        not_found = true;
                        break;
                    }

                    for (size_t l = 0; l < a.beacons.size(); ++l) {
                        if (k == i || l == j) continue;

                        auto cur_b = rotate(b.beacons[k], r);

                        cur_b[0] += transl[0];
                        cur_b[1] += transl[1];
                        cur_b[2] += transl[2];

                        auto rot_a = rotate(a.beacons[l], a.rot);

                        if (equal(cur_b, rot_a)) {
                            c++;
                            break;
                        }
                    }

                    if (c == 12 || not_found) break;
                }

                if (c == 12) {
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

aoch::Result solve_day19(aoch::Input& in) {
    aoch::Result a;

    // actually this time we need an extra empty line at the end
    in.push_back("");

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

        auto tmp = aoch::split(l, ',');

        std::array<int, 3> b;

        for (size_t i = 0; i < tmp.size(); ++i) {
            b[i] = std::stoi(tmp[i]);
        }

        scan.beacons.push_back(b);
    }

    std::vector<size_t> unregistered;
    for (size_t i = 1; i < scanners.size(); ++i) {
        unregistered.push_back(i);
    }

    std::vector<size_t> registered;
    registered.push_back(0);

    while (unregistered.size()) {
        // pick one registered scan
        size_t i = registered[0];
        registered.erase(registered.begin());

        // check against all unregistered
        for (size_t j = 0; j < unregistered.size(); ++j) {
            if (match_scanners(scanners[i], scanners[unregistered[j]])) {
                // since we found a match move the scanner from unregistered to registered
                registered.push_back(unregistered[j]);
                unregistered.erase(unregistered.begin() + j);

                j -= 1;
            }
        }
    }

    std::set<pos_t> unique_beacons;

    for (auto& s : scanners) {
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
