#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <string>

#include <aoch/AOCSolutionTypes.hpp>
#include <aoch/string.hpp>

#include "BucketQueue.hpp"


using EdgeT = std::array<unsigned int, 2>;
using AdjacencyListT = std::vector<std::vector<std::pair<unsigned int, unsigned int>>>;

// uses the stoer - wagner algorithm to find a minimal cut. (works for undirected graphs with non-negativ weights).
// for unweighted graphs, like the one in this problem, the minimal cut is the cut with the fewest edges cut, which is
// exactly what we want.
// https://en.wikipedia.org/wiki/Stoer%E2%80%93Wagner_algorithm
std::pair<unsigned int, std::vector<unsigned int>> find_minimum_cut(AdjacencyListT nodes) {

    size_t nodes_count = nodes.size();
    std::vector<std::vector<unsigned int>> merged_nodes(nodes_count);

    std::pair<unsigned int, std::vector<unsigned int>> best_min_cut = {std::numeric_limits<unsigned int>::max(), {}};
    BucketQueue<unsigned int> q(nodes_count);
    std::vector<unsigned int> a(nodes_count, 0);

    for (unsigned int i = 0; i < nodes_count; ++i) {
        merged_nodes[i].push_back(i);
    }

    for (size_t phase = 1; phase < nodes_count; ++phase) {
        std::vector<unsigned int> outside_a(nodes_count, 1);
        q.clear();

        for (auto e : nodes[0]) {
            a[e.first] = e.second;
            q.insert(e.first, e.second);
        }

        size_t s;
        size_t t;
        outside_a[0] = 0;

        for (size_t i = 0; i < nodes_count - phase - 1; ++i) {
            t = q.top();
            q.pop_top();

            outside_a[t] = 0;
            a[t] = 0;

            // remove outdated entries
            while (!outside_a[q.top()]) {
                q.pop_top();
            }

            for (const auto& e : nodes[t]) {
                if (outside_a[e.first]) {
                    a[e.first] += e.second;
                    q.insert(e.first, a[e.first]);
                }
            }
        }

        s = t;
        t = q.top();

        if (best_min_cut.first > a[t]) {
            best_min_cut = {a[t], merged_nodes[t]};
        }

        // early out because we know that a weight of 3 is the best we can get
        if (a[t] == 3) {
            return best_min_cut;
        }

        // make sure a is ready to use for next phase which means that
        // all entries in a have to be 0.
        // (and at this point only a[t] has a value other than 0)
        a[t] = 0;

        merged_nodes[s].insert(merged_nodes[s].cend(), merged_nodes[t].cbegin(), merged_nodes[t].cend());

        auto edge_t_s = std::find_if(nodes[t].cbegin(), nodes[t].cend(), [s](const auto& a) {
            return a.first == s;
        });
        if (edge_t_s != nodes[t].cend()) {
            nodes[t].erase(edge_t_s);
        }

        auto edge_s_t = std::find_if(nodes[s].cbegin(), nodes[s].cend(), [t](const auto& a) {
            return a.first == t;
        });
        if (edge_s_t != nodes[s].cend()) {
            nodes[s].erase(edge_s_t);
        }

        for (const auto& e : nodes[t]) {
            auto edge_e_t = std::find_if(nodes[e.first].begin(), nodes[e.first].end(), [t](const auto& a) {
                return a.first == t;
            });

            auto edge_s_e = std::find_if(nodes[s].begin(), nodes[s].end(), [&e](const auto& a) {
                return a.first == e.first;
            });

            if (edge_s_e == nodes[s].end()) {
                nodes[s].push_back(e);
                edge_e_t->first = s;
            } else {
                edge_s_e->second += e.second;
                nodes[e.first].erase(edge_e_t);

                auto edge_e_s = std::find_if(nodes[e.first].begin(), nodes[e.first].end(), [s](const auto& a) {
                    return a.first == s;
                });
                edge_e_s->second = edge_s_e->second;
            }
        }
    }

    return best_min_cut;
}

template <typename T>
AdjacencyListT create_adjacency_list(const std::set<EdgeT, T>& edges) {
    // find max node idx
    unsigned int max_node_idx = 0;
    for (const auto& e : edges) {
        max_node_idx = std::max(e[0], max_node_idx);
        max_node_idx = std::max(e[1], max_node_idx);
    }

    AdjacencyListT ret(max_node_idx + 1);

    for (const auto& e : edges) {
        ret[e[0]].push_back({e[1], 1});
        ret[e[1]].push_back({e[0], 1});
    }

    return ret;
}

aoch::Result solve_day25(aoch::Input& in) {
    aoch::Result a;

    auto cmp = [](const std::array<unsigned int, 2>& a, const std::array<unsigned int, 2> b) {
        unsigned int a_min, a_max, b_min, b_max;
        a_min = std::min(a[0], a[1]);
        a_max = std::max(a[0], a[1]);
        b_min = std::min(b[0], b[1]);
        b_max = std::max(b[0], b[1]);

        if (a_min == b_min) {
            return a_max < b_max;
        } else {
            return a_min < b_min;
        }
    };

    std::set<std::array<unsigned int, 2>, decltype(cmp)> wires(cmp);
    std::map<std::string, unsigned int> comp_name_to_comp_id;

    auto get_comp_id = [&comp_name_to_comp_id] (const std::string& comp_name) {
        static unsigned int next_free_comp_id = 0;

        if (!comp_name_to_comp_id.count(comp_name)) {
            comp_name_to_comp_id[comp_name] = next_free_comp_id;
            return next_free_comp_id++;
        } else {
            return comp_name_to_comp_id[comp_name];
        }
    };

    for (const auto& l : in) {
        auto tokens = aoch::split(l);
        tokens[0].pop_back();

        unsigned int left_comp_id = get_comp_id(tokens[0]);

        auto it = tokens.cbegin() + 1;
        while (it != tokens.cend()) {
            unsigned int right_comp_id = get_comp_id(*it);
            wires.insert({left_comp_id, right_comp_id});
            it++;
        }
    }

    auto list = create_adjacency_list(wires);
    auto result = find_minimum_cut(list);

    assert(result.first == 3);

    a.part1 = std::to_string(result.second.size() * (comp_name_to_comp_id.size() - result.second.size()));
    a.part2 = "N/A";

    return a;
}
