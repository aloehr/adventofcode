#include <iostream>
#include <map>
#include <vector>
#include <cassert>

#include "../aoc.hpp"


// forward declaration
struct virtual_machine1;

using memory = std::vector<int>;
using vm_opcode_func_ptr = void (virtual_machine1::*) ();

struct instruction1 {
    std::string name = "";
    int opcode = 0;
    int instr_size = 0;
    vm_opcode_func_ptr opcode_func = nullptr;
};


struct virtual_machine1 {

    enum vm_status {
        READY,
        RUNNING,
        WAITING_FOR_INPUT,
        FINISHED
    };

    vm_status state = vm_status::READY;
    size_t instr_ptr = 0;
    size_t in_args_ptr = 0;
    int cur_opcode  = 0;
    int registers[3];
    std::string reg_modes = "000";
    bool jumped = false;
    bool debug = false;
    bool verbose = false;
    std::map<int, instruction1> instructions;
    std::string name;
    memory mem;
    memory in_args;
    memory out_args;

    virtual_machine1() : virtual_machine1("Default", {99}, {}) {}

    virtual_machine1(std::string name, memory mem, memory in_args)
        : name(name), mem(mem), in_args(in_args) {

        this->register_instruction("ADD",   1, 4, &virtual_machine1::opcode_add);
        this->register_instruction("MUL",   2, 4, &virtual_machine1::opcode_mul);
        this->register_instruction("IN",    3, 2, &virtual_machine1::opcode_in);
        this->register_instruction("OUT",   4, 2, &virtual_machine1::opcode_out);
        this->register_instruction("JNZ",   5, 3, &virtual_machine1::opcode_jnz);
        this->register_instruction("JEZ",   6, 3, &virtual_machine1::opcode_jez);
        this->register_instruction("LT",    7, 4, &virtual_machine1::opcode_lt);
        this->register_instruction("EQ",    8, 4, &virtual_machine1::opcode_eq);
        this->register_instruction("EXIT", 99, 1, &virtual_machine1::opcode_exit);
    }

    void register_instruction(std::string name, int opcode, int size, vm_opcode_func_ptr opcode_func) {
        instruction1 instr;
        instr.name = name;
        instr.opcode = opcode;
        instr.instr_size = size;
        instr.opcode_func = opcode_func;
        instructions[opcode] = instr;
    }

    void read_instruction() {
        int cur_instr = mem[instr_ptr]; 

        this->cur_opcode = cur_instr%100;
        cur_instr /= 100;

        for (int i = 0; i < 3; ++i) {
            this->reg_modes[i] = '0' + cur_instr%10;
            cur_instr /= 10;
        }

        for (int i = 0; i < this->instructions[this->cur_opcode].instr_size - 1; ++i) {
            this->registers[i] = this->mem[this->instr_ptr + i + 1];
        }
    }

    void run() {
        if (this->state != vm_status::FINISHED) {
            this->state = vm_status::RUNNING;

            while (this->state == vm_status::RUNNING) {
                read_instruction(); 
                (this->*(this->instructions[this->cur_opcode].opcode_func))();

                if (!this->jumped && this->state != vm_status::WAITING_FOR_INPUT) {
                    this->instr_ptr += this->instructions[this->cur_opcode].instr_size;
                }
                this->jumped = false;
            }
        }
    }

    int get_value(int register_idx) {
        if (this->reg_modes[register_idx] == '1') {
            return this->registers[register_idx];
        }

        return this->mem[this->registers[register_idx]];
    }

    void print_instr() {

        std::cout << this->name << " Opcode '" << this->instructions[this->cur_opcode].name << "' instr_ptr: '" 
            << this->instr_ptr << "' mem[instr_ptr]: '" << this->mem[this->instr_ptr] << "' modes: '" << this->reg_modes << "'\n";

        for (int i = 0; i < this->instructions[this->cur_opcode].instr_size - 1; ++i) {

            std::cout << "\tr[" << i << "] = '" << this->registers[i] << "' ";
            if (this->reg_modes[i] == '0') std::cout << "(mem[" << this->registers[i] << "] = '" << this->get_value(i) << "')";
            std::cout << '\n';
        }

        if (this->instructions[this->cur_opcode].name == "IN" && this->in_args_ptr < this->in_args.size()) {
            std::cout << "\targ: '" << this->in_args[this->in_args_ptr] << "'\n";
        }

    }

    void set_debug(bool b) {
        this->debug = b;
    }

    void set_in_args(memory in_args) {
        this->in_args = in_args;
    }

    void opcode_add() {
        if (this->debug) this->print_instr(); 
        this->mem[this->registers[2]] = this->get_value(0) + this->get_value(1);
        if (this->debug) std::cout << "Res: " << this->mem[this->registers[2]] << '\n';
    }

    void opcode_mul() {
        if (this->debug) this->print_instr(); 
        this->mem[this->registers[2]] = this->get_value(0) * this->get_value(1);
        if (this->debug) std::cout << "Res: " << this->mem[this->registers[2]] << '\n';
    }

    void opcode_in() {
        if (this->debug) this->print_instr(); 
        if (this->in_args_ptr >= this->in_args.size()) {
            this->state = vm_status::WAITING_FOR_INPUT;
            return;
        }
        this->mem[this->registers[0]] = this->in_args[this->in_args_ptr++]; 
        if (this->debug) std::cout << "Res: " << this->mem[this->registers[0]] << '\n';
    }

    void opcode_out() {
        if (this->debug) this->print_instr(); 
        if (verbose) std::cout << this->name << " out: '" << this->get_value(0) << "'" << '\n';
        this->out_args.push_back(this->get_value(0));

    }

    void opcode_jnz() {
        if (this->debug) this->print_instr(); 
        if (this->get_value(0) != 0) {
            this->instr_ptr = this->get_value(1);
            this->jumped = true;
        }

        if (this->debug) std::cout << "JNZ instr_ptr after: " << this->instr_ptr << '\n';
    }

    void opcode_jez() {
        if (this->debug) this->print_instr(); 
        if (this->get_value(0) == 0) {
            this->instr_ptr = this->get_value(1);
            this->jumped = true;
        }

        if (this->debug) std::cout << "JEZ instr_ptr after: " << this->instr_ptr << '\n';
    }

    void opcode_lt() {
        if (this->debug) this->print_instr(); 
        this->mem[this->registers[2]] = this->get_value(0) < this->get_value(1) ? 1 : 0;
    }

    void opcode_eq() {
        if (this->debug) this->print_instr(); 
        this->mem[this->registers[2]] = this->get_value(0) == this->get_value(1) ? 1 : 0;
    }

    void opcode_exit() {
        if (this->debug) this->print_instr(); 
        this->state = vm_status::FINISHED;
    }
};

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

        virtual_machine1 vm("VM0", mem, {phase_setting[0], 0}); 
        for (size_t i = 1; i < phase_setting.size(); ++i) {
            vm.run();

            memory out = vm.out_args;
            std::string vm_name = "VM" + std::to_string(i);

            vm = virtual_machine1(vm_name, mem, {phase_setting[i], out.back()});
        }

        vm.run();

        if (vm.out_args.back() > max_thruster_signal || best_phase_setting.empty()) {
            max_thruster_signal = vm.out_args.back();
            best_phase_setting = phase_setting;
        }

        last_phase_setting = phase_setting;
        sepa(phase_setting);
    }
    
    //std::cout << "max_thruster_signal: " << max_thruster_signal << " with phase setting: ";
    //for (auto i : best_phase_setting) std::cout << i << " ";
    //std::cout << '\n';
    

    a.part1 = std::to_string(max_thruster_signal);

    last_phase_setting.clear();
    phase_setting = {5, 6, 7, 8, 9};
    max_thruster_signal = 0;
    best_phase_setting.clear();

    while (phase_setting != last_phase_setting) {
        virtual_machine1 vm[5];

        for (int i = 0; i < 5; ++i) {
            std::string vm_name = "VM" + std::to_string(i);
            vm[i] = virtual_machine1(vm_name, mem, {phase_setting[i]});
        }

        vm[0].in_args.push_back(0);

        size_t i = 0;

        while (true) {
            size_t tmp = vm[i].out_args.size();
            vm[i].run();
            assert(vm[i].out_args.size() - tmp == 1);

            if (vm[(i+1)%5].state == virtual_machine1::vm_status::FINISHED) {
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
    
    //std::cout << "max_thruster_signal: " << max_thruster_signal << " with phase setting: ";
    //for (auto i : best_phase_setting) std::cout << i << " ";
    //std::cout << '\n';

    a.part2 = std::to_string(max_thruster_signal);

    return a;
}
