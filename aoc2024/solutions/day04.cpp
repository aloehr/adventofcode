#include <array>
#include <string>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>


bool is_equal(const aoch::Input& data, size_t y, size_t x, const std::array<int, 2>& dir, const std::string& other) {
    const size_t m = data.size();
    const size_t n = data[0].size();
    const size_t l = other.size();

    if (y + (l - 1) * dir[1] >= m || x + (l - 1) * dir[0] >= n) return false;

    size_t c = 0;
    for (size_t i = 0; i < l; ++i) {
        c += data[y][x] == other[i];

        y += dir[1];
        x += dir[0];
    }

    return c == other.size();
}

aoch::Result solve_day04(aoch::Input& in) {
    aoch::Result a;

    const size_t m = in.size();
    const size_t n = in[0].size();

    const std::vector<std::array<int, 2>> dirs = {{
        { 0,  1}, { 0, -1},
        { 1,  0},
        {-1,  0},
        { 1,  1},
        { 1, -1},
        {-1, -1},
        {-1,  1},
    }};

    int total_xmas_count = 0;
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (in[i][j] != 'X') continue;

            for (const auto& d : dirs) {
                total_xmas_count += is_equal(in, i + d[1], j + d[0], d, "MAS");
            }
        }
    }

    a.part1 = std::to_string(total_xmas_count);


    // part 2
    int total_x_mas_count = 0;
    for (size_t i = 1; i < m - 1; ++i) {
        for (size_t j = 1; j < n - 1; ++j) {
            total_x_mas_count += in[i][j] == 'A' &&
                ((in[i-1][j-1] == 'M' && in[i+1][j+1] == 'S') || (in[i-1][j-1] == 'S' && in[i+1][j+1] == 'M')) &&
                ((in[i-1][j+1] == 'M' && in[i+1][j-1] == 'S') || (in[i-1][j+1] == 'S' && in[i+1][j-1] == 'M'));
        }
    }

    a.part2 = std::to_string(total_x_mas_count);

    return a;
}
