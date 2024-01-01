#include<array>
#include<cassert>
#include<cmath>
#include<iostream>
#include<string>
#include<vector>

#include<aoch/AOCSolutionTypes.hpp>
#include<aoch/string.hpp>


using PosT = std::array<long long, 3>;

bool is_zero(long double a) {
    const long double epsilon = 1e-11;

    return std::abs(a) < epsilon;
}

void zero_idx_in_row2_with_row1(const std::vector<long double>& row1, std::vector<long double>& row2, size_t idx) {
    long double factor = row2[idx] / row1[idx];
    row2[idx] = 0.0;

    for (size_t i = idx+1; i < row2.size(); ++i) {
        row2[i] -= factor * row1[i];
    }
}

enum SolutionType {ONE_SOLUTION, NO_SOLUTION, INFINITE_SOLUTIONS};

std::pair<SolutionType, std::vector<long double>> gauss_elim(std::vector<std::vector<long double>> mat) {
    std::vector<long double> res;

    for (size_t col = 0; col < mat[0].size()-2; ++col) {
        for (size_t row = col+1; row < mat.size(); ++row) {
            if (!is_zero(mat[row][col])) {
                zero_idx_in_row2_with_row1(mat[col], mat[row], col);
            }
        }
    }

    if (is_zero(mat[mat.size()-1][mat[0].size()-2])) {
        if (!is_zero(mat[mat.size()-1][mat[0].size()-1])) {
            return std::make_pair(SolutionType::NO_SOLUTION, res);
        } else {
            return std::make_pair(SolutionType::INFINITE_SOLUTIONS, res);
        }
    }

    res.insert(res.cend(), mat[0].size()-1, 0.0);

    for (size_t row = mat.size()-1; row < mat.size(); --row) {
        long double x = mat[row][mat[row].size()-1];

        for (size_t col = mat[row].size()-2; col > row; --col) {
            x -= mat[row][col] * res[col];
        }

        res[row] = x / mat[row][row];
    }

    return std::make_pair(SolutionType::ONE_SOLUTION, res);
}


struct Hailstone{
    PosT pos;
    PosT vel;

    Hailstone(const std::string& str) {
        auto tokens = aoch::split(str);

        this->pos[0] = std::stoll(tokens[0]);
        this->pos[1] = std::stoll(tokens[1]);
        this->pos[2] = std::stoll(tokens[2]);

        this->vel[0] = std::stoll(tokens[4]);
        this->vel[1] = std::stoll(tokens[5]);
        this->vel[2] = std::stoll(tokens[6]);
    }

    std::array<double, 2> calc_intersection_point(const Hailstone& o) const {
        double k1 = this->vel[1];
        double k2 = -1. * o.vel[1];
        double k3 = o.pos[1] - this->pos[1];

        if (k1 != 0.) {
            double factor = static_cast<double>(this->vel[1]) / static_cast<double>(this->vel[0]);

            k1 -= factor * this->vel[0];
            k2 += factor * o.vel[0];
            k3 -= factor * (o.pos[0] - this->pos[0]);
        }

        if (k2 != 1.) {
            k3 /= k2;
            k2 /= k2;
        }

        double x2 = k3;

        double x1 = (o.pos[0] + x2 * o.vel[0] - this->pos[0]) / static_cast<double>(this->vel[0]);

        return {x1, x2};
    }

    friend std::ostream& operator<<(std::ostream& os, const Hailstone& hs) {
        os << "Hailstone{p: (" << hs.pos[0] << ", " << hs.pos[1] << ", " << hs.pos[2];
        return os << "), v: (" << hs.vel[0] << ", " << hs.vel[1] << ", " << hs.vel[2] << ")}";
    }
};

aoch::Result solve_day24(aoch::Input& in) {
    aoch::Result a;

    std::vector<Hailstone> hailstones;
    hailstones.reserve(in.size());

    for (const auto& l : in) {
        hailstones.emplace_back(l);
    }

    double min = 2e14;
    double max = 4e14;
    int count = 0;

    for (size_t i = 0; i < hailstones.size()-1; ++i) {
        for (size_t j = i+1; j < hailstones.size(); ++j) {
            const Hailstone& a = hailstones[i];
            const Hailstone& b = hailstones[j];
            auto res = a.calc_intersection_point(b);

            // they are parallel
            if (std::isinf(res[0])) continue;

            // they would have collided in the past, so we don't count them
            if (res[0] < 0.0 || res[1] < 0.0) continue;

            double x = res[0] * a.vel[0] + a.pos[0];
            double y = res[0] * a.vel[1] + a.pos[1];

            if (x >= min && x <= max && y >= min && y <= max) {
                count++;
            }
        }
    }

    a.part1 = std::to_string(count);


    // part 2
    std::vector<std::vector<long double>> mat1(4);
    std::vector<std::vector<long double>> mat2(4);

    Hailstone& h1 = hailstones[0];

    for (size_t i = 0; i < 4; ++i) {
        Hailstone& h2 = hailstones[i+1];

        mat1[i].push_back(h1.pos[1] - h2.pos[1]);
        mat2[i].push_back(h1.pos[2] - h2.pos[2]);

        mat1[i].push_back(h1.vel[0] - h2.vel[0]);
        mat2[i].push_back(h1.vel[0] - h2.vel[0]);

        mat1[i].push_back(h2.pos[0] - h1.pos[0]);
        mat2[i].push_back(h2.pos[0] - h1.pos[0]);

        mat1[i].push_back(h2.vel[1] - h1.vel[1]);
        mat2[i].push_back(h2.vel[2] - h1.vel[2]);

        mat1[i].push_back(h1.pos[1] * h1.vel[0] - h2.pos[1] * h2.vel[0] + h2.pos[0] * h2.vel[1] - h1.pos[0] * h1.vel[1]);
        mat2[i].push_back(h1.pos[2] * h1.vel[0] - h2.pos[2] * h2.vel[0] + h2.pos[0] * h2.vel[2] - h1.pos[0] * h1.vel[2]);
    }

    auto res1 = gauss_elim(mat1);
    auto res2 = gauss_elim(mat2);

    assert(res1.first == SolutionType::ONE_SOLUTION && res2.first == SolutionType::ONE_SOLUTION);

    PosT p {
        static_cast<long long>(std::round(res1.second[3])),
        static_cast<long long>(std::round(res1.second[1])),
        static_cast<long long>(std::round(res2.second[1])),
    };

    a.part2 = std::to_string(p[0] + p[1] + p[2]);

    return a;
}
