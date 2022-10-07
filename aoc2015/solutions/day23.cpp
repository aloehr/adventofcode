#include <iostream>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


struct VM {

    size_t instr_ptr = 0;
    unsigned int reg_a = 0;
    unsigned int reg_b = 0;
    std::vector<std::string> code;

    void reset() {
        reg_a = 0;
        reg_b = 0;
        instr_ptr = 0;
    }

    void run() {
        while (instr_ptr < code.size()) {
            auto tokens = aoch::split(code[instr_ptr]);

            std::string op_code = tokens[0];
            char reg = tokens[1][0];
            int offset = 0;

            if (op_code == "jie" || op_code == "jio" || op_code == "jmp") {
                offset = op_code == "jmp" ? std::stoi(tokens[1]) : std::stoi(tokens[2]);
            }

            if (op_code == "hlf") {
                if (reg == 'a') reg_a /= 2;
                if (reg == 'b') reg_b /= 2;
                instr_ptr++;
            } else if (op_code == "tpl") {
                if (reg == 'a') reg_a *= 3;
                if (reg == 'b') reg_b *= 3;
                instr_ptr++;
            } else if (op_code == "inc") {
                if (reg == 'a') reg_a += 1;
                if (reg == 'b') reg_b += 1;
                instr_ptr++;
            } else if (op_code == "jmp") {
                instr_ptr += offset;
            } else if (op_code == "jie") {
                if (reg == 'a') {
                    instr_ptr += reg_a % 2 == 0 ? offset : 1;
                } else if (reg == 'b') {
                    instr_ptr += reg_b % 2 == 0 ? offset : 1;
                } else {
                    std::cout << "uknown register: " << reg << std::endl;
                }
            } else if (op_code == "jio") {
                if (reg == 'a') {
                    instr_ptr += reg_a  == 1 ? offset : 1;
                } else if (reg == 'b') {
                    instr_ptr += reg_b == 1 ? offset : 1;
                } else {
                    std::cout << "uknown register: " << reg << std::endl;
                }
            } else {
                std::cout << "unknown instruction: " << op_code << std::endl;
                instr_ptr++;
            }
        }
    }
};

aoch::Result solve_day23(aoch::Input& in) {
    aoch::Result r;

    VM vm;
    vm.code = in;
    vm.run();

    r.part1 = std::to_string(vm.reg_b);


    // part 2
    vm.reset();
    vm.reg_a = 1;
    vm.run();

    r.part2 = std::to_string(vm.reg_b);

    return r;
}
