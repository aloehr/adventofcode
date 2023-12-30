#include<algorithm>
#include<array>
#include<cassert>
#include<iostream>
#include<limits>
#include<map>
#include<queue>
#include<set>
#include<string>

#include<aoch/AOCSolutionTypes.hpp>
#include<aoch/string.hpp>


using EdgeT = std::array<unsigned int, 2>;
using AdjacencyMatrixT = std::vector<std::vector<unsigned int>>;

// uses the stoer - wagner algorithm to find a minimal cut. (works for undirected graphs with non-negativ weights).
// for unweighted graphs, like the one in this problem, the minimal cut is the cut with the fewest edges cut, which is
// exactly what we want.
// https://en.wikipedia.org/wiki/Stoer%E2%80%93Wagner_algorithm
std::pair<unsigned int, std::vector<unsigned int>> find_minimum_cut(AdjacencyMatrixT mat) {

    size_t nodes_count = mat.size();
    std::vector<std::vector<unsigned int>> merged_nodes(nodes_count);

    std::pair<unsigned int, std::vector<unsigned int>> best_min_cut = {std::numeric_limits<unsigned int>::max(), {}};

    for (unsigned int i = 0; i < nodes_count; ++i) {
        merged_nodes[i].push_back(i);
    }

    for (size_t phase = 1; phase < nodes_count; ++phase) {
        std::priority_queue<std::pair<unsigned int, unsigned int>> q;
        std::vector<unsigned int> a = mat[0];
        std::vector<unsigned int> outside_a(nodes_count, 1);
        outside_a[0] = 0;

        for (size_t i = 0; i < nodes_count; ++i) {
            if (a[i]) q.push({a[i], i});
        }

        size_t s;
        size_t t;
        for (size_t i = 0; i < nodes_count - phase - 1; ++i) {

            while (!outside_a[q.top().second]) {
                q.pop();
            }

            t = q.top().second;
            q.pop();

            outside_a[t] = 0;
            a[t] = 0;

            for (size_t j = 1; j < nodes_count; ++j) {
                if (mat[t][j] && outside_a[j]) {
                    a[j] += mat[t][j];
                    q.push({a[j], j});
                }
            }
        }

        while (!outside_a[q.top().second]) {
            q.pop();
        }

        s = t;
        t = q.top().second;

        if (best_min_cut.first > a[t]) {
            best_min_cut = {a[t], merged_nodes[t]};
        }

        merged_nodes[s].insert(merged_nodes[s].end(), merged_nodes[t].begin(), merged_nodes[t].end());


        mat[s][t] = 0;
        mat[t][s] = 0;
        for (size_t i = 0; i < nodes_count; ++i) {
            mat[s][i] += mat[t][i];
            mat[i][s] = mat[s][i];
            mat[t][i] = 0;
            mat[i][t] = 0;
        }
    }

    return best_min_cut;
}

AdjacencyMatrixT create_adjacency_matrix(const std::vector<EdgeT>& edges) {
    // find max node idx
    unsigned int max_node_idx = std::max(edges[0][0], edges[0][1]);
    for (size_t i = 1; i < edges.size(); ++i) {
        max_node_idx = std::max(edges[i][0], max_node_idx);
        max_node_idx = std::max(edges[i][1], max_node_idx);
    }

    // create adjacency matrix
    AdjacencyMatrixT mat(max_node_idx + 1, std::vector<unsigned int>(max_node_idx + 1, 0));
    for (const auto& e : edges) {
        mat[e[0]][e[1]] = 1;
        mat[e[1]][e[0]] = 1;
    }

    return mat;
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

    std::vector<std::array<unsigned int, 2>> wires_vec(wires.cbegin(), wires.cend());

    auto mat = create_adjacency_matrix(wires_vec);
    auto result = find_minimum_cut(create_adjacency_matrix(wires_vec));

    assert(result.first == 3);

    a.part1 = std::to_string(result.second.size() * (comp_name_to_comp_id.size() - result.second.size()));
    a.part2 = "N/A";

    return a;
}
