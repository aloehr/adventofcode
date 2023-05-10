#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


aoch::Result solve_day05(aoch::Input& in) {

    aoch::Result a;

    size_t stack_count = (in[0].length() + 1) / 4;
    size_t stack_begin_ptr = 0;
    bool found_top_first_stack = false;

    while (in[stack_begin_ptr][0] != ' ' || !found_top_first_stack) {
        if (!found_top_first_stack && in[stack_begin_ptr][0] == '[') {
            found_top_first_stack = true;
        }

        stack_begin_ptr++;
    }

    std::vector<std::vector<char>> stacks(stack_count);

    for (size_t i = stack_begin_ptr; i > 0; --i) {
        for (size_t j = 0; j < stack_count; ++j) {
            if (in[i-1][j*4+1] != ' ') {
                stacks[j].push_back(in[i-1][j*4+1]);
            }
        }
    }

    auto stacks2(stacks);

    for (size_t i = stack_begin_ptr+2; i < in.size(); ++i) {
        auto tokens = aoch::split(in[i]);

        int moves = std::stoi(tokens[1]);
        int from = std::stoi(tokens[3]) - 1;
        int to = std::stoi(tokens[5]) - 1;

        stacks2[to].insert(stacks2[to].end(), stacks2[from].end() - moves, stacks2[from].end());
        stacks2[from].erase(stacks2[from].end() - moves, stacks2[from].end());

        while (moves--) {
            stacks[to].push_back(stacks[from].back());
            stacks[from].pop_back();
        }
    }

    for (auto& s : stacks) {
        a.part1 += s.back();
    }

    for (auto& s : stacks2) {
        a.part2 += s.back();
    }

    return a;
}
