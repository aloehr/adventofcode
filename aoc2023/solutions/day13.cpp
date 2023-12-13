#include <array>
#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"

template<typename T>
bool is_equal(const T& vectorable, size_t l) {
    size_t i = l;
    size_t j = l+1;

    while (i < vectorable.size() && j < vectorable.size()) {
        if (vectorable[i] != vectorable[j]) return false;
        i--;
        j++;
    }

    return true;
}

bool is_reflection_line_vertical(const std::vector<std::string>& img, size_t l) {
    for (const auto& row : img) {
        if (!compare(row, l)) return false;
    }

    return true;
}

bool is_reflection_line_horizontal(const std::vector<std::string>& img, size_t l) {
    return compare(img, l);
}

using ReflectionLine = std::array<size_t, 2>;
using ReflectionLineOpt = std::optional<ReflectionLine>;

ReflectionLineOpt check_for_reflection(const std::vector<std::string>& img, ReflectionLineOpt ignored_reflection_line = std::nullopt) {
    // check for vertical reflection line
    for (size_t i = 0; i < img[0].size() - 1; ++i) {
        if (is_reflection_line_vertical(img, i)) {
            ReflectionLine ref_line = {0, i+1};
            if (!ignored_reflection_line || *ignored_reflection_line != ref_line) {
                return ref_line;
            }
        }
    }

    // check for horizontal reflection line
    for (size_t i = 0; i < img.size() - 1; ++i) {
        if (is_reflection_line_horizontal(img, i)) {
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

    std::vector<std::string> img;
    unsigned int sum1 = 0;
    unsigned int sum2 = 0;

    for (const auto& l : in) {

        if (l != "") {
            img.push_back(l);
        } else {
            auto reflection_line = check_for_reflection(img);
            sum1 += (*reflection_line)[0] * 100 + (*reflection_line)[1];

            bool found_ref_line = false;

            // we just alter every pixel and check if it leads to a new reflection line
            for (size_t i = 0; i < img.size() && !found_ref_line; ++i) {
                for (size_t j = 0; j < img[i].size(); ++j) {
                    char tmp = img[i][j];
                    img[i][j] = img[i][j] == '#' ? '.' : '#';

                    auto reflection_line2 = check_for_reflection(img, reflection_line);
                    img[i][j] = tmp;

                    if (reflection_line2) {
                        sum2 += (*reflection_line2)[0] * 100 + (*reflection_line2)[1];
                        found_ref_line = true;
                        break;
                    }
                }
            }

            // there should be exactly one altered pixel that leads to a new reflection line
            assert(found_ref_line);
            img.clear();
        }
    }

    a.part1 = std::to_string(sum1);
    a.part2 = std::to_string(sum2);

    return a;
}
