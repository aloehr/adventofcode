#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>
#include <aoch/string.hpp>


int bits_count(size_t val) {
    int count = 0;

    while (val) {
        count++;
        val >>= 1;
    }

    return count;
}

template<typename T>
std::string bin2string(T val) {
    std::string ret;

    while (val) {
        ret.push_back((val & 1) + '0');
        val >>= 1;
    }

    std::reverse(ret.begin(), ret.end());

    return ret;
}

template<typename T>
std::string bin2string(T val, int num_bits) {
    std::string ret(num_bits, 'X');

    while ((num_bits--)) {
        ret[num_bits] = (val & 1) + '0';
        val >>= 1;
    }

    return ret;
}

struct data {
    size_t a;
    size_t a_mask;
};

struct PC {
    long long a;
    long long b;
    long long c;

    size_t ip;

    std::vector<int> mem;
    std::vector<int> out;


    PC(const aoch::Input& d) : ip(0) {
        size_t l = d[0].find_last_of(' ');
        a = std::stoll(d[0].substr(l+1));
        l = d[1].find_last_of(' ');
        b = std::stoll(d[1].substr(l+1));
        l = d[2].find_last_of(' ');
        c = std::stoll(d[2].substr(l+1));

        auto t = aoch::split(d[4]);

        for (auto d : aoch::split(t[1], ',')) {
            mem.push_back(d[0] - '0');
        }
    }

    long long get_combo_operand(const size_t i) const {
        if (mem[i] < 4)  return mem[i];
        if (mem[i] == 4) return a;
        if (mem[i] == 5) return b;
        if (mem[i] == 6) return c;
        else {
            std::cout << "Error, unknown operand value: " << i << std::endl;
            return 0;
        }
    }

    void run() {
        while (ip < mem.size()) {
            switch (mem[ip]) {
                case 0: // adv
                    a >>= get_combo_operand(ip + 1);
                    ip += 2;
                    break;
                case 1: // bxl
                    b ^= mem[ip + 1];
                    ip += 2;
                    break;
                case 2: // bst
                    b = get_combo_operand(ip + 1) % 8;
                    ip += 2;
                    break;
                case 3: // jnz
                    if (a) ip = mem[ip+1];
                    else ip += 2;
                    break;
                case 4: // bxc
                    b ^= c;
                    ip += 2;
                    break;
                case 5: // out
                    out.push_back(get_combo_operand(ip + 1) % 8);
                    ip += 2;
                    break;
                case 6: // bdv
                    b = a >> get_combo_operand(ip + 1);
                    ip += 2;
                    break;
                case 7: // cdv
                    c = a >> get_combo_operand(ip + 1);
                    ip += 2;
                    break;
                default:
                    std::cout << "Error, skipping unknown instruction: " <<
                        mem[ip] << std::endl;
                    ip += 2;
            }
        }
    }

    void print() const {
        std::cout << "A: " << a << " B: " << b << " C: " << c << '\n';
        std::cout << "mem: ";

        for (auto n : mem) std::cout << n << " ";
        std::cout << std::endl;

        std::cout << "out: ";
        for (auto n : out) std::cout << n << " ";
        std::cout << std::endl;
    }

    std::string out_to_string() const {
        std::string ret;
        ret.reserve(2 * out.size() - 1);

        for (size_t i = 0; i < out.size(); ++i) {
            ret.push_back(out[i] + '0');
            if (i < out.size() - 1) ret.push_back(',');
        }

        return ret;
    }

    size_t calc() const {
        const int val_first_xor_b  = mem[3];
        const int val_second_xor_b = mem[11];

        std::array<std::vector<data>, 8> out_masks;

        for (int wanted_out_val = 0; wanted_out_val < 8; ++wanted_out_val) {
            for (int first_3_a_bits = 0; first_3_a_bits < 8; ++first_3_a_bits) {
                int b = (~first_3_a_bits & val_first_xor_b) |
                        (first_3_a_bits & (~val_first_xor_b & 7));

                int c = b ^ wanted_out_val ^ val_second_xor_b;

                int overlap_mask = (7 << b) & 7;
                bool works = ((c << b) & overlap_mask) ==
                             (first_3_a_bits & overlap_mask);

                if (works) {
                    size_t a = first_3_a_bits | (c << b);
                    size_t a_mask = 7 | (7 << b);

                    out_masks[wanted_out_val].push_back({a, a_mask});
                }
            }
        }

        std::vector<data> cur_masks;
        std::vector<data> new_masks;

        cur_masks.insert(
            cur_masks.end(),
            out_masks[mem[0]].cbegin(),
            out_masks[mem[0]].cend()
        );

        for (size_t i = 1; i < mem.size(); ++i) {
            int wanted = mem[i];

            for (const auto& m1 : out_masks[wanted]) {
                size_t val = m1.a << (3 * i);
                size_t mask = m1.a_mask << (3 * i);

                if (bits_count(val) > 48) continue;

                for (auto& m2 : cur_masks) {
                    if ((m2.a & mask) == (val & m2.a_mask)) {
                        new_masks.push_back({m2.a | val, m2.a_mask | mask});
                    }
                }
            }

            std::swap(cur_masks, new_masks);
            new_masks.clear();
        }

        const auto cmp = [] (const data& a, const data& b) { return a.a < b.a; };

        return std::min_element(cur_masks.cbegin(), cur_masks.cend(), cmp)->a;
    }
};

aoch::Result solve_day17(aoch::Input& in) {
    aoch::Result a;

    PC p(in);
    p.run();

    a.part1 = p.out_to_string();
    a.part2 = std::to_string(p.calc());

    return a;
}
