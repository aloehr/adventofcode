#pragma once

#include <string>
#include <vector>
#include <map>

// forward declaration
struct IntCodeVM;

using int64 = long long;
using memory = std::vector<int64>;
using vm_opcode_func_ptr = void (IntCodeVM::*) ();

struct VM_Instruction {
    std::string name = "";
    int opcode = 0;
    int instr_size = 0;
    vm_opcode_func_ptr opcode_func = nullptr;
};


class IntCodeVM {
    public:

    enum vm_status {
        READY,
        RUNNING,
        WAITING_FOR_INPUT,
        FINISHED
    };

    vm_status state = vm_status::READY;
    size_t instr_ptr = 0;
    size_t relative_base = 0;
    size_t in_args_ptr = 0;
    int cur_opcode  = 0;
    int64 registers[3];
    std::string reg_modes = "000";
    bool jumped = false;
    bool debug = false;
    bool verbose = false;
    std::map<int, VM_Instruction> instructions;
    std::string name;
    memory mem;
    memory in_args;
    memory out_args;

    IntCodeVM();
    IntCodeVM(std::string name, memory mem, memory in_args);

    void run();

    void set_debug(bool b) { this->debug = b;}
    void set_in_args(memory in_args) { this->in_args = in_args; }


    private:

    void register_instruction(std::string name, int opcode, int size,
        vm_opcode_func_ptr opcode_func);

    void read_instruction();

    int64 get_value(int reg_idx);
    size_t write_adr(int reg_idx);
    void increase_mem(size_t s);

    void print_instr();

    void opcode_add();
    void opcode_mul();
    void opcode_in();
    void opcode_out();
    void opcode_jnz();
    void opcode_jez();
    void opcode_lt();
    void opcode_eq();
    void opcode_rbo();
    void opcode_exit();
};
