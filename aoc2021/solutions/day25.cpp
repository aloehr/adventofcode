#include <iostream>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"


struct grid {

    size_t w = 0;
    size_t row_size = 0;
    size_t h = 0;
    char mask = 0x03;


    std::vector<unsigned char> data;

    grid(std::vector<std::string> in) {
        w = in[0].size();
        h = in.size();
        row_size = w/ 4 + (w% 4 != 0);
        size_t bytes_to_allocate = row_size * h;
        data.resize(bytes_to_allocate, '\0');


        for (size_t i = 0; i < h; ++i) {
            for (size_t j = 0; j < w; ++j) {
                if (in[i][j] == '>') {
                    set(i, j, 1);
                }
                else if(in[i][j] == 'v') {
                    set(i, j, 2);
                }
            }
        }
    }

    unsigned int get(size_t h, size_t w) {
        size_t w_idx = w / 4;

        char bucket = data[this->row_size * h + w_idx];
        size_t w_sub_idx = w % 4;

        return (bucket >> (w_sub_idx * 2)) & mask;
    }

    void set(size_t h, size_t w, unsigned int val) {
        char bucket = data[this->row_size * h + w/4];

        size_t bucket_idx = w % 4;
        char inv_mask = ~(mask << (bucket_idx * 2));
        bucket = bucket &  inv_mask;

        bucket |= val << (bucket_idx * 2);

        data[this->row_size * h + w/4] = bucket;
    }
};

// > = 1, v = 2
bool step(grid& g) {
    bool moved = false;

    size_t w = g.w;
    size_t h = g.h;

    for (size_t y = 0; y < h; ++y) {
        unsigned int first =  g.get(y, 0);
        unsigned int cur = first;
        for (size_t x = 0; x < w-1; ++x) {
            if (cur == 1) {
                cur = g.get(y, x+1);
                if (g.get(y, x+1) == 0) {
                    g.set(y, x, 0);
                    g.set(y, x+1, 1);
                    moved = true;
                }
            }
            else {
                cur = g.get(y, x+1);
            }
        }

        if (cur == 1) {
            if (first == 0) {
                g.set(y, w-1, 0);
                g.set(y, 0, 1);
                moved = true;
            }
        }
    }

    grid next_state = g;
    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            if (g.get(y, x) == 2) {
                if (g.get((y+1) % h, x) == 0) {
                    next_state.set(y, x, 0);
                    next_state.set((y+1) % h, x, 2);
                    moved = true;
                }
            }
        }
    }

    std::swap(g, next_state);
    return moved;
}

aoch::Result solve_day25(aoch::Input& in) {

    aoch::Result a;

    grid g(in);

    int steps = 1;
    while (step(g)) {
        steps++;
    }

    a.part1 = std::to_string(steps);
    a.part2 = "NA";

    return a;
}
