#include <iostream>
#include <map>
#include <vector>
#include <cassert>

#include "../aoc.hpp"

#include "IntCodeVM.hpp"


void sepa(std::vector<int64>& in) {
    size_t key_field = in.size();

    for (size_t i = in.size() - 1; i > 0; --i) {
        if (in[i-1] < in[i]) {
            key_field = i-1;
            break;
        }
    }

    // we are at last permutation (reverse order)
    if (key_field == in.size()) return;

    size_t smallest_in_tail_bigger_than_key = 0;
    for (size_t i = in.size(); i > 0; --i) {
        // because the tail is sorted, the first from the right bigger than the key is the smallest one
        if (in[i-1] > in[key_field]) {
            smallest_in_tail_bigger_than_key = i-1;
            break;
        }
    }

    std::swap(in[key_field], in[smallest_in_tail_bigger_than_key]);

    // sort the tail (because tail still in reverse sorted order swapping is sufficient)
    for (size_t i = 0; i < (in.size() - key_field - 1)/2; ++i) {
        std::swap(in[key_field + 1 + i], in[in.size() - 1 - i]);
    }
}

answer solve_day09(input& in) {

    answer a;

    std::string& data = in[0];
    memory mem;
    size_t ptr = 0;

    do {

        data = data.substr(ptr);
        mem.push_back(std::stoi(data));
        ptr = data.find(',');

        if (ptr != std::string::npos)
            ptr++;

    } while (ptr != std::string::npos);

    //mem = {109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99};
    IntCodeVM vm("VM", mem, {1});
    vm.run();

    assert(vm.out_args.size() == 1);

    a.part1 = std::to_string(vm.out_args[0]);

    IntCodeVM vm2("VM2", mem, {2});
    vm2.run();
    assert(vm2.out_args.size() == 1);

    a.part2 = std::to_string(vm2.out_args.back());

    return a;
}
