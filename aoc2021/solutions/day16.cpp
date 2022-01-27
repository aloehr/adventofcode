#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#include <map>

#include "aoch/AOCSolutionTypes.hpp"


struct packet {
    int version;
    int type_id;

    long value;
    std::vector<packet> subpackets;
};

int sum_ver_numbers(const packet& p) {
    int sum = 0;

    sum += p.version;

    for (const auto& sp : p.subpackets) {
        sum += sum_ver_numbers(sp);
    }

    return sum;
}

long evaluate_packet(const packet& p) {
    long a = 0;
    long b = 0;
    switch (p.type_id) {
        case 0:
            // sum
            a = 0;

            for (auto& sp : p.subpackets) {
                a += evaluate_packet(sp);
            }

            return a;

        case 1:
            //product
            a = 1;

            for (auto& sp : p.subpackets) {
                a *= evaluate_packet(sp);
            }

            return a;

        case 2:
            // min
            a = evaluate_packet(p.subpackets[0]);

            for (size_t i = 0; i < p.subpackets.size(); ++i) {
                a = std::min(a, evaluate_packet(p.subpackets[i]));
            }

            return a;

        case 3:
            // max
            a = evaluate_packet(p.subpackets[0]);

            for (size_t i = 0; i < p.subpackets.size(); ++i) {
                a = std::max(a, evaluate_packet(p.subpackets[i]));
            }

            return a;

        case 4:
            // value
            return p.value;

        case 5:
            // greater

            a = evaluate_packet(p.subpackets[0]);
            b = evaluate_packet(p.subpackets[1]);

            return a > b;

        case 6:
            // less

            a = evaluate_packet(p.subpackets[0]);
            b = evaluate_packet(p.subpackets[1]);

            return a < b;

        case 7:
            // eq

            a = evaluate_packet(p.subpackets[0]);
            b = evaluate_packet(p.subpackets[1]);

            return a == b;

        default:
            return 0;
    }
}

std::string get_bits(const std::string& d, size_t& idx, const size_t len) {
    std::string ret = d.substr(idx, len);
    idx += len;

    return ret;
}

packet parse_packet(const std::string& d, size_t& idx) {
    packet p;

    p.version = std::stoi(get_bits(d, idx, 3), nullptr, 2);
    p.type_id = std::stoi(get_bits(d, idx, 3), nullptr, 2);

    if (p.type_id == 4) {
        // literal value packet
        p.value = 0;
        while (d[idx++] == '1') {
            p.value+= std::stoi(get_bits(d, idx, 4), nullptr, 2);
            p.value <<= 4;
        }

        p.value += std::stoi(get_bits(d, idx, 4), nullptr, 2);

    }
    else {
        // operator packet
        if (d[idx++] == '0') {
            // next 15 bits, total bitcount of subpackets
            int c = std::stoi(get_bits(d, idx, 15), nullptr, 2);

            size_t s = idx;

            while (idx < s + c) {
                p.subpackets.push_back(parse_packet(d, idx));
            }
        } else {
            // next 11 bits, total count of subpackets
            int c = std::stoi(get_bits(d, idx, 11), nullptr, 2);

            while (c--) {
                p.subpackets.push_back(parse_packet(d, idx));
            }
        }
    }

    return p;
}

packet parse_packet(const std::string& d) {
    size_t idx = 0;
    return parse_packet(d, idx);
}

aoch::Result solve_day16(aoch::Input& in) {

    aoch::Result a;

    std::map<char, std::string> dict = {
        {'0', "0000"},
        {'1', "0001"},
        {'2', "0010"},
        {'3', "0011"},
        {'4', "0100"},
        {'5', "0101"},
        {'6', "0110"},
        {'7', "0111"},
        {'8', "1000"},
        {'9', "1001"},
        {'A', "1010"},
        {'B', "1011"},
        {'C', "1100"},
        {'D', "1101"},
        {'E', "1110"},
        {'F', "1111"}
    };

    std::stringstream ss;

    for (auto c : in[0]) {
        ss << dict[c];
    }

    packet p = parse_packet(ss.str());

    a.part1 = std::to_string(sum_ver_numbers(p));


    // part 2
    a.part2 = std::to_string(evaluate_packet(p));

    return a;
}
