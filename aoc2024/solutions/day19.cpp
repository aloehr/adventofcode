#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <aoch/AOCSolutionTypes.hpp>


struct TrieNode {
    std::array<TrieNode*, 26> children{};
    bool is_end = false;
};

struct PatternTrie {
    TrieNode root;

    void add_pattern(const std::string_view p) {
        TrieNode* cur = &root;

        for (auto c : p) {
            const size_t idx = c - 'a';

            if (cur->children[idx] == nullptr) {
                cur->children[idx] = new TrieNode;
            }

            cur = cur->children[idx];
        }

        cur->is_end = true;
    }

    std::vector<size_t> prefix_matches(const std::string_view s) const {
        std::vector<size_t> ret;

        size_t depth = 0;
        const TrieNode* cur = &root;

        for (auto c : s) {
            const size_t idx = c - 'a';

            if (cur->children[idx] == nullptr) break;

            depth++;

            cur = cur->children[idx];

            if (cur->is_end) {
                ret.push_back(depth);
            }
        }

        return ret;
    }
};

long long design_possible(
    const std::string_view design,
    const PatternTrie& pt,
    std::unordered_map<std::string_view, long long>& memo
) {
    if (design.empty()) return 1;

    if (memo.count(design)) {
        return memo[design];
    }

    long long total = 0;

    for (size_t length : pt.prefix_matches(design)) {
        total += design_possible(design.substr(length), pt, memo);
    }

    memo[design] = total;
    return total;
}

PatternTrie create_pattern_trie(std::string_view s) {
    PatternTrie pt;

    size_t start = 0;
    size_t end;

    while ((end = s.find_first_of(',', start)) != std::string::npos) {
        pt.add_pattern(s.substr(start, end - start));

        auto x = pt.prefix_matches(s.substr(start, end - start));
        if (x.size() == 0) std::cout << s.substr(start, end - start) << std::endl;
        // skipping comma and whie space
        start = end + 2;
    }

    // add the last pattern
    pt.add_pattern(s.substr(start));

    return pt;
}

aoch::Result solve_day19(aoch::Input& in) {
    aoch::Result a;

    PatternTrie pt = create_pattern_trie(in[0]);

    std::vector<std::string_view> designs;
    designs.reserve(in.size() - 2);

    for (size_t i = 2; i < in.size(); ++i) {
        designs.push_back(in[i]);
    }

    int count_possible = 0;
    long long total_ways = 0;

    std::unordered_map<std::string_view, long long> memo(100);

    for (const auto d : designs) {
        memo.clear();
        long long possible_ways = design_possible(d, pt, memo);

        count_possible += possible_ways != 0;
        total_ways += possible_ways;
    }

    a.part1 = std::to_string(count_possible);
    a.part2 = std::to_string(total_ways);

    return a;
}
