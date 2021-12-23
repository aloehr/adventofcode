#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <cassert>

#include "../aoc.hpp"

struct Cuboid {
    size_t id;
    bool status;
    std::array<std::array<long, 2>, 3> area;

    bool intersect(const Cuboid& o) const {
        for (size_t i = 0; i < 3; ++i) {
            if (area[i][0] < o.area[i][0] && area[i][1] < o.area[i][0])
                return false;
            if (area[i][0] > o.area[i][1] && area[i][1] > o.area[i][1])
                return false;
        }

        return true;
    }

    bool fully_inside(const Cuboid& o) const {

        for (size_t i = 0; i < 3; ++i) {
            if (area[i][0] >= o.area[i][0] && area[i][1] <= o.area[i][1]) {
            }
            else {
                return false;
            }
        }

        return true;
    }

    std::string to_string() const {
        std::stringstream ss;

        ss << id << ": ";

        if (status) {
            ss << "on ";
        }
        else {
            ss << "off ";
        }

        ss << "x=" << area[0][0] << ".." << area[0][1];
        ss << ",y=" << area[1][0] << ".." << area[1][1];
        ss << ",z=" << area[2][0] << ".." << area[2][1];

        return ss.str();
    }
};

answer solve_day22(input& in) {

    answer a;
/*
    in = {
"on x=-20..26,y=-36..17,z=-47..7",
"on x=-20..33,y=-21..23,z=-26..28",
"on x=-22..28,y=-29..23,z=-38..16",
"on x=-46..7,y=-6..46,z=-50..-1",
"on x=-49..1,y=-3..46,z=-24..28",
"on x=2..47,y=-22..22,z=-23..27",
"on x=-27..23,y=-28..26,z=-21..29",
"on x=-39..5,y=-6..47,z=-3..44",
"on x=-30..21,y=-8..43,z=-13..34",
"on x=-22..26,y=-27..20,z=-29..19",
"off x=-48..-32,y=26..41,z=-47..-37",
"on x=-12..35,y=6..50,z=-50..-2",
"off x=-48..-32,y=-32..-16,z=-15..-5",
"on x=-18..26,y=-33..15,z=-7..46",
"off x=-40..-22,y=-38..-28,z=23..41",
"on x=-16..35,y=-41..10,z=-47..6",
"off x=-32..-23,y=11..30,z=-14..3",
"on x=-49..-5,y=-3..45,z=-29..18",
"off x=18..30,y=-20..-8,z=-3..13",
"on x=-41..9,y=-7..43,z=-33..15",
"on x=-54112..-39298,y=-85059..-49293,z=-27449..7877",
"on x=967..23432,y=45373..81175,z=27513..53682",
    };
    */

    std::vector<Cuboid> cubes;
    cubes.reserve(in.size());

    size_t next_id = 0;

    for (auto& l : in) {
        auto tmp = split(l);
        assert(tmp.size() == 2);

        Cuboid c;
        c.status = l[1] == 'n' ? true : false;

        tmp = split(tmp[1], ',');

        assert(tmp.size() == 3);

        for (size_t i = 0; i < tmp.size(); ++i) {
            auto tmp2 = split(tmp[i].substr(2), '.');

            assert(tmp2.size() == 2);

            c.area[i][0] = std::stol(tmp2[0]);
            c.area[i][1] = std::stol(tmp2[1]);
        }

        c.id = next_id++;
        cubes.push_back(c);

    }

    /*
    int c = 0;

    for (size_t i = 0; i < cubes.size()-1; ++i) {
        for (size_t j = i+1; j < cubes.size(); ++j) {
            if (cubes[i].intersect(cubes[j])) {
                c++;
            }

        }
    }
    */

    std::vector<bool> cube_state(1e6, false);
    assert(cube_state.size() == 1000000);

    for (auto& c : cubes) {

        if (c.area[0][0] > 50 || c.area[0][1] < -50) continue;
        if (c.area[1][0] > 50 || c.area[1][1] < -50) continue;
        if (c.area[2][0] > 50 || c.area[2][1] < -50) continue;

        for (int x = c.area[0][0]; x <= c.area[0][1]; ++x) {
            for (int y = c.area[1][0]; y <= c.area[1][1]; ++y) {
                for (int z = c.area[2][0]; z <= c.area[2][1]; ++z) {
                    long idx = (x + 50) * 1e4 + (y + 50) * 1e2 + (z + 50);

                    if (idx < 0 || idx > 1e6) continue;

                    cube_state[idx] = c.status;
                }
            }
        }
    }

    a.part1 = std::to_string(sum(cube_state));

    // part 2

    a.part2 = std::to_string(2);

    return a;
}
