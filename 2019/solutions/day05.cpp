#include <iostream>
#include <map>
#include <vector>

#include "../aoc.hpp"

#include "IntCodeVM.hpp"


answer solve_day05(input& in) {

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

    IntCodeVM vm("VM1", mem, {1});
    IntCodeVM vm2("VM2", mem, {5});

    vm.run();
    vm2.run();

    a.part1 = std::to_string(vm.out_args.back());
    a.part2 = std::to_string(vm2.out_args.back());

    return a;
}
