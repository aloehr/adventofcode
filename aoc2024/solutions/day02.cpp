#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>
#include <aoch/string.hpp>


aoch::Result solve_day02(aoch::Input& in) {
    aoch::Result a;

    std::vector<std::vector<int>> reports(in.size());

    size_t report_idx = 0;
    for (const auto& line : in) {
        reports[report_idx].reserve(8);

        int val = 0;
        for (size_t i = 0; i < line.size(); ++i) {
            if (line[i] == ' ') {
                reports[report_idx].push_back(val);
                val = 0;
            } else {
                val = val * 10 + (line[i] & 0xF);
            }
        }

        reports[report_idx].push_back(val);
        report_idx++;
    }

    auto is_safe_report = [] (const std::vector<int>& r) {
        int min_diff = 1;
        int max_diff = 3;

        if (r[0] < r[1]) {
            min_diff = -3;
            max_diff = -1;
        }

        for (size_t i = 0; i < r.size() - 1; ++i) {
            const int diff = r[i] - r[i+1];

            if (diff < min_diff || diff > max_diff) {
                return false;
            }
        }

        return true;
    };

    a.part1 = std::to_string(std::count_if(reports.cbegin(), reports.cend(), is_safe_report));


    //part 2
    auto is_safe_report2 = [] (const std::vector<int>& r) {
        int decreasing_count = (r[0] > r[1]) + (r[1] > r[2]) + (r[2] > r[3]);

        int min_diff = 1;
        int max_diff = 3;

        if (decreasing_count < 2) {
            min_diff = -3;
            max_diff = -1;
        }

        bool used_problem_dampener = false;
        for (size_t i = 0; i < r.size() - 1; ++i) {
            const int diff = r[i] - r[i+1];

            if (diff < min_diff || diff > max_diff) {

                if (!used_problem_dampener) {

                    // skip i+1 (last element)
                    if (i+1 == r.size() - 1) {
                        used_problem_dampener = true;
                        continue;
                    }

                    // skip i+1
                    const int diff1 = r[i] - r[i+2];
                    if (diff1 >= min_diff && diff1 <= max_diff) {
                        used_problem_dampener = true;
                        i++;
                        continue;
                    }

                    // skip i (first element)
                    if (i == 0) {
                        used_problem_dampener = true;
                        continue;
                    }

                    // skip i
                    const int diff2 = r[i-1] - r[i+1];
                    if (diff2 >= min_diff && diff2 <= max_diff) {
                        used_problem_dampener = true;
                        continue;
                    }
                }

                return false;
            }
        }

        return true;
    };

    a.part2 = std::to_string(std::count_if(reports.cbegin(), reports.cend(), is_safe_report2));

    return a;
}
