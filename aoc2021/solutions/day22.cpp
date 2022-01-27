#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <cassert>
#include <set>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


struct Cuboid {

    size_t id = 0;
    bool status;

    std::vector<Cuboid> intersections;
    std::array<std::array<long, 2>, 3> area;


    long volume() const {

        long vol = (area[0][1] - area[0][0] + 1)
            * (area[1][1] - area[1][0] + 1)
            * (area[2][1] - area[2][0] + 1);

        assert(vol >= 0);

        for (auto& c : intersections) {
            vol -= c.volume();
        }

        return vol;
    }

    void calc_intersections() {

        auto& cbs = this->intersections;

        for (size_t i = 0; i < cbs.size()-1; ++i) {
            auto& cur = cbs[i];
            for (size_t j = i+1; j < cbs.size(); ++j) {
                auto& o = cbs[j];
                if (cur.intersect(o)) {
                    auto x = cur.get_intersection(o);
                    bool already_in = false;
                    for (auto& c : cur.intersections) {
                        if (c.equal(x)) {
                            already_in = true;
                            break;
                        }
                    }

                    if (!already_in) {
                        cur.intersections.push_back(x);
                    }
                }
            }

            if (cur.intersections.size() > 1) {
                cur.calc_intersections();
            }
        }

    }

    bool intersect(const Cuboid& o) const {
        for (size_t i = 0; i < 3; ++i) {
            if (area[i][0] > o.area[i][1] || area[i][1] < o.area[i][0])
                return false;
        }

        return true;
    }

    bool equal(const Cuboid& o) const {
        return area[0][0] == o.area[0][0]
            && area[0][1] == o.area[0][1]
            && area[1][0] == o.area[1][0]
            && area[1][1] == o.area[1][1]
            && area[2][0] == o.area[2][0]
            && area[2][1] == o.area[2][1];
    }

    Cuboid get_intersection(const Cuboid& o) const {
        Cuboid ret;

        if (this->intersect(o)) {
            ret.area[0][0] = std::max(area[0][0], o.area[0][0]);
            ret.area[0][1] = std::min(area[0][1], o.area[0][1]);
            ret.area[1][0] = std::max(area[1][0], o.area[1][0]);
            ret.area[1][1] = std::min(area[1][1], o.area[1][1]);
            ret.area[2][0] = std::max(area[2][0], o.area[2][0]);
            ret.area[2][1] = std::min(area[2][1], o.area[2][1]);
        }

        return ret;
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

aoch::Result solve_day22(aoch::Input& in) {

    aoch::Result a;

    std::vector<Cuboid> cubes;
    cubes.reserve(in.size());

    size_t next_id = 0;

    for (auto& l : in) {
        auto tmp = aoch::split(l);
        assert(tmp.size() == 2);

        Cuboid c;
        c.status = l[1] == 'n' ? true : false;

        tmp = aoch::split(tmp[1], ',');

        assert(tmp.size() == 3);

        for (size_t i = 0; i < tmp.size(); ++i) {
            auto tmp2 = aoch::split(tmp[i].substr(2), '.');

            assert(tmp2.size() == 2);

            c.area[i][0] = std::stol(tmp2[0]);
            c.area[i][1] = std::stol(tmp2[1]);
        }

        c.id = next_id++;
        cubes.push_back(c);

    }

    assert(cubes.size() == in.size());


    Cuboid init_cuboid;
    std::array<std::array<long, 2>, 3> xxx;
    xxx[0] = {-50, 50};
    xxx[1] = {-50, 50};
    xxx[2] = {-50, 50};
    init_cuboid.area = xxx;

    std::vector<Cuboid> initial_cubes;
    std::vector<Cuboid> next_cubes;

    for (auto& c : cubes) {
        if (c.intersect(init_cuboid)) {
            Cuboid tmp = init_cuboid.get_intersection(c);
            tmp.status = c.status;
            tmp.id = c.id;
            assert(tmp.volume() > 0);
            initial_cubes.push_back(tmp);
        }
    }


    for (size_t i = 0; i < initial_cubes.size()-1; ++i) {
        for (size_t j = i+1; j < initial_cubes.size(); ++j) {
            if (initial_cubes[i].intersect(initial_cubes[j])) {
                if (initial_cubes[i].status == true) {
                    auto tmp = initial_cubes[i].get_intersection(initial_cubes[j]);

                    bool already_in = false;
                    for (auto& c : initial_cubes[i].intersections) {
                        if (c.equal(tmp)) {
                            already_in = true;
                            break;
                        }
                    }

                    if (!already_in) {
                        initial_cubes[i].intersections.push_back(tmp);
                    }
                }
            }
        }

        if (initial_cubes[i].intersections.size()) {
            initial_cubes[i].calc_intersections();
        }

    }

    long vol = 0;

    for (auto& c : initial_cubes) {
        if (c.status == true) {
            long xxx = c.volume();
            vol += xxx;
        }
    }

    a.part1 = std::to_string(vol);


    // part 2

    for (size_t i = 0; i < cubes.size()-1; ++i) {
        for (size_t j = i+1; j < cubes.size(); ++j) {
            if (cubes[i].intersect(cubes[j])) {
                if (cubes[i].status == true) {
                    auto tmp = cubes[i].get_intersection(cubes[j]);

                    bool already_in = false;
                    for (auto& c : cubes[i].intersections) {
                        if (c.equal(tmp)) {
                            already_in = true;
                            break;
                        }
                    }

                    if (!already_in) {
                        cubes[i].intersections.push_back(tmp);
                    }
                }
            }
        }

        if (cubes[i].intersections.size()) {
            cubes[i].calc_intersections();
        }

    }

    vol = 0;

    for (auto& c : cubes) {
        if (c.status == true) {
            long xxx = c.volume();
            vol += xxx;
        }
    }
    a.part2 = std::to_string(vol);

    return a;
}
