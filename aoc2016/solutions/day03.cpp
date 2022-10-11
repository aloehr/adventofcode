#include <cassert>
#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


bool is_triangle(const std::array<int, 3>& t) {
    return t[0] + t[1] > t[2] && t[1] + t[2] > t[0] && t[0] + t[2] > t[1];
}

aoch::Result solve_day03(aoch::Input& in) {
    aoch::Result r;

    std::vector<std::array<int, 3>> triangles;
    triangles.reserve(in.size());

    std::vector<std::array<int, 3>> triangles2;
    triangles2.reserve(in.size());

    for (size_t i = 0; i < in.size(); i += 3) {
        auto t0 = aoch::split(in[i + 0]);
        auto t1 = aoch::split(in[i + 1]);
        auto t2 = aoch::split(in[i + 2]);

        triangles.push_back({
            std::stoi(t0[0]),
            std::stoi(t0[1]),
            std::stoi(t0[2])
        });

        triangles.push_back({
            std::stoi(t1[0]),
            std::stoi(t1[1]),
            std::stoi(t1[2])
        });

        triangles.push_back({
            std::stoi(t2[0]),
            std::stoi(t2[1]),
            std::stoi(t2[2])
        });

        triangles2.push_back({
            std::stoi(t0[0]),
            std::stoi(t1[0]),
            std::stoi(t2[0])
        });

        triangles2.push_back({
            std::stoi(t0[1]),
            std::stoi(t1[1]),
            std::stoi(t2[1])
        });

        triangles2.push_back({
            std::stoi(t0[2]),
            std::stoi(t1[2]),
            std::stoi(t2[2])
        });
    }

    int valid_triangles = 0;
    for (const auto& t : triangles) {
        valid_triangles += is_triangle(t);
    }

    r.part1 = std::to_string(valid_triangles);


    // part 2
    valid_triangles= 0;
    for (const auto& t : triangles2) {
        valid_triangles += is_triangle(t);
    }

    r.part2 = std::to_string(valid_triangles);

    return r;
}
