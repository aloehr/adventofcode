#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>

#include "aoch/AOCSolutionsManager.hpp"
#include "aoch/fileio.hpp"


int main(int argc, char *args[]) {

    std::vector<aoch::Solve_fnc> solutions;

    DECL_AND_ADD_SOLUTION(solve_day01, solutions)
    DECL_AND_ADD_SOLUTION(solve_day02, solutions)
    DECL_AND_ADD_SOLUTION(solve_day03, solutions)
    DECL_AND_ADD_SOLUTION(solve_day04, solutions)
    DECL_AND_ADD_SOLUTION(solve_day05, solutions)
    DECL_AND_ADD_SOLUTION(solve_day06, solutions)
    DECL_AND_ADD_SOLUTION(solve_day07, solutions)
    DECL_AND_ADD_SOLUTION(solve_day08, solutions)
    DECL_AND_ADD_SOLUTION(solve_day09, solutions)
    DECL_AND_ADD_SOLUTION(solve_day10, solutions)
    // DECL_AND_ADD_SOLUTION(solve_day11, solutions)
    // DECL_AND_ADD_SOLUTION(solve_day12, solutions)
    // DECL_AND_ADD_SOLUTION(solve_day13, solutions)
    // DECL_AND_ADD_SOLUTION(solve_day14, solutions)
    // DECL_AND_ADD_SOLUTION(solve_day15, solutions)
    // DECL_AND_ADD_SOLUTION(solve_day16, solutions)
    // DECL_AND_ADD_SOLUTION(solve_day17, solutions)
    // DECL_AND_ADD_SOLUTION(solve_day18, solutions)
    // DECL_AND_ADD_SOLUTION(solve_day19, solutions)
    // DECL_AND_ADD_SOLUTION(solve_day20, solutions)
    // DECL_AND_ADD_SOLUTION(solve_day21, solutions)
    // DECL_AND_ADD_SOLUTION(solve_day22, solutions)
    // DECL_AND_ADD_SOLUTION(solve_day23, solutions)
    // DECL_AND_ADD_SOLUTION(solve_day24, solutions)
    // DECL_AND_ADD_SOLUTION(solve_day25, solutions)

    size_t run_single_day = 0;

    if (argc == 2) {
        run_single_day = std::stoi(args[1]);

        if (run_single_day < 1 || run_single_day > 25)
            run_single_day = 0;
    }

    aoch::AOCSolutionsManager manager;

    for (size_t i = 0; i < solutions.size(); ++i) {
        if (run_single_day && i != run_single_day - 1) continue;

        char buf[22];
        snprintf(buf, 22, "%02lu", i+1);

        std::string input_file = "./data/day" + std::string(buf) + ".txt";
        std::string solution_file = "./data/day" + std::string(buf) + "_solution.txt";

        auto [input_data, success] = aoch::get_lines_from_textfile(input_file);

        if (!success) {
            std::cout << "Warning: Unable to read inputfile '" << input_file;
            std::cout << "'. Solution for day " << (i+1) << " will be skipped.\n";
            continue;
        }

        std::optional<aoch::Result> er = std::nullopt;

        auto [solution_data, success2] = aoch::get_lines_from_textfile(solution_file);

        if (success2 && solution_data.size() == 2) {
            er = {solution_data[0], solution_data[1]};
        }

        manager.add_solution(i+1, solutions[i], std::move(input_data), er);
    }

    manager.run();
    manager.print_result_table();

    return 0;
}
