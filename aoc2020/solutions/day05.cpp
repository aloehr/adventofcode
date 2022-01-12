#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>

#include "../aoc.hpp"


int replace(std::string& in, char from, char to) {
    int count = 0;
    for (size_t i = 0; i < in.size(); ++i) {
        if (in[i] == from) {
            in[i] = to;
            count++;
        }
    }

    return count;
}

int decode_bsp(const std::string& in) {
    int min = 0;
    int max = (1 << in.size()) - 1;

    for (size_t i = 0; i < in.size(); ++i) {
        char c = in[i];

        if (c == 'A') {
            max = (min + max - 1) / 2 ;
        }
        else {
            min = (min + max + 1) / 2;
        }
    }

    return min;
}

int get_ticket_nr(const int row, const int col) {
    return row * 8 + col;
}

answer solve_day05(input& in) {

    answer a;
    in.pop_back();

    std::vector<int> ticket_nrs;

    int max_ticket_nr = 0;
    int min_row = 999999;

    for (auto& str : in) {
        std::string row_str = str.substr(0, 7);
        std::string col_str = str.substr(7);

        assert(col_str.size() == 3);

        replace(row_str, 'F', 'A');
        replace(col_str, 'L', 'A');

        int row = decode_bsp(row_str);
        int col = decode_bsp(col_str);

        if (row < min_row) min_row = row;

        int ticket_nr = get_ticket_nr(row, col);
        ticket_nrs.push_back(ticket_nr);
        if (ticket_nr > max_ticket_nr) max_ticket_nr = ticket_nr;
    }

    std::sort(ticket_nrs.begin(), ticket_nrs.end());
    int my_ticket = 0;

    for (size_t i = 0; i < ticket_nrs.size() - 1; ++i) {
        if (ticket_nrs[i] <= get_ticket_nr(min_row, 7)) continue;

        if (ticket_nrs[i+1] - ticket_nrs[i] == 2) {
            my_ticket = ticket_nrs[i] + 1;
            break;
        }
    }

    a.part1 = std::to_string(max_ticket_nr);
    a.part2 = std::to_string(my_ticket);

    return a;
}
