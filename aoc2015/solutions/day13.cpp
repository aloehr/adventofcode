#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


void sepa2(std::vector<size_t>& in) {
    size_t key_field = in.size();

    for (size_t i = in.size() - 1; i > 0; --i) {
        if (in[i-1] < in[i]) {
            key_field = i-1;
            break;
        }
    }

    // we are at last permutation (reverse order)
    if (key_field == in.size()) return;

    size_t smallest_in_tail_bigger_than_key = 0;
    for (size_t i = in.size(); i > 0; --i) {
        // because the tail is sorted, the first from the right bigger than the key is the smallest one
        if (in[i-1] > in[key_field]) {
            smallest_in_tail_bigger_than_key = i-1;
            break;
        }
    }

    std::swap(in[key_field], in[smallest_in_tail_bigger_than_key]);

    // sort the tail (because tail still in reverse sorted order swapping is sufficient)
    for (size_t i = 0; i < (in.size() - key_field - 1)/2; ++i) {
        std::swap(in[key_field + 1 + i], in[in.size() - 1 - i]);
    }
}

int calc_happiness(const std::vector<size_t>& perm, std::map<size_t, std::map<size_t, int>>& pref) {
    int total = 0;

    for (size_t i = 0; i < perm.size(); ++i) {
        total += pref[perm[i]][perm[(i+1) % perm.size()]];
        total += pref[perm[(i+1) % perm.size()]][perm[i]];
    }

    return total;
}

int calc_best_happiness(std::map<size_t, std::map<size_t, int>>& pref) {

    std::vector<size_t> perm(pref.size(), 0);

    for (size_t i = 0; i < perm.size(); ++i) {
        perm[i] = i;
    }

    std::vector<size_t> last;
    int best = std::numeric_limits<int>::min();

    do {
        best = std::max(best, calc_happiness(perm, pref));
        last = perm;
        sepa2(perm);
    } while (last != perm);

    return best;
}

aoch::Result solve_day13(aoch::Input& in) {
    aoch::Result r;

    std::map<size_t, std::map<size_t, int>> preferences;
    std::map<std::string, size_t> person_to_id;

    size_t next_id = 0;

    for (const auto& l : in) {
        auto tokens = aoch::split(l);
        std::string person1 = tokens[0];
        std::string person2 = tokens[10].substr(0, tokens[10].size() - 1);

        if (person_to_id.count(person1) == 0) {
            person_to_id[person1] = next_id++;
        }

        if (person_to_id.count(person2) == 0) {
            person_to_id[person2] = next_id++;
        }

        const int sign = tokens[2] == "gain" ? 1 : -1;

        const size_t p1_id = person_to_id[person1];
        const size_t p2_id = person_to_id[person2];

        preferences[p1_id][p2_id] = sign * std::stoi(tokens[3]);
    }

    r.part1 = std::to_string(calc_best_happiness(preferences));

    // part 2
    const size_t my_id = next_id++;

    for (size_t i = 0; i < person_to_id.size() - 1; ++i) {
        preferences[my_id][i] = 0;
        preferences[i][my_id] = 0;
    }

    r.part2 = std::to_string(calc_best_happiness(preferences));

    return r;
}
