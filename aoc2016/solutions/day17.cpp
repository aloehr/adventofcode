#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <queue>



#include "aoch/AOCSolutionTypes.hpp"


using uint = unsigned int;

static void byte_to_hex(char byte, std::array<char, 33>& buf, size_t p) {
    std::array<char, 16> convert {'0', '1', '2', '3', '4', '5', '6', '7',
                            '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    buf[p] = convert[(byte >> 4) & 0xF];
    buf[p+1] = convert[byte & 0xF];
}

static void md5_to_char(uint a, uint b, uint c, uint d, std::array<char, 33>& buf) {
    for (size_t i = 0; i < 4; ++i) {
        byte_to_hex((a >> i*8) & 0xFF, buf, 2*i);
        byte_to_hex((b >> i*8) & 0xFF, buf, 8 + 2*i);
        byte_to_hex((c >> i*8) & 0xFF, buf, 16 + 2*i);
        byte_to_hex((d >> i*8) & 0xFF, buf, 24 + 2*i);
    }

    buf[32] = '\0';
}

static uint left_rotate(const uint x, const uint times) {
    return (x << times) | (x >> (32 - times));
}

static void md5_hash(const std::string& msg, std::array<char, 33>& out) {


    const std::array<uint, 64> s = {
        7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
        5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
        4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
        6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
    };

    const std::array<uint, 64> K = {
         0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
         0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
         0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
         0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
         0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
         0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
         0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
         0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
         0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
         0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
         0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
         0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
         0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
         0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
         0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
         0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

    uint a0 = 0x67452301;   // A
    uint b0 = 0xefcdab89;   // B
    uint c0 = 0x98badcfe;   // C
    uint d0 = 0x10325476;   // D

    // padding msg
    std::array<unsigned char, 64> msg_internal;

    size_t msg_begin_ptr = 0;
    bool used_padding_one = false;
    bool all_blocks_done = false;

    while (!all_blocks_done) {
        msg_internal.fill(0);
        size_t idx = 0;
        for (; idx < 64 && msg_begin_ptr + idx < msg.size(); ++idx) {
            msg_internal[idx] = static_cast<unsigned char>(msg[msg_begin_ptr + idx]);
        }

        if (!used_padding_one && idx < 64) {
            msg_internal[idx] = 0x80;
            used_padding_one = true;
        }

        msg_begin_ptr += idx;

        if (msg_begin_ptr >= msg.size()) {
            if (idx < 56) {
                all_blocks_done = true;
            }
        }


        if (all_blocks_done) {
            long *length_ptr = reinterpret_cast<long *>(msg_internal.data() + 56);
            *length_ptr = msg.size() * 8;
        }

        std::array<uint, 16> M;

        for (size_t i = 0; i < M.size(); ++i) {
            M[i] = msg_internal[i*4 +3] << 24 | msg_internal[i*4 + 2] << 16 | msg_internal[i*4 + 1] << 8 | msg_internal[i*4 + 0];
        }

        uint A = a0;
        uint B = b0;
        uint C = c0;
        uint D = d0;

        for (uint i = 0; i < 64; ++i) {
            uint F, g;

            if (i < 16) {
                F = (B & C) | (~B & D);
                g = i;
            } else if (i < 32) {
                F = (D & B) | (~D & C);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                F = B ^ C ^ D;
                g = (3 * i + 5) % 16;
            } else {
                F = C ^ (B | ~D);
                g = (7 * i) % 16;
            }

            F = F + A + K[i] + M[g];
            A = D;
            D = C;
            C = B;
            B = B + left_rotate(F, s[i]);
        }

        a0 = a0 + A;
        b0 = b0 + B;
        c0 = c0 + C;
        d0 = d0 + D;
    }

    md5_to_char(a0, b0, c0, d0, out);
}

struct State2 {
    std::array<int, 2> pos {0, 0};
    std::string path_taken = "";
    int heuristic;

    void update_heuristic(std::array<int, 2> goal) {
        heuristic = path_taken.size() + goal[0] - pos[0] + goal[1] - pos[1];
    }

    bool operator<(const State2& o) const {
        return heuristic > o.heuristic;
    }
};

bool valid_pos(const std::array<int, 2>& p) {
    return p[0] >= 0 && p[1] >= 0 && p[0] <= 3 && p[1] <= 3;
}

std::pair<std::string, size_t> solve(std::string salt) {
    std::array<int, 2> goal {3, 3};

    State2 init_state;
    init_state.update_heuristic(goal);

    std::priority_queue<State2> pq;
    pq.push(init_state);

    std::array<char, 33> md5;

    std::pair<std::string, size_t> res = std::make_pair("", 0);

    while (pq.size()) {
        auto c = pq.top();
        pq.pop();

        if (c.pos == goal) {
            if (res.first == "") {
                res.first = c.path_taken;
            }

            if (res.second < c.path_taken.size()) {
                res.second = c.path_taken.size();
            }

            continue;
        }

        std::array<char, 4> possible_moves {'D', 'R', 'U', 'L'};

        md5_hash(salt + c.path_taken, md5);

        for (auto m : possible_moves) {

            auto pos_copy = c.pos;

            if (m == 'D') {
                if (md5[1] <= 'a') continue;
                pos_copy[0] += 1;
            } else if (m == 'U') {
                if (md5[0] <= 'a') continue;
                pos_copy[0] -= 1;
            } else if (m == 'R') {
                if (md5[3] <= 'a') continue;
                pos_copy[1] += 1;
            } else if (m == 'L') {
                if (md5[2] <= 'a') continue;
                pos_copy[1] -= 1;
            }

            if (!valid_pos(pos_copy)) continue;

            auto next_state = c;
            next_state.pos = pos_copy;
            next_state.path_taken.push_back(m);
            next_state.update_heuristic(goal);

            pq.push(next_state);
        }
    }

    return res;
}

aoch::Result solve_day17(aoch::Input& in) {
    aoch::Result r;

    auto res = solve(in[0]);

    r.part1 = res.first;
    r.part2 = std::to_string(res.second);

    return r;
}
