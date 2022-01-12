#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <map>
#include <cassert>
#include <chrono>

#include "../aoc.hpp"


using rule = std::vector<int>;
using rule_set = std::vector<rule>;


bool is_only_char(const rule& r, const std::map<int, rule_set>& rules) {
    for (auto rn : r) {
        if (rules.count(rn)) return false; 
    }

    return true;
}

long long count_rules(const rule_set& rs, std::map<int, rule_set>& rules) {
    long long res = 0;
    for (const auto& r : rs) {
        long long t = 1;
        for (auto rn : r) {
            if (rules.count(rn)) {
                t *= count_rules(rules[rn], rules);
            }
        }   

        res += t;
    }

    return res;
}

void print_rs(const rule_set& rs) {
        for (const auto& r : rs) {
            for (const auto& n : r) {
                std::cout << n << " ";
            }
            std::cout << "| "; 
        }
        std::cout << std::endl;
}

int check(std::string& m, int r_nr, std::map<int, rule_set>& rules, std::map<int, std::vector<char>>& char_rules, int idx = 0) {

    if (!rules.count(r_nr)) {
        if (m[idx] == char_rules[r_nr][0]) {
            return idx+1;
        }
        else {
            return -1;
        }
    }
    else {
        rule_set rs = rules[r_nr];
        
        int idx_copy = idx;
        for (auto& r : rs) {
            idx = idx_copy;

            for (auto n : r) {
                idx = check(m, n, rules, char_rules, idx); 
                if ((unsigned int) idx > m.size()) idx = -1;
                if (idx == -1) {
                    break;
                }
            }

            if (idx != -1) {
                return idx;
            }

        }
        
        return -1;
    }
}

bool special_case(std::string m, std::map<int, rule_set>& rules, std::map<int, std::vector<char>>& char_rules) {
    // rule 8 multiple times...
    std::vector<std::string> nxt;
    int idx = check(m, 42, rules, char_rules);
    while (idx >= 0) {
        m = m.substr(idx);
        if (m.empty()) break;
        nxt.push_back(m);
        idx = check(m, 42, rules, char_rules);
    }

    if (nxt.empty()) return false;

    for (auto cm : nxt) {
        auto cm_copy = cm;

        int c = 1;

        while (true) {
            bool good = true;
            cm = cm_copy;
            for (int j = 0; j < c; ++j) {
                int idx = check(cm, 42, rules, char_rules);

                if (idx == -1) {    
                    good = false;
                    break;
                }

                cm = cm.substr(idx);
            }

            if (!good) break;
            if (cm.empty()) break;

            for (int j = 0; j < c; ++j) {
                int idx = check(cm, 31, rules, char_rules);

                if (idx == -1) {    
                    good = false;
                    break;
                }

                cm = cm.substr(idx);
            }


            if (cm.empty()) {
                return true;
            }

            c++;
        }
    }

    return false;
}

using inverse_map_t = std::map<std::pair<int, int>, std::unordered_set<int>>;

inverse_map_t inverse_mapping(const std::map<int, rule_set>& rls) {

    inverse_map_t res;

    for (const auto& rs_p : rls) {
        for (const auto& r : rs_p.second) {
            res[std::make_pair(r[0], r[1])].insert(rs_p.first);        
        }
    }

    return res;
}

std::unordered_set<int> check_cyk(const std::string& msg, const std::map<int, std::vector<char>>& char_rules, const inverse_map_t& inv_rls) {

    std::vector<std::unordered_set<int>> empty_line(msg.size());
    std::vector<std::vector<std::unordered_set<int>>> dp(msg.size(), empty_line);

    for (size_t i = 0; i < msg.size(); ++i) {
        for (auto& cr_p : char_rules) {
            for (auto c : cr_p.second) {
                if (c == msg[i]) {
                    dp[0][i].insert(cr_p.first);
                    break;
                }
            }
        }

        assert(dp[0][i].size() == 2);
    }

    for (size_t i = 1; i < dp.size(); ++i) {
        for (size_t j = 0; j < dp[i].size() - i; ++j) {
            for (size_t k = 0; k < i; ++k) {
                for (auto a : dp[k][j]) {
                    for (auto b : dp[i-k-1][j+k+1]) {
                        auto p = std::make_pair(a, b);
                        if (inv_rls.count(p)) {
                            dp[i][j].insert(inv_rls.at(p).begin(), inv_rls.at(p).end());
                        }
                    }
                }
            }
        }
    }

    return dp.back()[0];
}

#define USE_YCK 0

answer solve_day19(input& in) {

    // parsing
    
    //auto start = std::chrono::high_resolution_clock::now();

    answer a;
    in.pop_back();

    std::vector<std::string> raw_rules;
    std::vector<std::string> msges;

    bool got_msges = false;
    for (const auto& l : in) {
        if (got_msges) {
            msges.push_back(l);
        }
        else {
            if (l.empty()) {
                got_msges = true;
                continue;
            }

            raw_rules.push_back(l);
        }
    }
    
    std::map<int, rule_set> rules;
    std::map<int, std::vector<char>> char_rules; 

    for (auto r : raw_rules) {

        size_t ptr = 0;
        
        int rule_nr = std::stoi(r, &ptr);
        ptr += 2;

        if (r[ptr] == '"') {
            char_rules[rule_nr].push_back(r[ptr+1]);
            continue;
        }

        rule_set rule_set;
        rule rule;

        while (true) {
            r = r.substr(ptr);
            rule.push_back(std::stoi(r, &ptr));
            
            if (ptr == r.size()) {
               rule_set.push_back(rule); 
               break;
            }

            else {
                ptr += 1;
            }
            if (r[ptr] == '|') {
                rule_set.push_back(rule);
                rule.clear();
                ptr += 2;
            }
        }

        rules[rule_nr] = rule_set;
    }

    //auto dur0 = std::chrono::high_resolution_clock::now() - start;


    // part 1

    //start = std::chrono::high_resolution_clock::now();

#if USE_YCK

    // transform rules into chomsky normal from (in this case we only need to edit rules like A -> B)
    for (auto& rs_p : rules) {
        for (size_t i = 0; i < rs_p.second.size(); ++i) {
            if (rs_p.second[i].size() == 1) {
                size_t replacement_rule = rs_p.second[i][0];
                rs_p.second.erase(rs_p.second.begin() + i);
                if (rules.count(replacement_rule)) {
                    for (const auto& r : rules[replacement_rule])
                        rs_p.second.push_back(r);
                }
                else {
                    if (char_rules.count(rs_p.first)) {
                        char_rules[rs_p.first].insert(char_rules[rs_p.first].end(), char_rules[replacement_rule].begin(), char_rules[replacement_rule].end());
                    }

                    else {
                        char_rules[rs_p.first] = char_rules[replacement_rule];
                    }
                }

                i--;
            }
        }
    }

    // remove empty rules
    for (auto& p : rules) {
        if (!p.second.size()) {
            rules.erase(p.first);
        }
    }

    for (const auto& rs_p : rules) {
        for (const auto& r : rs_p.second) {
            assert(r.size() == 2);
        }
    }

    int count = 0;

    for (const auto& m : msges) {
        if (check_cyk(m, char_rules, inverse_mapping(rules)).count(0)) {
            count++;
        }
    }

#else 

    int count = 0;

    for (auto& m: msges) {
        if ((unsigned int) check(m, 0, rules, char_rules) == m.size()) {
            count++;
        }
    }

#endif

    a.part1 = std::to_string(count);
    //auto dur1 = std::chrono::high_resolution_clock::now() - start;


    //part 2
    //start = std::chrono::high_resolution_clock::now();
    
#if USE_YCK

    
    rules[8].push_back({42, 8});
    rules[11].push_back({42, 999});
    rules[999].push_back({11, 31});
    
    count = 0;
    for (const auto& m : msges) {
        if (check_cyk(m, char_rules, inverse_mapping(rules)).count(0)) {
            count++;
        }
    }

#else

    count = 0;

    for (auto& m: msges) {
        if ((unsigned int) check(m, 0, rules, char_rules) == m.size() || special_case(m, rules, char_rules)) {
            count++;
            //std::cout << m << " " << special_case(m, rules, char_rules) << std::endl;
        }
    }


#endif

    a.part2 = std::to_string(count);

    //auto dur2 = std::chrono::high_resolution_clock::now() - start;

    //std::cout << "parsing: " << 1e-6 * dur0.count() << std::endl;
    //std::cout << "part 1: " << 1e-6 * dur1.count() << std::endl;
    //std::cout << "part 2: " << 1e-6 * dur2.count() << std::endl;

    return a;
}
