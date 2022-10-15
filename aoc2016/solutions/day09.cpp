#include <cassert>
#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"


size_t decompressed_size(std::string str) {
    size_t pos = 0;
    size_t size = 0;

    while (pos < str.size()) {
        size_t opening_bracket = str.find('(', pos);

        if (opening_bracket == std::string::npos) {
            size += str.size() - pos;
            break;
        }

        size += opening_bracket - pos;

        size_t after_length = str.find('x', pos + 1);
        size_t closing_bracket = str.find(')', after_length + 1);

        size_t length = std::stoul(
            str.substr(opening_bracket + 1, after_length - opening_bracket - 1)
        );
        size_t repeats = std::stoul(
            str.substr(after_length + 1, closing_bracket - after_length - 1)
        );

        // we assume that markers inside markers don't exceed their parent markers str size
        assert(closing_bracket + 1 + length > str.size());

        size += repeats * decompressed_size(str.substr(closing_bracket + 1, length));
        pos = closing_bracket + 1 + length;
    }

    return size;
}

aoch::Result solve_day09(aoch::Input& in) {
    aoch::Result r;

    std::string data = in[0];
    std::string decompressed;

    size_t pos = 0;

    while (pos < data.size()) {
        if (data[pos] != '(') {
            decompressed += data[pos++];
        } else {

            pos++;
            size_t after_n  = data.find('x', pos);
            size_t closing_bracket = data.find(')', after_n);

            size_t length = std::stoul(data.substr(pos, after_n - pos));
            size_t repeats = std::stoul(data.substr(after_n + 1, closing_bracket- after_n - 1));

            std::string tmp = data.substr(closing_bracket + 1, length);

            for (size_t i = 0; i < repeats; ++i) {
                decompressed += tmp;
            }

            pos = closing_bracket + length + 1;
        }
    }

    r.part1 = std::to_string(decompressed.size());
    r.part2 = std::to_string(decompressed_size(data));

    return r;
}
