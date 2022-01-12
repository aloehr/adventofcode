#include <vector>
#include <string>

#include "../aoc.hpp"


int solve(const std::vector<int>& in, const size_t max) {

    std::vector<int> m(max);

    for (size_t i = 0; i < in.size()-1; ++i) {
        m[in[i]] = i+1;
    }

    int last = in.back();
    size_t count = in.size();

    while (count < max) {
        if (!m[last]) {
                
            m[last] = count;
            last = 0;
        }
        else {
            int l = count - m[last];
            m[last] = count;
            last = l;
        }

        count++;
    }

    return last;
}

answer solve_day15(input& in) {
    
    answer a;

    std::vector<int> input = {2,0,1,9,5,19};

    a.part1 = std::to_string(solve(input, 2020));
    a.part2 = std::to_string(solve(input, 30000000));

    return a;
}
