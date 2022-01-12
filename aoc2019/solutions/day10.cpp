#include <iostream>
#include <set>
#include <utility>
#include <cassert>
#include <vector>
#include <algorithm>

#include "../aoc.hpp"


int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }

    return a;
}

struct point {
    int pos_x;
    int pos_y;
    int rel_x;
    int rel_y;
    int quadrant;
    double gradient;
    double distance;
    bool destroyed;

    bool operator<(const point& oth) const {
        if (this->quadrant != oth.quadrant) return this->quadrant < oth.quadrant;
        if (this->gradient != oth.gradient) {
            if (this->quadrant == 0) {
                return this->gradient > oth.gradient;
            }
            else if (this->quadrant == 1) {
                return this->gradient > oth.gradient;
            }
            else if (this->quadrant == 2) {
                return this->gradient > oth.gradient;
            }
            else if (this->quadrant == 3) {
                return this->gradient > oth.gradient;
            }
        }
        return this->distance < oth.distance;
    }

    void print_point() {
        std::cout << pos_x << "," << pos_y << "(" << rel_x << "," << rel_y << ")";
        std::cout << " q " << quadrant << " grad " << gradient << " dist " << distance << '\n';
    }
};

double calc_gradient(int rel_x, int rel_y) {
    int _gcd;
    while ((_gcd = std::abs(gcd(rel_x, rel_y))) != 1) {
        rel_x /= _gcd;
        rel_y /= _gcd;
    }

    return (double) rel_y / rel_x;
}

int get_quadrant(int x, int y) {

    if (y > 0 && x >= 0) return 0;
    if (y <= 0 && x > 0) return 1;
    if (y < 0 && x <= 0) return 2;
    if (y >= 0 && x < 0) return 3;

    assert(false);

    return 4;
}

answer solve_day10(input& in) {

    answer a;
    in.pop_back();


    // part 1

    int best_station_x = 0;
    int best_station_y = 0;
    int best_station_asteroids_count = 0;

    for (int y = 0; y < (int) in.size(); ++y) {
        for (int x = 0; x < (int) in[y].size(); ++x) {

            if (in[y][x] == '.') continue;

            std::set<std::pair<int, int>> already_seen;
            int asteroids_count = 0;

            for (int y2 = 0; y2 < (int) in.size(); ++y2) {
                for (int x2 = 0; x2 < (int) in[y2].size(); ++x2) {

                    if (in[y2][x2] != '#' || (x == x2 && y == y2)) continue;


                    int relative_x = x - x2;
                    int relative_y = y - y2;

                    int tmp = std::abs(gcd(relative_x, relative_y));
                    while (tmp != 1) {
                        relative_x /= tmp;
                        relative_y /= tmp;

                        tmp = std::abs(gcd(relative_x, relative_y));
                    }

                    auto p = std::make_pair(relative_x, relative_y);
                    if (already_seen.insert(p).second) {
                        asteroids_count++;
                    }
                }
            }

            if (asteroids_count > best_station_asteroids_count) {
                best_station_x = x;
                best_station_y = y;
                best_station_asteroids_count = asteroids_count;
            }

        }
    }

    a.part1 = std::to_string(best_station_asteroids_count);


    // part 2

    int station_x = best_station_x;
    int station_y = best_station_y;

    std::vector<point> positions;

    for (int y = 0; y < (int) in.size(); ++y) {
        for (int x = 0; x < (int) in[y].size(); ++x) {
            if ((y == station_y && x == station_x) || in[y][x] != '#') continue;

            point p;
            p.pos_x = x;
            p.pos_y = y;
            p.rel_x = x - station_x; // x axis is normal, incresing to the right
            p.rel_y = station_y - y; // y axis gets decremented upward
            p.quadrant = get_quadrant(p.rel_x, p.rel_y);
            p.gradient = calc_gradient(p.rel_x, p.rel_y);

            // hack: In quadrant 3 (south west quadrant) the gradient for the negative y-axis (x = 0) becomes -inf but the gradients inside the quadrant are positiv. Therefore the negativ y-axis would be sorted last in this quadrant. Since  we need it to be first (clockwise rotation around 0) the gradient gets changed to +inf.
            if (p.gradient == -1.0/0.0) p.gradient = 1.0/0.0;
            p.distance = std::abs(p.rel_x) + std::abs(p.rel_y);
            p.destroyed = false;

            positions.push_back(p);

        }
    }

    std::sort(positions.begin(), positions.end());

    int count = 0;
    double last_gradient = 0.0;;
    int last_quadrant = 4;
    size_t i = 0;
    point& p = positions[0];

    while (count != 200) {
        i %= positions.size();
        p = positions[i++];
        if (p.destroyed || (last_quadrant == p.quadrant && p.gradient == last_gradient)) continue;

        count++;
        p.destroyed = true;
        last_gradient = p.gradient;
        last_quadrant = p.quadrant;
    }

    a.part2 = std::to_string(p.pos_x * 100 + p.pos_y);

    return a;
}
