#include <functional>
#include <iostream>
#include <string>

#include <aoch/AOCSolutionTypes.hpp>


aoch::Result solve_day12(aoch::Input& in) {
    aoch::Result a;

    const size_t rows = in.size();
    const size_t columns = in[0].size();

    std::vector<char> visited(rows * columns);

    enum {AREA, PERIMETER, CORNERS};
    std::vector<std::array<int, 3>> region_counts;

    std::function<void(size_t, size_t, int)> populate_region = [&] (size_t x, size_t y, int region_id) {
        visited[y * columns + x] = 1;

        auto& region_count = region_counts[region_id];

        region_count[AREA]++;

        const char cur_region_symbol = in[y][x];

        const bool is_top_same_region    =  y > 0            && in[y - 1][x    ] == cur_region_symbol;
        const bool is_right_same_region  = (x < columns - 1) && in[y    ][x + 1] == cur_region_symbol;
        const bool is_bottom_same_region = (y < rows - 1)    && in[y + 1][x    ] == cur_region_symbol;
        const bool is_left_same_region   =  x > 0            && in[y    ][x - 1] == cur_region_symbol;


        if (is_top_same_region && !visited[(y - 1) * columns + x]) {
            populate_region(x, y - 1, region_id);
        }

        if (is_right_same_region && !visited[y * columns + x + 1]) {
            populate_region(x + 1, y, region_id);
        }

        if (is_bottom_same_region && !visited[(y + 1) * columns + x]) {
            populate_region(x, y + 1, region_id);
        }

        if (is_left_same_region && !visited[y * columns + x - 1]) {
            populate_region(x - 1, y, region_id);
        }

        // inner corners detection
        region_count[CORNERS] +=
            (is_top_same_region    && is_right_same_region  && in[y - 1][x + 1] != cur_region_symbol) +
            (is_right_same_region  && is_bottom_same_region && in[y + 1][x + 1] != cur_region_symbol) +
            (is_bottom_same_region && is_left_same_region   && in[y + 1][x - 1] != cur_region_symbol) +
            (is_left_same_region   && is_top_same_region    && in[y - 1][x - 1] != cur_region_symbol);

        if (is_left_same_region && is_top_same_region && is_right_same_region && is_bottom_same_region) return;

        region_count[PERIMETER] += !is_top_same_region + !is_right_same_region + !is_bottom_same_region + !is_left_same_region;

        // outer corners detection
        region_count[CORNERS] +=
            (!is_top_same_region    && !is_right_same_region ) +
            (!is_right_same_region  && !is_bottom_same_region) +
            (!is_bottom_same_region && !is_left_same_region  ) +
            (!is_left_same_region   && !is_top_same_region   );
    };

    for (size_t y = 0; y < rows; ++y) {
        for (size_t x = 0; x < columns; ++x) {
            if (visited[y * columns + x]) continue;
            region_counts.push_back({0, 0, 0});
            populate_region(x, y, region_counts.size() - 1);
        }
    }

    int total_price  = 0;
    int total_price2 = 0;

    for (size_t i = 0; i < region_counts.size(); ++i) {
        const auto& region_count = region_counts[i];
        total_price  += region_count[AREA] * region_count[PERIMETER];
        total_price2 += region_count[AREA] * region_count[CORNERS];
    }

    a.part1 = std::to_string(total_price);
    a.part2 = std::to_string(total_price2);

    return a;
}
