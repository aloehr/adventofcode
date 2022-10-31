#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


void swap1(std::string& str, size_t p1, size_t p2) {
    char tmp = str[p1];
    str[p1] = str[p2];
    str[p2] = tmp;
}

void swap2(std::string& str, char a, char b) {

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == a) {
            str[i] = b;
        } else if (str[i] == b) {
            str[i] = a;
        }
    }
}

void rotate(std::string& str, size_t steps, bool left = false) {
    steps %= str.size();
    if (!left) {
        std::string first = str.substr(str.size() - steps, steps);
        std::string second = str.substr(0, str.size() - steps);

        str = first + second;
    } else {
        std::string first = str.substr(steps);
        std::string second = str.substr(0, steps);

        str = first + second;
    }
}

void rotate(std::string& str, char x) {
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == x) {
            rotate(str, i + 1 + (i >= 4));
            break;
        }
    }
}

void rotate_reversed(std::string& str, char x) {
    std::vector<size_t> reverse {7, 7, 2, 6, 1, 5, 0, 4};

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == x) {
            rotate(str, reverse[i]);
            break;
        }
    }
}

void reverse(std::string& str, size_t x, size_t y) {
    std::string prefix = str.substr(0, x);
    std::string reverse_this = str.substr(x, y - x + 1);
    std::string postfix = str.substr(y+1);

    str = prefix + std::string(reverse_this.rbegin(), reverse_this.rend()) + postfix;
}

void move(std::string& str, size_t x, size_t y) {
    if (x == y) return;

    char tmp = str[x];

    if ( x > y) {
        for (size_t i = x; i > y; i--) {
            str[i] = str[i-1];
        }
    } else {
        for (size_t i = x; i < y; i++) {
            str[i] = str[i+1];
        }
    }

    str[y] = tmp;
}

aoch::Result solve_day21(aoch::Input& in) {
    aoch::Result r;

    std::string pw = "abcdefgh";

    for (const auto& l : in) {
        auto tokens = aoch::split(l);

        if (tokens[0] == "rotate") {
            if (tokens[1] == "based") {
                rotate(pw, tokens[6][0]);
            } else {
                rotate(pw, std::stoul(tokens[2]), tokens[1] == "left");
            }
        } else if (tokens[0] == "swap") {
            if (tokens[1] == "position") {
                swap1(pw, std::stoul(tokens[2]), std::stoul(tokens[5]));
            } else {
                swap2(pw, tokens[2][0], tokens[5][0]);
            }
        } else if (tokens[0] == "reverse") {
            reverse(pw, std::stoul(tokens[2]), std::stoul(tokens[4]));
        } else if (tokens[0] == "move") {
            move(pw, std::stoul(tokens[2]), std::stoul(tokens[5]));
        }
    }

    r.part1 = pw;

    pw = "fbgdceah";

    // part 2
    for (size_t i = in.size(); i > 0; --i) {
        auto tokens = aoch::split(in[i-1]);

        if (tokens[0] == "rotate") {
            if (tokens[1] == "based") {
                rotate_reversed(pw, tokens[6][0]);
            } else {
                rotate(pw, std::stoul(tokens[2]), tokens[1] != "left");
            }
        } else if (tokens[0] == "swap") {
            if (tokens[1] == "position") {
                swap1(pw, std::stoul(tokens[2]), std::stoul(tokens[5]));
            } else {
                swap2(pw, tokens[2][0], tokens[5][0]);
            }
        } else if (tokens[0] == "reverse") {
            reverse(pw, std::stoul(tokens[2]), std::stoul(tokens[4]));
        } else if (tokens[0] == "move") {
            move(pw, std::stoul(tokens[5]), std::stoul(tokens[2]));
        }
    }

    r.part2 = pw;

    return r;
}
