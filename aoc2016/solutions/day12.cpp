#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


struct VM {

    enum class InstructionType { CPY, INC, DEC, JNZ };

    enum class ArgType { VALUE, REG_DEST };

    struct Instruction {
        InstructionType type;

        ArgType arg1_type;
        int arg1;

        ArgType arg2_type;
        int arg2;
    };

    std::vector<Instruction> instructions;
    int regs[4] = {};
    size_t instr_ptr = 0;


    VM(std::vector<std::string> in) {

        for (const auto& l : in) {
            auto tokens = aoch::split(l);

            Instruction instr;

            if (tokens[0] == "cpy") {
                instr.type = InstructionType::CPY;

                if (tokens[1][0] < '9') {
                    instr.arg1_type = ArgType::VALUE;
                    instr.arg1 = std::stoi(tokens[1]);
                } else {
                    instr.arg1_type = ArgType::REG_DEST;
                    instr.arg1 = tokens[1][0] - 'a';
                }

                if (tokens[2][0] < '9') {
                    std::cout << "copy instr has value as reg destination\n";
                    continue;
                } else {
                    instr.arg2_type = ArgType::REG_DEST;
                    instr.arg2 = tokens[2][0] - 'a';
                }
            } else if (tokens[0] == "inc") {
                instr.type = InstructionType::INC;

                if (tokens[1][0] < '9') {
                    std::cout << "inc instr has value as reg destination\n";
                    continue;
                } else {
                    instr.arg1_type = ArgType::REG_DEST;
                    instr.arg1 = tokens[1][0] - 'a';
                }
            } else if (tokens[0] == "dec") {
                instr.type = InstructionType::DEC;

                if (tokens[1][0] < '9') {
                    std::cout << "dec instr has value as reg destination\n";
                    continue;
                } else {
                    instr.arg1_type = ArgType::REG_DEST;
                    instr.arg1 = tokens[1][0] - 'a';
                }
            } else if (tokens[0] == "jnz") {
                instr.type = InstructionType::JNZ;

                if (tokens[1][0] < '9') {
                    instr.arg1_type = ArgType::VALUE;
                    instr.arg1 = std::stoi(tokens[1]);
                } else {
                    instr.arg1_type = ArgType::REG_DEST;
                    instr.arg1 = tokens[1][0] - 'a';
                }

                if (tokens[2][0] < '9') {
                    instr.arg2_type = ArgType::VALUE;
                    instr.arg2 = std::stoi(tokens[2]);
                } else {
                    instr.arg2_type = ArgType::REG_DEST;
                    instr.arg2 = tokens[2][0] - 'a';
                }
            } else {
                std::cout << "unknown instruction: " << tokens[0] << '\n';
                continue;
            }

            instructions.push_back(instr);
        }
    }

    void reset() {
        instr_ptr = 0;

        for (size_t i = 0; i < 4; ++i)
            regs[i] = 0;
    }

    void run() {

        while (instr_ptr < instructions.size()) {

            auto& current_instr = instructions[instr_ptr];

            if (current_instr.type == InstructionType::CPY) {

                if (current_instr.arg1_type == ArgType::VALUE) {
                    regs[current_instr.arg2] = current_instr.arg1;
                } else {
                    regs[current_instr.arg2] = regs[current_instr.arg1];
                }

                instr_ptr++;
            } else if (current_instr.type == InstructionType::INC) {
                regs[current_instr.arg1]++;
                instr_ptr++;
            } else if (current_instr.type == InstructionType::DEC) {
                regs[current_instr.arg1]--;
                instr_ptr++;
            } else if (current_instr.type == InstructionType::JNZ) {

                int condition = current_instr.arg1_type == ArgType::VALUE ? current_instr.arg1 : regs[current_instr.arg1];

                if (condition) {
                    instr_ptr += current_instr.arg2_type == ArgType::VALUE ? current_instr.arg2 : regs[current_instr.arg2];
                } else {
                    instr_ptr++;
                }
            }
        }
    }

};

aoch::Result solve_day12(aoch::Input& in) {
    aoch::Result r;

    VM vm(in);

    vm.run();
    r.part1 = std::to_string(vm.regs[0]);

    vm.reset();
    vm.regs[2] = 1;

    vm.run();
    r.part2 = std::to_string(vm.regs[0]);

    return r;
}
