#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

#include "../aoc.hpp"

std::array<long, 2> calc_line_scores(const std::string& l) {
    std::vector<char> stack;

    for (auto c : l) {
        if (c == '(' || c == '{' || c == '[' || c == '<') {
            stack.push_back(c);
        }
        else {
            char b = stack.back();
            stack.pop_back();

            // syntax error score
            if      (c == ')' && b != '(') return {3, 0};
            else if (c == ']' && b != '[') return {57, 0};
            else if (c == '}' && b != '{') return {1197, 0};
            else if (c == '>' && b != '<') return {25137, 0};
        }
    }

    // if we haven't found a syntax error we calc the completion string score
    long score = 0;
    while (stack.size()) {
        char c = stack.back();
        stack.pop_back();

        score *= 5;

        if      (c == '(') score += 1;
        else if (c == '[') score += 2;
        else if (c == '{') score += 3;
        else if (c == '<') score += 4;
    }

    return {0, score};
}

answer solve_day10(input& in) {

    answer a;

    long sum_error = 0;
    std::vector<long> compl_str_scores;

    for (const auto& l : in) {
        auto score = calc_line_scores(l);
        sum_error += score[0];

        if (score[1])
            compl_str_scores.push_back(score[1]);
    }

    a.part1 = std::to_string(sum_error);

    // wanted completion string score is the median (odd number of scores guaranteed)
    std::sort(compl_str_scores.begin(), compl_str_scores.end());
    a.part2 = std::to_string(compl_str_scores[compl_str_scores.size() >> 1]);

    return a;
}
