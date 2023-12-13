#include <array>
#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"


using ImgType = std::vector<std::string_view>;

size_t difference_vertical(const ImgType& img, size_t l, size_t wanted_diff = 0) {
    size_t diff = 0;

    for (size_t row_idx = 0; row_idx < img.size(); ++row_idx) {
        size_t i = l;
        size_t j = l+1;

        while (i < img[row_idx].size() && j < img[row_idx].size()) {
            if (img[row_idx][i] != img[row_idx][j]) {
                diff++;

                if (diff > wanted_diff) return diff;
            }

            i--;
            j++;
        }
    }

    return diff;
}

size_t difference_horizontal(const ImgType& img, size_t l, size_t wanted_diff = 0) {
    size_t diff = 0;

    for (size_t col_idx = 0; col_idx < img[0].size(); ++col_idx) {
        size_t i = l;
        size_t j = l+1;

        while (i < img.size() && j < img.size()) {
            if (img[i][col_idx] != img[j][col_idx]) {
                diff++;

                if (diff > wanted_diff) return diff;
            }

            i--;
            j++;
        }
    }

    return diff;
}

using ReflectionLine = std::array<size_t, 2>;
using ReflectionLineOpt = std::optional<ReflectionLine>;

ReflectionLineOpt check_for_reflection(const ImgType& img, ReflectionLineOpt ignored_reflection_line = std::nullopt, size_t wanted_diff = 0) {
    // check for vertical reflection line
    for (size_t i = 0; i < img[0].size() - 1; ++i) {
        if (difference_vertical(img, i, wanted_diff) == wanted_diff) {
            ReflectionLine ref_line = {0, i+1};
            if (!ignored_reflection_line || *ignored_reflection_line != ref_line) {
                return ref_line;
            }
        }
    }

    // check for horizontal reflection line
    for (size_t i = 0; i < img.size() - 1; ++i) {
        if (difference_horizontal(img, i, wanted_diff) == wanted_diff) {
            ReflectionLine ref_line = {i+1, 0};
            if (!ignored_reflection_line || *ignored_reflection_line != ref_line) {
                return ref_line;
            }
        }
    }

    return std::nullopt;
}

aoch::Result solve_day13(aoch::Input& in) {
    aoch::Result a;

    // adding an empty line at the end for easier processing
    in.push_back("");

    ImgType img;
    unsigned int sum1 = 0;
    unsigned int sum2 = 0;

    for (const auto& l : in) {
        if (l != "") {
            img.emplace_back(l);
        } else {
            auto reflection_line = check_for_reflection(img);

            // there should always be exactly one reflection line
            assert(reflection_line);
            sum1 += (*reflection_line)[0] * 100 + (*reflection_line)[1];

            auto reflection_line2 = check_for_reflection(img, reflection_line, 1);

            // there should be exactly one altered pixel that leads to a new reflection line
            assert(reflection_line2);
            sum2 += (*reflection_line2)[0] * 100 + (*reflection_line2)[1];

            img.clear();
        }
    }

    a.part1 = std::to_string(sum1);
    a.part2 = std::to_string(sum2);

    return a;
}
