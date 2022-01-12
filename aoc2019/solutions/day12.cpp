#include <cmath>
#include <iostream>

#include "../aoc.hpp"


struct moon {

    int pos[3] = {0, 0, 0};
    int vel[3] = {0, 0, 0};


    int energy() const {
        int pot_energy = 0;
        int kin_energy = 0;
        for (int i = 0; i < 3; ++i) {
            pot_energy += std::abs(pos[i]);
            kin_energy += std::abs(vel[i]);
        }

        return pot_energy * kin_energy;
    }

    std::string to_string() const {
        std::string r = "<p[x ";
        r += std::to_string(this->pos[0]);
        r += " y " + std::to_string(this->pos[1]) + " z " + std::to_string(this->pos[2]) + "],";

        r += " v[x " + std::to_string(this->vel[0]) + " y " + std::to_string(this->vel[1]);
        r += " z " + std::to_string(this->vel[2]) + "]>";

        return r;
    }
};

void simulate_steps(std::vector<moon>& moons, int steps) {

    while (steps--) {
        for (size_t i = 0; i < moons.size()-1; ++i) {
            for (size_t j = i+1; j - moons.size(); ++j) {
                moon& a = moons[i];
                moon& b = moons[j];

                for (int k = 0; k < 3; ++k) {
                    if (a.pos[k] < b.pos[k]) {
                        a.vel[k]++;
                        b.vel[k]--;
                    }
                    else if (b.pos[k] < a.pos[k]) {
                        a.vel[k]--;
                        b.vel[k]++;
                    }
                }
            }
        }

        for (auto& m : moons) {
            for (int i = 0; i < 3; ++i) {
                m.pos[i] += m.vel[i];
            }
        }
    }
}

bool check_equal_dim(const std::vector<moon>& a, const std::vector<moon>& b, int dim) {
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i].pos[dim] != b[i].pos[dim] || a[i].vel[dim] != b[i].vel[dim]) return false;
    }

    return true;
}

unsigned long long calc_period_steps_dim(std::vector<moon> moons, int dim) {

    std::vector<moon> init_state = moons;
    unsigned long long steps = 0;

    do {

        steps++;

        for (size_t i = 0; i < moons.size()-1; ++i) {
            for (size_t j = i+1; j - moons.size(); ++j) {
                moon& a = moons[i];
                moon& b = moons[j];

                if (a.pos[dim] < b.pos[dim]) {
                    a.vel[dim]++;
                    b.vel[dim]--;
                }
                else if (b.pos[dim] < a.pos[dim]) {
                    a.vel[dim]--;
                    b.vel[dim]++;
                }
            }
        }

        for (auto& m : moons) {
            m.pos[dim] += m.vel[dim];
        }


    } while (!check_equal_dim(moons, init_state, dim));

    return steps;
}

unsigned long long gcd(unsigned long long a, unsigned long long b) {
    while (b != 0) {
        unsigned long long tmp = b;
        b = a % b;
        a = tmp;
    }

    return a;
}

unsigned long long lcm(unsigned long long a, unsigned long long b) {
    return a * b / gcd(a, b);
}

unsigned long long lcm(const std::vector<unsigned long long> vals) {

    unsigned long long current_lcm = vals[0];
    for (size_t i = 1; i < vals.size(); ++i) {
        current_lcm = lcm(current_lcm, vals[i]);
    }
    return current_lcm;
}

answer solve_day12(input& in) {

    answer a;
    in.pop_back();

    std::vector<moon> moons;
    moons.reserve(in.size());

    for (auto l : in) {
        moon m;

        size_t ptr = l.find('=');
        l = l.substr(ptr+1);
        m.pos[0] = std::stoi(l);
        ptr = l.find('=');
        l = l.substr(ptr+1);
        m.pos[1] = std::stoi(l);
        ptr = l.find('=');
        l = l.substr(ptr+1);
        m.pos[2] = std::stoi(l);

        moons.push_back(m);
    }

    auto moons_copy = moons;
    simulate_steps(moons, 1000);


    int total_energy = 0;

    for (auto& m : moons) {
        total_energy += m.energy();
    }

    a.part1 = std::to_string(total_energy);

    std::vector<unsigned long long> periods;

    for (int i = 0; i < 3; ++i) {
        periods.push_back(calc_period_steps_dim(moons_copy, i));
    }

    a.part2 = std::to_string(lcm(periods));

    return a;
}
