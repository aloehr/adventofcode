#include <iostream>
#include <string>
#include <vector>

#include "../aoc.hpp"

#include "aoch/string.hpp"


struct bingo_board {

    std::vector<std::vector<int>> board;
    std::vector<std::vector<bool>> board_marks;
    bool has_bingo;

    int winning_nr;

    bingo_board() {
        board.resize(5, std::vector<int>(5, 0));
        board_marks.resize(5, std::vector<bool>(5, false));

        has_bingo = false;
        winning_nr = -1;
    }

    void set_board(std::vector<std::vector<int>> data) {
        if (data.size() != 5) {
            std::cout << "error while setting board: input data size wrong" << std::endl;
            return;
        }

        for (const auto& line : data)  {
            if (line.size() != 5) {
                std::cout << "error while setting board: input data size wrong" << std::endl;
                return;
            }
        }

        for (unsigned int i = 0; i < 5; ++i) {
            for (unsigned int j = 0; j < 5; ++j) {
                board[i][j] = data[i][j];
            }
        }
    }

    void to_string() {
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                std::cout <<  board[i][j] << (board_marks[i][j] ? "X" : "") << " ";
            }
            std::cout << std::endl;
        }
    }

    bool mark_number(int n) {
        for (unsigned int i = 0; i < 5; ++i) {
            for (unsigned int j = 0; j < 5; ++j) {
                // assume every number appears max once
                if (board[i][j] == n) {
                    board_marks[i][j] = true;


                    //check for bingo
                    bool vert_bingo = true;
                    bool horizontal_bingo = true;
                    for (unsigned int k = 0; k < 5; ++k) {
                        vert_bingo &= board_marks[i][k];
                        horizontal_bingo &= board_marks[k][j];
                    }

                    if (vert_bingo || horizontal_bingo) {
                        has_bingo = true;
                        winning_nr = n;

                        // we got a bingo
                        return true;
                    }

                    else {
                        //mark didn't end up in a bingo
                        return false;
                    }
                }

            }
        }

        // returns false if nothing got marked
        return false;
    }

    int calc_final_score() {
        int score = 0;

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (!board_marks[i][j]) score += board[i][j];
            }
        }

        score *= winning_nr;

        return score;
    }
};

answer solve_day04(input& in) {

    answer a;

    std::vector<bingo_board> bingo_boards;
    std::vector<int> number_order;

    for (auto& s : aoch::split(in[0], ',')) {
        number_order.push_back(std::stoi(s));
    }

    std::vector<std::vector<int>> tmp(5);

    for (unsigned int i = 2; i < in.size() - 5; i += 6) {
        bingo_board cur;

        for (unsigned int j = 0; j < 5; ++j) {
            tmp[j].clear();

            for (auto& s : aoch::split(in[i + j])) {
                tmp[j].push_back(std::stoi(s));
            }
        }

        cur.set_board(tmp);
        bingo_boards.push_back(cur);
    }

    unsigned int bingo_count = 0;

    for (auto n : number_order) {
        for (auto& bb : bingo_boards) {

            // boards that already bingo'ed are not relevant anymore
            if (bb.has_bingo == true) {
                continue;
            }

            // we got a new bingo
            if (bb.mark_number(n)) {
                bingo_count++;

                // the first bingo board with a bingo
                if (bingo_count == 1) {
                    a.part1 = std::to_string(bb.calc_final_score());
                }

                // here the last bingo board got his bingo
                if (bingo_count == bingo_boards.size()) {
                    a.part2 = std::to_string(bb.calc_final_score());
                    break;
                }
            }
        }

        if (bingo_count == bingo_boards.size()) break;
    }

    return a;
}
