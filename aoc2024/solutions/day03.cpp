#include <array>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>


bool is_digit(const char c) {
    return c >= '0' && c <= '9';
}

bool in_range_inclusive(size_t val, size_t from, size_t to) {
    return val >= from && val <= to;
}

int parse_number(const std::string& s, size_t start_pos, size_t length) {
    int val = 0;

    for (size_t i = 0; i < length; ++i) {
        const char c = s[start_pos + i];

        if (!is_digit(c)) {
            return -1;
        }

        val = val * 10 + (c & 0xF);
    }

    return val;
}

aoch::Result solve_day03(aoch::Input& in) {
    aoch::Result a;

    size_t prev_lines_size = 0;
    int sum = 0;
    std::vector<std::pair<size_t, int>> products;
    for (const auto& line : in) {

        size_t min_idx = 0;
        while (min_idx < line.size()) {
            size_t mul_pos = line.find("mul(", min_idx);

            if (mul_pos == std::string::npos) break;

            size_t opening_bracket_pos = mul_pos + 3;
            min_idx = opening_bracket_pos + 1;

            size_t comma_pos = line.find_first_of(',', opening_bracket_pos + 1);

            if (comma_pos == std::string::npos) break;

            size_t first_number_length = comma_pos - opening_bracket_pos - 1;

            if (!in_range_inclusive(first_number_length, 1, 3)) {
                continue;
            }

            min_idx = comma_pos + 1;

            int val1 = parse_number(line, opening_bracket_pos + 1, first_number_length);

            if (val1 < 0) {
                continue;
            }

            size_t closing_bracket_pos = line.find_first_of(')', comma_pos + 1);

            if (closing_bracket_pos == std::string::npos) break;

            size_t second_number_length = closing_bracket_pos - comma_pos - 1;

            if (!in_range_inclusive(second_number_length, 1, 3)) {
                continue;
            }

            min_idx = closing_bracket_pos + 1;

            int val2 = parse_number(line, comma_pos + 1, second_number_length);

            if (val2 < 0) {
                continue;
            }

            const int product = val1 * val2;
            sum += product;

            products.emplace_back(mul_pos + prev_lines_size, product);
        }

        prev_lines_size += line.size();
    }


    a.part1 = std::to_string(sum);


    // part 2
    prev_lines_size = 0;
    std::vector<std::pair<size_t, bool>> mul_states;
    mul_states.emplace_back(0, true);

    const std::array<std::string, 2> search_term = {"do()", "don't()"};

    for (const auto& line : in) {

        size_t min_idx = 0;
        while (min_idx < line.size()) {
            size_t pos = line.find(search_term[mul_states.back().second], min_idx);

            if (pos == std::string::npos) break;

            min_idx = pos + (mul_states.back().second ? 7 : 4);
            mul_states.emplace_back(pos + prev_lines_size, !mul_states.back().second);
        }

        prev_lines_size += line.size();
    }

    int sum2 = 0;
    size_t state_idx = 0;
    for (size_t i = 0; i < products.size(); ++i) {

        while (state_idx < mul_states.size() - 1 && mul_states[state_idx+1].first < products[i].first) {
            state_idx++;
        }

        if (mul_states[state_idx].second) sum2 += products[i].second;
    }

    a.part2 = std::to_string(sum2);

    return a;
}
