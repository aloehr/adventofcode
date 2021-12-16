#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <bitset>
#include <cassert>

#include "../aoc.hpp"

struct packet {
    int version;
    int type_id;

    long value;
    std::vector<packet> subpackets;
};

int sum_ver_numbers(std::vector<packet> packets) {
    int sum = 0;

    for (auto& p : packets) {
        sum += p.version;

        sum += sum_ver_numbers(p.subpackets);
    }

    return sum;
}

long evaluate_packet(packet& p) {
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

packet parse_packet(std::string& d) {
    packet p;

    p.version = std::stoi(d.substr(0, 3), nullptr, 2);
    d = d.substr(3);
    p.type_id = std::stoi(d.substr(0, 3), nullptr, 2);
    d = d.substr(3);

    // literal value
    if (p.type_id == 4) {
        long val = 0L;
        while (d[0] == '1') {
            val += std::stoi(d.substr(1, 4), nullptr, 2);
            val <<= 4;
            d = d.substr(5);
        }

        val += std::stoi(d.substr(1, 4), nullptr, 2);
        p.value = val;
        d = d.substr(5);

    }
    else {
        if (d[0] == '0') {
            // next 15 bits, total bitcount of subpackets
            int c = std::stoi(d.substr(1, 15), nullptr, 2);
            d = d.substr(16);

            size_t s = d.size();

            while (d.size() > s-c) {
                p.subpackets.push_back(parse_packet(d));
            }
        } else {
            // next 11 bits, total count of subpackets
            int c = std::stoi(d.substr(1, 11), nullptr, 2);
            d = d.substr(12);

            while (c--) {
                p.subpackets.push_back(parse_packet(d));
            }
        }
    }

    return p;
}

answer solve_day16(input& in) {

    answer a;

    std::stringstream ss;
    std::stringstream ss_tmp;

    std::vector<packet> packets;

    for (auto c : in[0]) {
        ss_tmp << "0x" << c;
        std::bitset<4> bs(std::stoi(ss_tmp.str(), nullptr, 16));

        ss << bs;
        ss_tmp.str("");
    }


    std::string d = ss.str();

    while (d.size() >= 11) {
        packets.push_back(parse_packet(d));
    }

    a.part1 = std::to_string(sum_ver_numbers(packets));


    assert(packets.size() == 1);
    // part 2

    a.part2 = std::to_string(evaluate_packet(packets[0]));

    return a;
}
