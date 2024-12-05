#include <algorithm>
#include <array>
#include <string>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>


aoch::Result solve_day05(aoch::Input& in) {
    aoch::Result a;

    auto parse_int = [] (const std::string& s, size_t i) {
        return (s[i] & 0xF) * 10 + (s[i + 1] & 0xF);
    };

    size_t line_idx = 0;
    const size_t lines_count = in.size();

    std::array<bool, 100 * 100> rules{};

    while (in[line_idx] != "") {
        const int val1 = parse_int(in[line_idx], 0);
        const int val2 = parse_int(in[line_idx], 3);
        rules[val2 * 100 + val1] = true;

        line_idx++;
    }

    line_idx++;
    std::vector<std::vector<int>> updates;
    updates.reserve(lines_count - line_idx);

    while (line_idx < lines_count) {
        const size_t line_size = in[line_idx].size();

        std::vector<int> tmp(23);

        size_t tmp_idx = 0;
        size_t val_idx = 0;

        while (val_idx < line_size) {
            tmp[tmp_idx++] = parse_int(in[line_idx], val_idx);
            val_idx += 3;
        }

        tmp.resize(tmp_idx);

        updates.push_back(std::move(tmp));

        line_idx++;
    }

    auto is_correct_update = [&rules] (const std::vector<int>& update) {
        for (size_t i = 0; i < update.size() - 1; ++i) {
            if (rules[update[i] * 100 + update[i + 1]]) return false;
        }

        return true;
    };

    int middle_page_sum = 0;
    int middle_page_sum_corrected = 0;

    for (auto& u : updates) {
        if (is_correct_update(u)) {
            middle_page_sum += u[u.size() / 2];
        } else {
            std::nth_element(u.begin(), u.begin() + u.size() / 2, u.end(), [&rules] (int a, int b) {
                return rules[b * 100 + a];
            });

            middle_page_sum_corrected += u[u.size() / 2];
        }
    }

    a.part1 = std::to_string(middle_page_sum);
    a.part2 = std::to_string(middle_page_sum_corrected);

    return a;
}
