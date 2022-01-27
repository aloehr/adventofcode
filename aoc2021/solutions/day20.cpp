#include <iostream>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"


using img_t = std::vector<std::string>;

img_t enhance(const img_t& img, std::string iaa, char border) {
    size_t h = img.size();
    size_t w = img[0].size();
    img_t ret(2 + h, std::string(2 + w, border));
    int c = 0;
    for (size_t j = 1; j < h-1; ++j) {
        for (size_t i = 1; i < w-1; ++i) {
            c++;
            std::string bin;
            bin += img[j-1].substr(i-1, 3);
            bin += img[j  ].substr(i-1, 3);
            bin += img[j+1].substr(i-1, 3);

            ret[j+1][i+1] = iaa[std::stoi(bin, nullptr, 2)] == '#' ? '1' : '0';
        }
    }

    return ret;
}

aoch::Result solve_day20(aoch::Input& in) {

    aoch::Result a;

    std::string& iaa = in[0];

    size_t w = in[2].size();
    size_t h = in.size() - 2;

    for (size_t j = 0; j < h; ++j) {
        for (size_t i = 0; i < w; ++i) {
            in[2+j][i] = in[2+j][i] == '#' ? '1' : '0';
        }
    }

    img_t input_img(h+4, std::string(w+6, '0'));

    for (size_t j = 0; j < h; ++j) {
            input_img[2+j] = "000" + in[2 + j] + "000";
    }

    input_img = enhance(input_img, iaa, '1');
    input_img = enhance(input_img, iaa, '0');

    int count = 0;
    for (auto& l : input_img) {
        for (auto c : l) {
            count += c == '1' ? 1 : 0;
        }
    }

    a.part1 = std::to_string(count);

    int i = 24;

    while (i--) {
        input_img = enhance(input_img, iaa, '1');
        input_img = enhance(input_img, iaa, '0');
    }


    // part 2
    count = 0;

    for (auto& l : input_img) {
        for (auto& c : l) {
            count += c == '1' ? 1 : 0;
        }
    }

    a.part2 = std::to_string(count);

    return a;
}
