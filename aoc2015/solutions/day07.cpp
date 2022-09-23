#include <map>
#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"

enum class gate_type {AND, OR, NOT, RSHIFT, LSHIFT, WIRE};

struct gate {
    gate_type type;
    std::string input1_id;
    std::string input2_id;
    std::string output_id;
    unsigned int arg;
};

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

void evaluate(std::vector<gate> gates, std::map<std::string, unsigned int>& wire_outputs) {

    // 16 bit mask
    const unsigned int mask = 0x0000FFFF;

    while (gates.size()) {
        for (size_t i = 0; i < gates.size(); ++i) {

            gate& g = gates[i];

            if (wire_outputs.count(g.input1_id) && (g.input2_id == "" || wire_outputs.count(g.input2_id))) {
                switch (g.type) {
                    case gate_type::AND:
                        wire_outputs[g.output_id] = wire_outputs[g.input1_id] & wire_outputs[g.input2_id];
                        break;
                    case gate_type::OR:
                        wire_outputs[g.output_id] = wire_outputs[g.input1_id] | wire_outputs[g.input2_id];
                        break;
                    case gate_type::LSHIFT:
                        wire_outputs[g.output_id] = (wire_outputs[g.input1_id] << g.arg) & mask;
                        break;
                    case gate_type::RSHIFT:
                        wire_outputs[g.output_id] = wire_outputs[g.input1_id] >> g.arg;
                        break;
                    case gate_type::NOT:
                        wire_outputs[g.output_id] = (~wire_outputs[g.input1_id]) & mask;
                        break;
                    case gate_type::WIRE:
                        wire_outputs[g.output_id] = wire_outputs[g.input1_id];
                        break;
                    default:
                        std::cout << "shouldn't have reached this case..." << std::endl;
                        break;
                }

                gates.erase(gates.begin() + i);
                break;
            }
        }
    }

}

aoch::Result solve_day07(aoch::Input& in) {
    aoch::Result r;

    std::map<std::string, unsigned int> wire_outputs;

    // and gates can have 1 has one of their inputs instead of a wire_id, so we act like 1 is a wire_id with output 1
    wire_outputs["1"] = 1;
    std::vector<gate> unfinished_gates;

    for (auto& l : in) {
        auto tokens = aoch::split(l);

        if (tokens.size() == 3) {

            if (is_digit(tokens[0][0])) {
                wire_outputs[tokens[2]] = (unsigned int) std::stoul(tokens[0]);
            } else {
                unfinished_gates.push_back({gate_type::WIRE, tokens[0], "", tokens[2], 0});
            }
        } else if (tokens.size() == 4) {
            unfinished_gates.push_back({gate_type::NOT, tokens[1], "", tokens[3], 0});
        } else if (tokens[1] == "AND") {
            unfinished_gates.push_back({gate_type::AND, tokens[0], tokens[2], tokens[4], 0});
        } else if (tokens[1] == "OR") {
            unfinished_gates.push_back({gate_type::OR, tokens[0], tokens[2], tokens[4], 0});
        } else if (tokens[1] == "LSHIFT") {
            unfinished_gates.push_back({gate_type::LSHIFT, tokens[0], "", tokens[4], (unsigned int) std::stoul(tokens[2])});
        } else if (tokens[1] == "RSHIFT") {
            unfinished_gates.push_back({gate_type::RSHIFT, tokens[0], "", tokens[4], (unsigned int) std::stoul(tokens[2])});
        } else {
            std::cout << "something went wrong, should never get here!" << std::endl;
        }
    }

    std::map<std::string, unsigned int> wire_outputs_copy = wire_outputs;

    evaluate(unfinished_gates, wire_outputs);
    r.part1 = std::to_string(wire_outputs["a"]);

    // part 2
    unsigned int tmp = wire_outputs["a"];

    std::swap(wire_outputs, wire_outputs_copy);

    // we don't have to remove the "xxx -> b" rule because it was a "number -> wire" rule wich we inserted directly into wire_outputs
    wire_outputs["b"] = tmp;

    evaluate(unfinished_gates, wire_outputs);
    r.part2 = std::to_string(wire_outputs["a"]);

    return r;
}
