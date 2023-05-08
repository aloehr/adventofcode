#include <vector>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


aoch::Result solve_day02(aoch::Input& in) {

    aoch::Result a;

    std::vector<std::vector<int>> outcomes = {
        {4, 8, 3},
        {1, 5, 9},
        {7, 2, 6}
    };

    std::vector<std::vector<int>> outcomes2 = {
        {3, 4, 8},
        {1, 5, 9},
        {2, 6, 7}
    };

    int total_points = 0;
    int total_points2 = 0;

    for (const auto& l : in) {
        total_points  +=  outcomes[l[0] - 'A'][l[2] - 'X'];
        total_points2 += outcomes2[l[0] - 'A'][l[2] - 'X'];
    }

    a.part1 = std::to_string(total_points);
    a.part2 = std::to_string(total_points2);

    return a;
}
