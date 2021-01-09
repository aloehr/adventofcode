#include <iostream>
#include <map>
#include <vector>
#include <cassert>

#include "../aoc.hpp"

#include "IntCodeVM.hpp"


void sepa(std::vector<int>& in) {
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

answer solve_day07(input& in) {

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

    std::vector<int> last_phase_setting;
    std::vector<int> phase_setting = {0, 1, 2, 3, 4};
    int max_thruster_signal = 0;
    std::vector<int> best_phase_setting;

    while (phase_setting != last_phase_setting) {

        IntCodeVM vm("VM0", mem, {phase_setting[0], 0});
        for (size_t i = 1; i < phase_setting.size(); ++i) {
            vm.run();

            memory out = vm.out_args;
            std::string vm_name = "VM" + std::to_string(i);

            vm = IntCodeVM(vm_name, mem, {phase_setting[i], out.back()});
        }

        vm.run();

        if (vm.out_args.back() > max_thruster_signal || best_phase_setting.empty()) {
            max_thruster_signal = vm.out_args.back();
            best_phase_setting = phase_setting;
        }

        last_phase_setting = phase_setting;
        sepa(phase_setting);
    }

    a.part1 = std::to_string(max_thruster_signal);

    last_phase_setting.clear();
    phase_setting = {5, 6, 7, 8, 9};
    max_thruster_signal = 0;
    best_phase_setting.clear();

    while (phase_setting != last_phase_setting) {
        IntCodeVM vm[5];

        for (int i = 0; i < 5; ++i) {
            std::string vm_name = "VM" + std::to_string(i);
            vm[i] = IntCodeVM(vm_name, mem, {phase_setting[i]});
        }

        vm[0].in_args.push_back(0);

        size_t i = 0;

        while (true) {
            size_t tmp = vm[i].out_args.size();
            vm[i].run();
            assert(vm[i].out_args.size() - tmp == 1);

            if (vm[(i+1)%5].state == IntCodeVM::vm_status::FINISHED) {
                break;
            }
            else {
                vm[(i+1)%5].in_args.push_back(vm[i].out_args.back());
            }

            i = (i+1)%5;
        }

        if (vm[4].out_args.back() > max_thruster_signal || best_phase_setting.empty()) {
            max_thruster_signal = vm[4].out_args.back();
            best_phase_setting = phase_setting;
        }

        last_phase_setting = phase_setting;
        sepa(phase_setting);
    }

    a.part2 = std::to_string(max_thruster_signal);

    return a;
}
