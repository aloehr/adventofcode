#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"


using uint = unsigned int;

uint switch_endian(const uint n) {
    return (n << 24 & 0xFF000000) |
           (n <<  8 & 0x00FF0000) |
           (n >>  8 & 0x0000FF00) |
           (n >> 24 & 0x000000FF);
}

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

uint left_rotate(const uint x, const uint times) {
    return (x << times) | (x >> (32 - times));
}

bool md5_single_block(const std::string& msg, std::array<char, 33>& out) {


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

    // since we do only a single block we can't process more than 55 bytes
    assert(msg.size() < 56);

    std::array<unsigned char, 64> msg_internal;
    msg_internal.fill(0);

    for (size_t i = 0; i < msg.size(); ++i) {
        msg_internal[i] = static_cast<unsigned char>(msg[i]);
    }

    msg_internal[msg.size()] = 0x80;

    long *length_ptr = reinterpret_cast<long *>(msg_internal.data() + 56);
    *length_ptr = msg.size() * 8;

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

    if ((a0 & 0x00F0FFFF) == 0) {
        md5_to_char(a0, b0, c0, d0, out);

        return true;
    }

    return false;
}

bool has_five_leading_zeros(const std::array<char, 33>& str) {
    return str[0] == '0' && str[1] == '0' && str[2] == '0' && str[3] == '0' && str[4] == '0';
}

aoch::Result solve_day05(aoch::Input& in) {
    aoch::Result r;

    std::string pw;
    std::string pw2 = "XXXXXXXX";

    uint idx = 0;
    uint pw2_chars_inserted = 0;
    std::array<char, 33> md5_hash;

    while (pw.size() < 8 || pw2_chars_inserted < 8) {
        if (md5_single_block(in[0] + std::to_string(idx), md5_hash)) {
            if (pw.size() < 8) {
                pw += md5_hash[5];
            }

            if (pw2_chars_inserted < 8) {
                const int pos = md5_hash[5] - '0';

                if (pos < 8) {

                    if (pw2[pos] == 'X') {
                        pw2[pos] = md5_hash[6];
                        pw2_chars_inserted++;
                    }
                }
            }
        }

        idx++;
    }

    r.part1 = pw;
    r.part2 = pw2;

    return r;
}
