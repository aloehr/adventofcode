#include "IntCodeVM.hpp"

#include <iostream>

IntCodeVM::IntCodeVM() : IntCodeVM("Default", {99}, {}) {}

IntCodeVM::IntCodeVM(std::string name, memory mem, memory in_args)
    : name(name), mem(mem), in_args(in_args) {

    this->register_instruction("ADD",   1, 4, &IntCodeVM::opcode_add);
    this->register_instruction("MUL",   2, 4, &IntCodeVM::opcode_mul);
    this->register_instruction("IN",    3, 2, &IntCodeVM::opcode_in);
    this->register_instruction("OUT",   4, 2, &IntCodeVM::opcode_out);
    this->register_instruction("JNZ",   5, 3, &IntCodeVM::opcode_jnz);
    this->register_instruction("JEZ",   6, 3, &IntCodeVM::opcode_jez);
    this->register_instruction("LT",    7, 4, &IntCodeVM::opcode_lt);
    this->register_instruction("EQ",    8, 4, &IntCodeVM::opcode_eq);
    this->register_instruction("RBO",   9, 2, &IntCodeVM::opcode_rbo);
    this->register_instruction("EXIT", 99, 1, &IntCodeVM::opcode_exit);
}

void IntCodeVM::register_instruction(std::string name, int opcode, int size,
    vm_opcode_func_ptr opcode_func) {

    VM_Instruction instr;
    instr.name = name;
    instr.opcode = opcode;
    instr.instr_size = size;
    instr.opcode_func = opcode_func;
    this->instructions[opcode] = instr;
}

void IntCodeVM::read_instruction() {
    int64 cur_instr = this->mem[this->instr_ptr];

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

void IntCodeVM::run() {
    if (this->state != vm_status::FINISHED) {
        this->state = vm_status::RUNNING;

        while (this->state == vm_status::RUNNING) {
            this->read_instruction();
            (this->*(this->instructions[this->cur_opcode].opcode_func))();

            if (!this->jumped && this->state != vm_status::WAITING_FOR_INPUT) {
                this->instr_ptr += this->instructions[this->cur_opcode].instr_size;
            }
            this->jumped = false;
        }
    }
}

void IntCodeVM::increase_mem(size_t s) {
    if (s >= this->mem.size()) {
        this->mem.resize(s+1, 0);
    }
}

int64 IntCodeVM::get_value(int register_idx) {
    if (this->reg_modes[register_idx] == '1') {
        return this->registers[register_idx];
    }
    else if (this->reg_modes[register_idx] == '2') {
        size_t adr = this->relative_base + this->registers[register_idx];
        this->increase_mem(adr);
        return this->mem[adr];
    }

    size_t adr = this->registers[register_idx];
    this->increase_mem(adr);
    return this->mem[adr];
}

void IntCodeVM::print_instr() {
    std::cout << this->name << " Opcode '" << this->instructions[this->cur_opcode].name
        << "' instr_ptr: '" << this->instr_ptr << "' mem[instr_ptr]: '"
        << this->mem[this->instr_ptr] << "' modes: '" << this->reg_modes << "'\n";

    for (int i = 0; i < this->instructions[this->cur_opcode].instr_size - 1; ++i) {

        std::cout << "\tr[" << i << "] = '" << this->registers[i] << "' ";
        if (this->reg_modes[i] == '0') std::cout << "(mem[" << this->registers[i]
            << "] = '" << this->get_value(i) << "')";
        std::cout << '\n';
    }

    if (this->instructions[this->cur_opcode].name == "IN"
            && this->in_args_ptr < this->in_args.size()) {

        std::cout << "\targ: '" << this->in_args[this->in_args_ptr] << "'\n";
    }

}

size_t IntCodeVM::write_adr(int reg_idx) {
    size_t ret = this->registers[reg_idx];
    if (this->reg_modes[reg_idx] == '2') {
        ret += this->relative_base;
    }

    increase_mem(ret);
    return ret;
}

void IntCodeVM::opcode_add() {
    if (this->debug) this->print_instr();
    this->mem[this->write_adr(2)] = this->get_value(0) + this->get_value(1);
    if (this->debug) std::cout << "Res: " << this->mem[this->registers[2]] << '\n';
}

void IntCodeVM::opcode_mul() {
    if (this->debug) this->print_instr();
    this->mem[this->write_adr(2)] = this->get_value(0) * this->get_value(1);
    if (this->debug) std::cout << "Res: " << this->mem[this->registers[2]] << '\n';
}

void IntCodeVM::opcode_in() {
    if (this->debug) this->print_instr();
    if (this->in_args_ptr >= this->in_args.size()) {
        this->state = vm_status::WAITING_FOR_INPUT;
        return;
    }
    this->mem[this->write_adr(0)] = this->in_args[this->in_args_ptr++];
    if (this->debug) std::cout << "Res: " << this->mem[this->registers[0]] << '\n';
}

void IntCodeVM::opcode_out() {
    if (this->debug) this->print_instr();
    if (verbose) std::cout << this->name << " out: '" << this->get_value(0) << "'" << '\n';
    this->out_args.push_back(this->get_value(0));

}

void IntCodeVM::opcode_jnz() {
    if (this->debug) this->print_instr();
    if (this->get_value(0) != 0) {
        this->instr_ptr = this->get_value(1);
        this->jumped = true;
    }

    if (this->debug) std::cout << "JNZ instr_ptr after: " << this->instr_ptr << '\n';
}

void IntCodeVM::opcode_jez() {
    if (this->debug) this->print_instr();
    if (this->get_value(0) == 0) {
        this->instr_ptr = this->get_value(1);
        this->jumped = true;
    }

    if (this->debug) std::cout << "JEZ instr_ptr after: " << this->instr_ptr << '\n';
}

void IntCodeVM::opcode_lt() {
    if (this->debug) this->print_instr();
    this->mem[this->write_adr(2)] = this->get_value(0) < this->get_value(1) ? 1 : 0;
}

void IntCodeVM::opcode_eq() {
    if (this->debug) this->print_instr();
    this->mem[this->write_adr(2)] = (this->get_value(0) == this->get_value(1)) ? 1 : 0;
}

void IntCodeVM::opcode_rbo() {
    if (this->debug) this->print_instr();
    this->relative_base += this->get_value(0);
}

void IntCodeVM::opcode_exit() {
    if (this->debug) this->print_instr();
    this->state = vm_status::FINISHED;
}
