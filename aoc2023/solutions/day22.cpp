#include<array>
#include<iostream>
#include<string>
#include<set>
#include<vector>

#include<aoch/AOCSolutionTypes.hpp>
#include<aoch/string.hpp>

// fwd declaration
struct Brick;

using PosT = std::array<int, 3>;
using TowerT = std::vector<std::vector<std::vector<Brick*>>>;
struct Brick {
    std::array<PosT, 2> pos;

    Brick(const std::string& str) {
        auto tokens = aoch::split(str, '~');

        for (size_t i = 0; i < 2; ++i) {
            auto pos_tokens = aoch::split(tokens[i], ',');

            this->pos[i] = {
                std::stoi(pos_tokens[0]),
                std::stoi(pos_tokens[1]),
                std::stoi(pos_tokens[2])
            };
        }
    }

    bool only_supporter(const TowerT& tower) const {
        std::set<Brick*> supported_bricks;

        for (int x = this->pos[0][0]; x <= this->pos[1][0]; ++x) {
            for (int y = this->pos[0][1]; y <= this->pos[1][1]; ++y) {
                int z = this->pos[1][2];
                if (tower[z+1][y][x] != nullptr) {
                    supported_bricks.insert(tower[z+1][y][x]);
                }
            }
        }

        if (supported_bricks.empty()) return false;

        for (auto b : supported_bricks) {
            std::set<Brick*> supporters;

            for (int x = b->pos[0][0]; x <= b->pos[1][0]; ++x) {
                for (int y = b->pos[0][1]; y <= b->pos[1][1]; ++y) {
                    int z = b->pos[0][2];
                    if (tower[z-1][y][x] != nullptr) {
                        supporters.insert(tower[z-1][y][x]);
                    }
                }
            }

            if (supporters.size() < 2) return true;
        }

        return false;
    }

    unsigned int count_all_supported_by_me(const TowerT& tower) const {
        std::set<const Brick*> visited;

        std::vector<const Brick*> q { this };

        while (q.size()) {
            const Brick* cur = q.back();
            q.pop_back();

            std::set<Brick*> supporters_for_cur;
            for (int x = cur->pos[0][0]; x <= cur->pos[1][0]; ++x) {
                for (int y = cur->pos[0][1]; y <= cur->pos[1][1]; ++y) {
                    int z = cur->pos[0][2];
                    if (tower[z-1][y][x] != nullptr) {
                        supporters_for_cur.insert(tower[z-1][y][x]);
                    }
                }
            }

            if (cur != this) {
                bool has_other_supporters = false;
                for (auto b : supporters_for_cur) {
                    if (!visited.count(b)) {
                        has_other_supporters = true;
                        break;
                    }
                }

                if (has_other_supporters) continue;
            }

            if (visited.count(cur)) continue;
            visited.insert(cur);

            for (int x = cur->pos[0][0]; x <= cur->pos[1][0]; ++x) {
                for (int y = cur->pos[0][1]; y <= cur->pos[1][1]; ++y) {
                    int z = cur->pos[1][2];
                    if (tower[z+1][y][x] != nullptr) {
                        q.push_back(tower[z+1][y][x]);
                    }
                }
            }

        }

        return visited.size() - 1;
    }

    friend std::ostream& operator<<(std::ostream& os, const Brick& b) {
        os << "Brick{";

        for (size_t i = 0; i < 2; ++i) {
            os << "p" << i << ": (";

            for (size_t j = 0; j < 3; ++j) {
                os << b.pos[i][j];
                if (j < 2) os << ", ";
            }

            os << ")";
            if (i < 1) os << ", ";
        }

        return os << "}";
    }
};

aoch::Result solve_day22(aoch::Input& in) {
    aoch::Result a;

    // bricks pos are in order (pos[0] <= pos[1]) and they differ at most in one dimension
    std::vector<Brick> bricks;
    bricks.reserve(in.size());


    for (const auto& l : in) {
        bricks.emplace_back(l);
    }

    PosT min = bricks.front().pos[0];
    PosT max = bricks.front().pos[1];

    for (const auto& b : bricks) {
        min[0] = std::min(min[0], b.pos[0][0]);
        min[1] = std::min(min[1], b.pos[0][1]);
        min[2] = std::min(min[2], b.pos[0][2]);

        max[0] = std::max(max[0], b.pos[1][0]);
        max[1] = std::max(max[1], b.pos[1][1]);
        max[2] = std::max(max[2], b.pos[1][2]);
    }

    TowerT tower(max[2] + 2, std::vector<std::vector<Brick*>>(max[1] + 1, std::vector<Brick*>(max[0] + 1, nullptr)));

    for (size_t i = 0; i < bricks.size(); ++i) {
        Brick *b = &bricks[i];
        for (int x = b->pos[0][0]; x <= b->pos[1][0]; ++x) {
            for (int y = b->pos[0][1]; y <= b->pos[1][1]; ++y) {
                for (int z = b->pos[0][2]; z <= b->pos[1][2]; ++z) {
                    tower[z][y][x] = b;
                }
            }
        }
    }

    bool moved_brick = true;
    while (moved_brick) {
        moved_brick = false;

        for (auto& b : bricks) {
            if (b.pos[0][2] == 1) continue;

            bool can_move = true;
            for (int x = b.pos[0][0]; x <= b.pos[1][0] && can_move; ++x) {
                for (int y = b.pos[0][1]; y <= b.pos[1][1] && can_move; ++y) {
                    if (tower[b.pos[0][2] - 1][y][x] != nullptr) can_move = false;
                }
            }

            if (can_move) {
                moved_brick = true;

                for (int x = b.pos[0][0]; x <= b.pos[1][0]; ++x) {
                    for (int y = b.pos[0][1]; y <= b.pos[1][1]; ++y) {
                        for (int z = b.pos[0][2]; z <= b.pos[1][2]; ++z) {
                            tower[z-1][y][x] = tower[z][y][x];
                            tower[z][y][x] = nullptr;
                        }
                    }
                }

                b.pos[0][2]--;
                b.pos[1][2]--;
            }
        }
    }

    int count = 0;
    unsigned int sum_p2 = 0;
    for (const auto& b : bricks) {
        if (!b.only_supporter(tower)) count++;
        else {
            sum_p2 += b.count_all_supported_by_me(tower);
        }
    }

    a.part1 = std::to_string(count);
    a.part2 = std::to_string(sum_p2);

    return a;
}
