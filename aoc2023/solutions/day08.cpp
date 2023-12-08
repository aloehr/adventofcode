#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


uint get_or_assign_node_id(const std::string& node_str, std::map<std::string, uint>& node_to_node_id) {
    static uint next_free_node_id = 0;

    if (node_to_node_id.count(node_str)) {
        return node_to_node_id[node_str];
    }

    node_to_node_id[node_str] = next_free_node_id;
    return next_free_node_id++;
}

aoch::Result solve_day08(aoch::Input& in) {
    aoch::Result a;

    std::map<std::string, uint> node_to_node_id;
    std::vector<std::array<uint, 2>> nodes(in.size() - 2);

    for (uint i = 2; i < in.size(); ++i) {
        auto tokens = aoch::split(in[i]);

        uint cur_node_id = get_or_assign_node_id(tokens[0], node_to_node_id);

        nodes[cur_node_id][0] = get_or_assign_node_id(tokens[2].substr(1, 3), node_to_node_id);
        nodes[cur_node_id][1] = get_or_assign_node_id(tokens[3].substr(0, 3), node_to_node_id);
    }

    std::vector<uint> dirs(in[0].size());
    for (uint i = 0; i < in[0].size(); ++i) {
        dirs[i] = in[0][i] == 'L' ? 0 : 1;
    }

    uint cur_node_id = node_to_node_id["AAA"];
    uint end_node_id = node_to_node_id["ZZZ"];
    uint steps = 0;
    uint idx = 0;

    while (cur_node_id != end_node_id) {
        cur_node_id = nodes[cur_node_id][dirs[idx]];
        idx = (idx + 1) % dirs.size();
        steps++;
    }

    a.part1 = std::to_string(steps);


    // part 2
    std::vector<uint> cur_nodes_ids;
    std::set<uint> end_nodes_ids;

    for (const auto& kv : node_to_node_id) {
        if (kv.first.back() == 'A') {
            cur_nodes_ids.push_back(kv.second);
        } else if (kv.first.back() == 'Z') {
            end_nodes_ids.insert(kv.second);
        }
    }

    std::vector<uint> cycles(cur_nodes_ids.size());

    for (uint i = 0; i < cur_nodes_ids.size(); ++i) {
        uint cur_node_id = cur_nodes_ids[i];

        steps = 0;
        idx = 0;

        while (!cycles[i]) {
            cur_node_id = nodes[cur_node_id][dirs[idx]];
            idx = (idx + 1) % dirs.size();
            steps++;

            if (end_nodes_ids.count(cur_node_id)) {
                if (cycles[i] == 0) {
                    cycles[i] = steps;
                }
            }
        }
    }

    unsigned long long cur_cycle = cycles[0];
    for (uint i = 1; i < cycles.size(); ++i) {
        cur_cycle = std::lcm(cur_cycle, cycles[i]);
    }

    a.part2 = std::to_string(cur_cycle);

    return a;
}
