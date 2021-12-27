#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <tuple>
#include <cassert>

#include "../aoc.hpp"


struct ALU {
    enum REG { W = 0, X = 1, Y = 2, Z = 3, CONSTANT = 4, NOTHING = 5};

    using op_fnc = void (ALU::*) (REG, REG, long);

    std::map<size_t, op_fnc> op_code_fnc_mapping;
    std::map<std::string, size_t> op_name_op_code_mapping;
    std::map<size_t, std::string> op_code_op_name_mapping;

    std::vector<std::tuple<size_t, REG, REG, long>> instructions;
    std::vector<long> input;
    size_t input_ptr = 0;
    size_t instr_ptr = 0;

    bool debug_print = false;

    std::array<long, 4> registers = {0, 0, 0, 0};

    std::map<char, REG> name_register_mapping;
    std::map<REG, char> register_name_mapping;

    void register_operator(std::string name, size_t op_code, op_fnc op) {
        op_name_op_code_mapping[name] = op_code;
        op_code_op_name_mapping[op_code] = name;
        op_code_fnc_mapping[op_code] = op;
    }

    ALU() {
        register_operator("inp", 0, &ALU::op_inp);
        register_operator("add", 1, &ALU::op_add);
        register_operator("mul", 2, &ALU::op_mul);
        register_operator("div", 3, &ALU::op_div);
        register_operator("mod", 4, &ALU::op_mod);
        register_operator("eql", 5, &ALU::op_eql);

        name_register_mapping['w'] = REG::W;
        name_register_mapping['x'] = REG::X;
        name_register_mapping['y'] = REG::Y;
        name_register_mapping['z'] = REG::Z;

        register_name_mapping[REG::W] = 'w';
        register_name_mapping[REG::X] = 'x';
        register_name_mapping[REG::Y] = 'y';
        register_name_mapping[REG::Z] = 'z';
    }

    std::string registers_to_string() {
        std::stringstream ss;

        ss << "{" << registers[0] << ", " << registers[1] << ", " << registers[2] << ", " << registers[3] << "}";

        return ss.str();
    }

    void op_inp(REG a, REG b, long val) {
        if (debug_print) std::cout << "inp " << register_name_mapping[a] << " input: " << input[input_ptr] << "; " << registers_to_string();
        registers[a] = input[input_ptr++];
        if (debug_print) std::cout << " -> " << registers_to_string() << std::endl;
    }

    void op_add(REG a, REG b, long val) {
        if (debug_print) std::cout << "add " << register_name_mapping[a];
        if (b == REG::CONSTANT) {
            if (debug_print) std::cout << " " << std::to_string(val) << "; " << registers_to_string();
            registers[a] += val;
        }
        else {
            if (debug_print) std::cout << " " << register_name_mapping[b] << "; " << registers_to_string();
            registers[a] += registers[b];
        }

        if (debug_print) std::cout << " -> " << registers_to_string() << std::endl;
    }

    void op_mul(REG a, REG b, long val) {
        if (debug_print) std::cout << "mul " << register_name_mapping[a];
        if (b == REG::CONSTANT) {
            if (debug_print) std::cout << " " << std::to_string(val) << "; " << registers_to_string();
            registers[a] *= val;
        }
        else {
            if (debug_print) std::cout << " " << register_name_mapping[b] << "; " << registers_to_string();
            registers[a] *= registers[b];
        }

        if (debug_print) std::cout << " -> " << registers_to_string() << std::endl;
    }

    void op_div(REG a, REG b, long val) {
        if (debug_print) std::cout << "div " << register_name_mapping[a];
        if (b == REG::CONSTANT) {
            if (debug_print) std::cout << " " << std::to_string(val) << "; " << registers_to_string();
            registers[a] /= val;
        }
        else {
            if (debug_print) std::cout << " " << register_name_mapping[b] << "; " << registers_to_string();
            registers[a] /= registers[b];
        }

        if (debug_print) std::cout << " -> " << registers_to_string() << std::endl;
    }

    void op_mod(REG a, REG b, long val) {
        if (debug_print) std::cout << "mod " << register_name_mapping[a];
        if (b == REG::CONSTANT) {
            if (debug_print) std::cout << " " << std::to_string(val) << "; " << registers_to_string();
            registers[a] %= val;
        }
        else {
            if (debug_print) std::cout << " " << register_name_mapping[b] << "; " << registers_to_string();
            registers[a] %= registers[b];
        }

        if (debug_print) std::cout << " -> " << registers_to_string() << std::endl;
    }

    void op_eql(REG a, REG b, long val) {
        if (debug_print) std::cout << "eql " << register_name_mapping[a];
        if (b == REG::CONSTANT) {
            if (debug_print) std::cout << " " << std::to_string(val) << "; " << registers_to_string();
            registers[a] = registers[a] == val;
        }
        else {
            if (debug_print) std::cout << " " << register_name_mapping[b] << "; " << registers_to_string();
            registers[a] = registers[a] == registers[b];
        }

        if (debug_print) std::cout << " -> " << registers_to_string() << std::endl;
    }

    void run() {
        while (instr_ptr < instructions.size()) {
            auto [op_code, a, b, val] = instructions[instr_ptr++];
            (this->*(op_code_fnc_mapping[op_code]))(a, b, val);
        }
    }

    void reset() {
        instr_ptr = 0;
        input.clear();
        input_ptr = 0;
        registers[0] = 0;
        registers[1] = 0;
        registers[2] = 0;
        registers[3] = 0;
    }
};

answer solve_day24(input& in) {

    answer a;
    ALU al;

    for (auto& l : in) {
        auto tmp = split(l);

        size_t op_code = al.op_name_op_code_mapping[tmp[0]];

        ALU::REG a = al.name_register_mapping[tmp[1][0]];
        ALU::REG b = ALU::REG::NOTHING;
        long val = 0;

        if (tmp.size() == 3) {
            b = ALU::REG::CONSTANT;
            if (tmp[2][0] >= 'a' && tmp[2][0] <= 'z') {
                b = al.name_register_mapping[tmp[2][0]];
            } else {
                val = std::stol(tmp[2]);
            }
        }

        al.instructions.push_back(std::make_tuple(op_code, a, b, val));
    }


    a.part1 = std::to_string(51983999947999);

    for (auto c : a.part1) al.input.push_back(c - '0');
    al.run();

    assert(al.registers[ALU::REG::Z] == 0);

    al.reset();


    // part 2
    a.part2 = std::to_string(11211791111365);

    for (auto c : a.part2) al.input.push_back(c - '0');
    al.run();

    assert(al.registers[ALU::REG::Z] == 0);

    return a;
}
/*
    reverse engineerd instructions:
        w1 = read d
        z = w1 + 6

        w2 = read d
        z = z * 26 + w2 + 14        # z = (w1 + 6) * 26 + w2 + 14

        w3 = read d
        z = z * 26 + w3 + 13        # z = ((w1 + 6) * 26 + w2 + 12) * 26 + w3 + 13

        w4 = read d
        x = z % 26  - 14            # x = w3 + 13 - 14 = w3 - 1
        z = z / 26                  # z = (w1 + 6) * 26 + w2 + 14
        if (w4 == x) x = 0
        else x = 1
        y = 25 * x + 1
        z = z * y
        y = (w4 + 1) * x
        z = z + y

        w5 = read d
        z = z * 26 + w5 + 6         # z = ((w1 + 6) * 26 + w2 + 14) * 26 + w5 + 6

        w6 = read d
        x = z % 26 + 0              # x = w5 + 6 + 0 = w5 + 6
        z = z / 26                  # z = (w1 + 6) * 26 + w2 + 14
        if (x == w6) x = 0
        else x = 1
        y = 25 * x + 1
        z = z * y
        y = (w6 + 13) * x
        z = z + y

        w7 = read d
        x = z % 26 - 6              # x = w2 + 14 - 6 = w2 + 8
        z = z / 26                  # z = w1 + 6
        if (x == w7) x = 0
        else x = 1
        y = 25 * x + 1
        z = z * y
        y = (w7 + 6) * x
        z = z + y

        w8 = read d
        z = z * 26  + w8 + 3        # z = (w1 + 6) * 26 + w8 + 3

        w9 = read d
        x = z % 26 - 3              # x = w8 + 3 - 3 = w8
        z = z / 26                  # z = w1 + 6
        if (x == w9) x = 0
        else x = 1
        y = 25 * x + 1
        z = z * y
        y = (w9 + 8) * x
        z = z + y

        w10 = read d
        z = z * 26 + w10 + 14       # z = (w1 + 6) * 26 + w10 + 14

        w11 = read d
        z = z * 26 + w11 + 4        # z = ((w1 + 6) * 26 + w10 + 14) * 26 + w11 + 4

        w12 = read d
        x = z % 26 - 2              # x = w11 + 4 - 2 = w11 + 2
        z = z / 26                  # z = (w1 + 6) * 26 + w10 + 14
        if (x == w12) x = 0
        else x = 1
        y = 25 * x + 1
        z = z * y
        y = (w12 + 7) * x
        z = z + y

        w13 = read d
        x = z % 26 - 9              # x = w10 + 14 - 9 = w10 + 5
        z = z / 26                  # z = w1 + 6
        if (x == w13) x = 0
        else x = 1
        y = 25 * x + 1
        z = z * y
        y = (w13 + 15) * x
        z = z + y

        w14 = read d
        x = z % 26 - 2              # x = w1 + 6 - 2 = w1 + 4
        z = z / 26                  # z = 0
        if (x == w14) x = 0
        else x = 1
        y = 25 * x + 1
        z = z * y
        y = (w + 1) * x
        z = z + y


    a valid model number has the following constraints:
        w4  == w3  - 1
        w6  == w5  + 6
        w7  == w2  + 8
        w9  == w8
        w12 == w11 + 2
        w13 == w10 + 5
        w14 == w1  + 4


    max and min model number with these constraints (solutions):
        51983999947999
        11211791111365


*/
