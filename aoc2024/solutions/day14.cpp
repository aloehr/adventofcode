#include <array>
#include <complex>
#include <iostream>
#include <string>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>
#include <aoch/string.hpp>


using Point = std::complex<int>;

struct Robot {
    Point pos;
    Point vel;
};

aoch::Result solve_day14(aoch::Input& in) {
    aoch::Result a;

    const int w = 101;
    const int h = 103;

    Point map_boundaries = {w, h};

    const int half_w = w / 2;
    const int half_h = h / 2;

    std::vector<std::vector<char>> img(h, std::vector<char>(w, '.'));
    std::vector<std::vector<char>> img2(h, std::vector<char>(w, '.'));

    std::vector<Robot> robots;
    robots.reserve(in.size());

    for (auto& l : in) {
        auto t = aoch::split(l);

        auto t1 = aoch::split(t[0], '=');
        auto t2 = aoch::split(t1[1], ',');

        auto t3 = aoch::split(t[1], '=');
        auto t4 = aoch::split(t3[1], ',');

        Robot r;

        r.pos = {std::stoi(t2[0]), std::stoi(t2[1])};
        r.vel = {std::stoi(t4[0]), std::stoi(t4[1])};

        robots.push_back(std::move(r));
    }

    int sim_steps = 0;

    const size_t christmas_tree_threshold = robots.size() * 0.6666;

    auto has_neighbors = [&img, w, h] (int x, int y) {
        if (x > 0     && img[y    ][x - 1] == '#') return true;
        if (y > 0     && img[y - 1][x    ] == '#') return true;
        if (y < h - 1 && img[y + 1][x    ] == '#') return true;
        if (x < w - 1 && img[y    ][x + 1] == '#') return true;

        return false;
    };

    while (sim_steps < 11000) {
        sim_steps++;

        for (auto& r : robots) {
            img[r.pos.imag()][r.pos.real()] = '.';

            r.pos += r.vel + map_boundaries;
            r.pos = {r.pos.real() % w, r.pos.imag() % h};

            img2[r.pos.imag()][r.pos.real()] = '#';
        }

        std::swap(img, img2);

        if (sim_steps == 100) {
            std::array<int, 4> quadrant_count{};

            for (auto& r : robots) {
                if      (r.pos.real() < half_w && r.pos.imag() < half_h) quadrant_count[0]++;
                else if (r.pos.real() < half_w && r.pos.imag() > half_h) quadrant_count[1]++;
                else if (r.pos.real() > half_w && r.pos.imag() < half_h) quadrant_count[2]++;
                else if (r.pos.real() > half_w && r.pos.imag() > half_h) quadrant_count[3]++;
            }

            a.part1 = std::to_string(quadrant_count[0] * quadrant_count[1] * quadrant_count[2] * quadrant_count[3]);
        }

        size_t c = 0;
        for (auto& r :robots) {
            c += has_neighbors(r.pos.real(), r.pos.imag());
        }

        // we assume that if more than 2/3 of the robots have at least one neighbour
        // we will have hit the sim step where the robots have built the christmas tree.
        if (c > christmas_tree_threshold) {
            a.part2 = std::to_string(sim_steps);
            break;
        }
    }

    return a;
}
