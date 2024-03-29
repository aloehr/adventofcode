#include<algorithm>
#include<array>
#include<cmath>
#include<cassert>
#include<deque>
#include<functional>
#include<iostream>
#include<map>
#include<numeric>
#include<string>

#include<aoch/AOCSolutionTypes.hpp>
#include<aoch/string.hpp>


enum ModuleType { BROADCASTER, FLIPFLOP, CONJUNCTION, NONE };
enum Pulse { LOW, HIGH };

struct Module;
using PulseType = std::tuple<Module*, Module*, Pulse>;

struct Module {
    ModuleType type = ModuleType::NONE;
    std::string name = "";
    std::vector<Module*> destinations;
    std::vector<Module*> inputs;
    std::vector<Pulse> state = { Pulse::LOW };

    std::vector<PulseType> receive_signal(Module *source, Pulse p) {
        std::vector<PulseType> ret;

        if (this->type == ModuleType::BROADCASTER) {
            for (auto d : this->destinations) {
                ret.push_back(std::make_tuple(this, d, p));
            }
        } else if (this->type == ModuleType::FLIPFLOP) {
            if (p == Pulse::LOW) {
                this->state[0] = this->state[0] == Pulse::LOW ? Pulse::HIGH : Pulse::LOW;

                for (auto d : this->destinations) {
                    ret.push_back(std::make_tuple(this, d, this->state[0]));
                }
            }
        } else if (this->type == ModuleType::CONJUNCTION) {
            size_t idx = std::find(this->inputs.cbegin(), this->inputs.cend(), source) - this->inputs.cbegin();
            this->state[idx] = p;

            Pulse send_pulse = Pulse::LOW;
            if (std::any_of(this->state.cbegin(), this->state.cend(), std::logical_not<bool>())) {
                send_pulse = Pulse::HIGH;
            }

            for (auto d : this->destinations) {
                ret.push_back(std::make_tuple(this, d, send_pulse));
            }
        }

        return ret;
    }

    void reset_state() {
        for (size_t i = 0; i < this->state.size(); ++i) {
            this->state[i] = Pulse::LOW;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Module& m) {
        os << "Module{name: " << m.name << ", type: " << m.type;
        os << ", state: [";
        for (size_t i = 0; i < m.state.size(); ++i) {
            os << m.state[i];
            if (i < m.state.size() - 1) {
                os << ", ";
            }
        }

        os << "], destinations: [";
        for (size_t i = 0; i < m.destinations.size(); ++i) {
            os << m.destinations[i]->name;
            if (i < m.destinations.size() - 1) {
                os << ", ";
            }
        }

        os << "], inputs: [";
        for (size_t i = 0; i < m.inputs.size(); ++i) {
            os << m.inputs[i]->name;
            if (i < m.inputs.size() - 1) {
                os << ", ";
            }
        }

        return os << "]}";

    }
};

unsigned int run_until_receive_high(Module* start, Module* end) {
    std::deque<PulseType> q;
    unsigned int button_pressed_count = 0 ;

    bool keep_running = true;

    while (keep_running) {
        button_pressed_count++;
        q.push_back(std::make_tuple(nullptr, start, Pulse::LOW));

        while (q.size()) {
            Module *from, *to;
            Pulse p;

            std::tie(from, to, p) = q.front();
            q.pop_front();

            if (to == end && p == Pulse::HIGH) {
                keep_running = false;
            }
            auto next_pulses = to->receive_signal(from, p);

            q.insert(q.end(), next_pulses.begin(), next_pulses.end());
        }
    }

    return button_pressed_count;
}

aoch::Result solve_day20(aoch::Input& in) {
    aoch::Result a;

    // parsing
    std::vector<Module*> modules;
    std::map<std::string, Module*> module_name_to_module;

    for (const auto& l : in) {
        auto tokens = aoch::split(l);

        ModuleType type = ModuleType::BROADCASTER;
        if (tokens[0][0] == '%') {
            type = ModuleType::FLIPFLOP;
            tokens[0] = tokens[0].substr(1);
        } else if (tokens[0][0] == '&') {
            type = ModuleType::CONJUNCTION;
            tokens[0] = tokens[0].substr(1);
        }

        Module *m = nullptr;
        if (module_name_to_module.count(tokens[0])) {
            m = module_name_to_module[tokens[0]];
        } else {
            modules.push_back(new Module());
            m = modules.back();
            module_name_to_module[tokens[0]] = m;
        }

        m->name = tokens[0];
        m->type = type;

        for (size_t i = 2; i < tokens.size(); ++i) {
            if (tokens[i].back() == ',') {
                tokens[i].pop_back();
            }

            Module *dest = nullptr;
            if (module_name_to_module.count(tokens[i])) {
                dest = module_name_to_module[tokens[i]];
            } else {
                modules.push_back(new Module());
                modules.back()->name = tokens[i];
                dest = modules.back();
                module_name_to_module[tokens[i]] = dest;
            }

            m->destinations.push_back(dest);
        }
    }

    for (auto m : modules) {
        for (auto o : modules) {
            if (m == o) continue;

            if (std::find(o->destinations.cbegin(), o->destinations.cend(), m) != o->destinations.cend()) {
                m->inputs.push_back(o);
            }
        }

        if (m->type == ModuleType::CONJUNCTION) {
            m->state.clear();
            m->state.insert(m->state.end(), m->inputs.size(), Pulse::LOW);
        }
    }


    // part 1
    std::deque<PulseType> q;
    unsigned int wanted_button_presses = 1000;
    Module* broadcaster_ptr = module_name_to_module["broadcaster"];
    std::array<unsigned int, 2> total_pulses_count {0, 0};

    while (wanted_button_presses--) {
        q.push_back(std::make_tuple(nullptr, broadcaster_ptr, Pulse::LOW));

        while (q.size()) {
            Module *from, *to;
            Pulse p;

            std::tie(from, to, p) = q.front();
            q.pop_front();

            total_pulses_count[p]++;
            auto next_pulses = to->receive_signal(from, p);

            q.insert(q.end(), next_pulses.begin(), next_pulses.end());
        }
    }

    a.part1 = std::to_string(total_pulses_count[0] * total_pulses_count[1]);


    // part 2
    Module *rx = module_name_to_module["rx"];
    Module *rx_parent = rx->inputs[0];

    for (auto m : modules) {
        m->reset_state();
    }

    // checks if modules states is af they were just reset
    auto is_state_reset = [&modules] () {
        for (auto m : modules) {
            // the state of 1 input conjunctions aka inverters is not relevant for the global state
            // to be reset or not
            if (m->type == ModuleType::CONJUNCTION && m->state.size() == 1) continue;

            for (size_t i = 0; i < m->state.size(); ++i) {
                if (m->state[i] != Pulse::LOW) return false;
            }
        }

        return true;
    };

    unsigned long long result = 1;
    for (auto start : broadcaster_ptr->destinations) {
        result = std::lcm(run_until_receive_high(start, rx_parent), result);

        if (!is_state_reset()) {
            std::cout << "2023 20 p2 Error: assumption that the modules state ";
            std::cout << "is reset after a high was sent to rx_parent failed" << std::endl;
        }
    }

    a.part2 = std::to_string(result);

    for (auto m : modules) delete m;

    return a;
}
