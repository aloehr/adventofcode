#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

#include "aoc.hpp"


struct result {
    int day;
    answer ans;
    std::chrono::nanoseconds dur;
};

input read_file(std::string filename) {
    std::ifstream f(filename);
    input ret;

    if (!f.good()) {
        std::cout << "Error: unable to read file: '" << filename << "'" << std::endl;
        return ret;
    }

        std::stringbuf sb;
    while (f.good()) {
        std::string line;
        std::getline(f, line);
        ret.emplace_back(line);
    }

    // remove last line if empty
    while (ret.back().empty()) ret.pop_back();

    return ret;
}

std::string num2str(int n) {

    std::string r;

    if (n < 10) r += "0";

    r += std::to_string(n);

    return r;
}

void print_results(const std::vector<result>& results) {

    size_t p1_ans_max_length = 6;
    size_t p2_ans_max_length = 6;

    std::chrono::nanoseconds total_time(0);

    for (const auto& r : results) {
        p1_ans_max_length = std::max(p1_ans_max_length, r.ans.part1.size());
        p2_ans_max_length = std::max(p2_ans_max_length, r.ans.part2.size());

        total_time += r.dur;
    }

    std::string p1 = "Part 1";
    std::string p2 = "Part 2";
    std::string sep_line(12 + p1_ans_max_length + 2 + p2_ans_max_length + 15, '-');

    // header
    printf("%s\n", sep_line.c_str());
    printf("| Day | %*s | %*s | %13s |\n", (int) p1_ans_max_length, p1.c_str(), (int) p2_ans_max_length, p2.c_str(), "Time");
    printf("%s\n", sep_line.c_str());

    // content
    for (const auto& r : results) {
        printf(
            "|  %.2d | %*s | %*s | %10.3f ms |\n",
            r.day,
            (int) p1_ans_max_length,
            r.ans.part1.c_str(),
            (int) p2_ans_max_length,
            r.ans.part2.c_str(),
            1e-6 * r.dur.count()
        );
    }

    // total time
    printf("%s\n", sep_line.c_str());
    printf("| %*.3f ms |\n", (int) (sep_line.size() - 7), 1e-6 *  total_time.count());
    printf("%s\n", sep_line.c_str());


}

int main(int argc, char *args[]) {

    size_t single_run_day = 0;
    std::vector<result> results;

    std::vector<solve_fnc> solutions = {
        solve_day01, solve_day02, solve_day03, solve_day04, solve_day05,
        solve_day06, solve_day07, solve_day08, solve_day09, //solve_day10,
//        solve_day11, solve_day12, solve_day13, solve_day14, solve_day15,
//        solve_day16, solve_day17, solve_day18, solve_day19, solve_day20,
//        solve_day21, solve_day22, solve_day23, solve_day24, solve_day25,
    };


    if (argc == 2) {
        single_run_day = std::stoi(args[1]) - 1;

        if (single_run_day < 1 || single_run_day > 25)
            single_run_day = 0;
    }

    for (size_t i = 0; i < solutions.size(); ++i) {
        if (single_run_day && i != single_run_day) continue;

        std::string input_file = "../data/day" + num2str(i+1) + ".txt";
        input in = read_file(input_file);

        result r;
        r.day = i+1;

        auto start = std::chrono::high_resolution_clock::now();
        r.ans = solutions[i](in);
        auto end = std::chrono::high_resolution_clock::now();

        r.dur = end - start;

        results.push_back(r);
    }

    std::sort(results.begin(), results.end(), [](const result& a, const result&b) { return a.day < b.day; });
    print_results(results);

    return 0;
}
