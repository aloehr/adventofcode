#include <algorithm>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <chrono>

#include "../aoc.hpp"


void replace_char(std::string& in, char from, char to) {
    for (size_t i = 0; i < in.size(); ++i) {
        if (in[i] == from) in[i] = to;
    }
}

void create_masks(const std::string& mask_str, unsigned long long& one_mask, unsigned long long& zero_mask) {
    //std::cout << "'" << mask_str << "'" << std::endl;
    std::string one_mask_str = mask_str;
    replace_char(one_mask_str, 'X', '0');
    //std::cout << "'" << one_mask_str << "'" << std::endl;
    one_mask = std::stoull(one_mask_str, nullptr, 2);

    std::string zero_mask_str = mask_str;
    replace_char(zero_mask_str, 'X', '1');
    //std::cout << "'" << zero_mask_str << "'" << std::endl;

    zero_mask = std::stoull(zero_mask_str, nullptr, 2);
}

void access_memory(std::unordered_map<unsigned long long, unsigned long long>& mem, const std::string& mask_str, unsigned long long mem_ptr, unsigned long long mem_value, unsigned long long mask_base) {

    std::vector<unsigned int> x_idxes;

    size_t idx = mask_str.find('X');
    while (idx != std::string::npos) {
        x_idxes.push_back(idx); 
        idx = mask_str.find('X', idx+1);
    }

    unsigned int lim = 1 << x_idxes.size();

    for (unsigned int i = 0; i < lim; ++i) {
        unsigned long long mask = mask_base;
        for (size_t idx = 0; idx < x_idxes.size(); ++idx) {
            if ((i >> idx) & 1U) {
                mask = mask | (1ULL << (36 - x_idxes[idx] - 1));
            }
        }
        mem[mem_ptr | mask] = mem_value;
    }
}

answer solve_day14(input& in) {

    answer a;
    in.pop_back(); // remove last blank line

    std::unordered_map<unsigned long long, unsigned long long> mem;
    std::string mask = "";
    unsigned long long sum = 0;
    for (auto &l : in) {
        if (l[1] == 'a') {
            mask = l.substr(7);
        }
        else {
            std::string tmp = l.substr(4);
            size_t idx;
            unsigned long long mem_ptr = std::stoull(tmp, &idx);

            tmp = tmp.substr(idx + 4);
            unsigned long long mem_value = std::stoull(tmp);

            unsigned long long one_mask, zero_mask;
            create_masks(mask, one_mask, zero_mask);
            mem_value &= zero_mask;
            mem_value |= one_mask;
            sum += mem_value - mem[mem_ptr];
            mem[mem_ptr] = mem_value;
        }
    }

    //part 2
    std::unordered_map<unsigned long long, unsigned long long> mem2;
    mask = "";

    for (auto &l : in) {
        if (l[1] == 'a') {
            mask = l.substr(7);
        }
        else {
            std::string tmp = l.substr(4);
            size_t idx;
            unsigned long long mem_ptr = std::stoull(tmp, &idx);

            tmp = tmp.substr(idx + 4);
            unsigned long long mem_value = std::stoull(tmp);
            std::string or_mask_str = mask;
            replace_char(or_mask_str, 'X', '0');
            unsigned long long or_mask = std::stoull(or_mask_str, nullptr, 2);
            std::string and_mask_str = mask;
            replace_char(and_mask_str, '0', '1');
            replace_char(and_mask_str, 'X', '0');
            unsigned long long and_mask = std::stoull(and_mask_str, nullptr, 2);

            std::string mask_copy = mask;
            replace_char(mask_copy, '1', '0');

            access_memory(mem2, mask_copy, (mem_ptr | or_mask) & and_mask, mem_value, or_mask);
        }
    }

    unsigned long long sum2 = 0;
    for (auto& p : mem2) {
        sum2 += p.second;
    }

    a.part1 = std::to_string(sum);
    a.part2 = std::to_string(sum2);

    return a;
}
