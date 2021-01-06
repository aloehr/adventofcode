#include <iostream>
#include <map>
#include <vector>

#include "../aoc.hpp"


// forward declaration
struct virtual_machine;

using memory = std::vector<int>;
using vm_opcode_func_type = void (virtual_machine::*) ();

struct instruction {
    std::string name = "";
    int opcode = 0;
    int instr_size = 0;
    vm_opcode_func_type opcode_func = nullptr;
};

struct virtual_machine {
    
    size_t instr_ptr = 0;
    size_t in_args_ptr = 0;
    int cur_opcode  = 0;
    int registers[3];
    std::string reg_modes = "000";
    bool halted = false;
    bool jumped = false;
    bool debug = false;
    std::map<int, instruction> instructions;
    std::string name;
    memory mem;
    memory in_args;
    memory out_args;

    virtual_machine(std::string name, memory mem, memory in_args) : name(name), mem(mem), in_args(in_args) {
        register_instruction("ADD",   1, 4, &virtual_machine::opcode_add);
        register_instruction("MUL",   2, 4, &virtual_machine::opcode_mul);
        register_instruction("IN",    3, 2, &virtual_machine::opcode_in);
        register_instruction("OUT",   4, 2, &virtual_machine::opcode_out);
        register_instruction("JNZ",   5, 3, &virtual_machine::opcode_jnz);
        register_instruction("JEZ",   6, 3, &virtual_machine::opcode_jez);
        register_instruction("LT",    7, 4, &virtual_machine::opcode_lt);
        register_instruction("EQ",    8, 4, &virtual_machine::opcode_eq);
        register_instruction("EXIT", 99, 1, &virtual_machine::opcode_exit);
    }

    void register_instruction(std::string name, int opcode, int size, vm_opcode_func_type opcode_func) {
        instruction instr;
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
        while (!this->halted) {
            read_instruction(); 
            (this->*(this->instructions[this->cur_opcode].opcode_func))();

            if (!this->jumped) {
                this->instr_ptr += this->instructions[this->cur_opcode].instr_size;
            }
            this->jumped = false;
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

        if (this->instructions[this->cur_opcode].name == "IN") {
            std::cout << "\targ: '" << this->in_args[this->in_args_ptr] << "'\n";
        }

    }

    void set_debug(bool b) {
        this->debug = b;
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
        this->mem[this->registers[0]] = this->in_args[this->in_args_ptr++]; 
        if (this->debug) std::cout << "Res: " << this->mem[this->registers[0]] << '\n';
    }

    void opcode_out() {
        if (this->debug) this->print_instr(); 
        //std::cout << this->name << " out: '" << this->get_value(0) << "'" << '\n';
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
        this->halted = true;

    }
};


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

    virtual_machine vm("VM1", mem, {1});
    virtual_machine vm2("VM2", mem, {5});
    //vm2.set_debug(true);

    vm.run();
    vm2.run();

    a.part1 = std::to_string(vm.out_args.back());
    a.part2 = std::to_string(vm2.out_args.back());

    return a;
}
