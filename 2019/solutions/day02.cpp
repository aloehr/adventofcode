#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "../aoc.hpp"


using memory = std::vector<int>;

int execute(memory mem) {
    size_t instr_ptr = 0;
    int reg1 = 0;
    int reg2 = 0;

    while (mem[instr_ptr] != 99) {
        switch (mem[instr_ptr]) {
            case 1: 
                // add
                reg1 = mem[mem[instr_ptr+1]];
                reg2 = mem[mem[instr_ptr+2]];
                mem[mem[instr_ptr+3]] = reg1 + reg2;
                break;
            case 2:
                // multiply
                reg1 = mem[mem[instr_ptr+1]];
                reg2 = mem[mem[instr_ptr+2]];
                mem[mem[instr_ptr+3]] = reg1 * reg2;
                break;
            default:
                std::cout << "Error: unknown opcode '" << mem[instr_ptr] << "'" << std::endl;
                return 1;
                break;
        }

        instr_ptr += 4;
    }

    return mem[0];
}

answer solve_day02(input& in) {

    answer a;
    in.pop_back();

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

    mem[1] = 12;
    mem[2] = 2;

    a.part1 = std::to_string(execute(mem));

    bool found = false;

    for (int noun = 0; noun < 100 && !found; ++noun) {
        for (int verb = 0; verb < 100 && !found; ++verb) {
            mem[1] = noun;
            mem[2] = verb;

            int res = execute(mem);

            if (res == 19690720) {
                a.part2 = std::to_string((noun * 100 + verb)); 
                found = true;
            }
        }
    }   

    return a;
}
