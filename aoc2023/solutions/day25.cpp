#include<algorithm>
#include<array>
#include<iostream>
#include<map>
#include<set>
#include<string>
#include<unordered_set>

#include<aoch/AOCSolutionTypes.hpp>
#include<aoch/string.hpp>


using EdgeT = std::array<unsigned int, 2>;
using AdjacencyMatrixT = std::vector<std::vector<unsigned int>>;

void print_edge(const EdgeT e, std::map<unsigned int, std::string>& id_to_name) {
    std::cout << id_to_name[e[0]] << " -- " << id_to_name[e[1]] << std::endl;
}

void create_dsf_tree_iterative(const AdjacencyMatrixT& mat, std::vector<EdgeT>& dsf_tree) {
    static std::vector<unsigned int> visited;
    std::vector<EdgeT> q;

    visited.assign(mat.size(), 0);

    dsf_tree.clear();
    dsf_tree.reserve(mat.size());

    q.push_back({0, 0});

    while (q.size()) {
        auto cur = q.back();
        q.pop_back();

        if (visited[cur[0]]) continue;
        visited[cur[0]] = 1;
        dsf_tree.push_back(cur);

        for (size_t i = 0; i < mat.size(); ++i) {
            if (!mat[cur[0]][i] || visited[i]) continue;

            q.push_back({static_cast<unsigned int>(i), cur[0]});
        }
    }
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

// .first contains the chains, .second contains Edges that weren't used in any chain if there were any
std::pair<std::vector<std::vector<EdgeT>>, std::vector<EdgeT>> chain_decomposition(const AdjacencyMatrixT& mat, std::vector<EdgeT>& dsf_tree) {
    std::unordered_set<size_t> used_edges;
    std::unordered_set<size_t> dsf_edges;
    std::vector<unsigned int> visited(dsf_tree.size(), 0);
    std::vector<unsigned int> node_id_to_dsf_tree_id(dsf_tree.size(), 0);

    for (size_t i = 0; i < dsf_tree.size(); ++i) {
        node_id_to_dsf_tree_id[dsf_tree[i][0]] = i;
    }

    for (size_t i = 1; i < dsf_tree.size(); ++i) {
        auto& e = dsf_tree[i];
        dsf_edges.insert((static_cast<size_t>(std::min(e[0], e[1])) << 32) | std::max(e[0], e[1]));
    }

    std::vector<std::vector<EdgeT>> chains;

    for (size_t i = 0; i < dsf_tree.size(); ++i) {
        for (size_t j = 0; j < mat.size(); ++j) {

            unsigned int cur = dsf_tree[i][0];

            if (!mat[cur][j]) continue;
            if (dsf_edges.count((static_cast<size_t>(std::min(cur, static_cast<unsigned int>(j))) << 32) | std::max(cur, static_cast<unsigned int>(j))))
                continue;

            if (node_id_to_dsf_tree_id[j] < i) continue;
            unsigned int next = j;

            // if we are here we have found a backedge
            visited[cur] = 1;

            std::vector<EdgeT> cur_chain;

            while (true) {
                used_edges.insert((static_cast<size_t>(std::min(cur, next)) << 32) | std::max(cur, next));
                used_edges.insert({next, cur});
                cur_chain.push_back({cur, next});

                if (visited[next]) break;

                visited[next] = 1;
                cur = next;
                next = dsf_tree[node_id_to_dsf_tree_id[next]][1];
            }

            chains.push_back(std::move(cur_chain));
        }
    }

    std::vector<EdgeT> unused_edges;
    for (size_t i = 0; i < mat.size(); ++i) {
        for (size_t j = i + 1; j < mat.size(); ++j) {
            if (mat[i][j] && !used_edges.count((static_cast<size_t>(std::min(i, j)) << 32) | std::max(i, j)))
                unused_edges.push_back({static_cast<unsigned int>(i), static_cast<unsigned int >(j)});
        }
    }

    return {std::move(chains), {std::move(unused_edges)}};
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
    std::map<unsigned int, std::string> comp_id_to_comp_name;

    auto get_comp_id = [&comp_name_to_comp_id, &comp_id_to_comp_name] (const std::string& comp_name) {
        static unsigned int next_free_comp_id = 0;

        if (!comp_name_to_comp_id.count(comp_name)) {
            comp_name_to_comp_id[comp_name] = next_free_comp_id;
            comp_id_to_comp_name[next_free_comp_id] = comp_name;
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

    std::vector<std::set<unsigned int>> components(comp_name_to_comp_id.size());

    for (const auto& w : wires) {
        components[w[0]].insert(w[1]);
        components[w[1]].insert(w[0]);
    }

    std::vector<std::array<unsigned int, 2>> wires_vec(wires.cbegin(), wires.cend());

    auto mat = create_adjacency_matrix(wires_vec);
    std::vector<EdgeT> dsf_tree;
    dsf_tree.reserve(mat.size());

    bool found = false;
    for (size_t i = 0; i < wires_vec.size() - 1 && !found; ++i) {
        auto& w1 = wires_vec[i];

        mat[w1[0]][w1[1]] = 0;
        mat[w1[1]][w1[0]] = 0;

        for (size_t j = i + 1; j < wires_vec.size() && !found; ++j) {
            auto& w2 = wires_vec[j];
            mat[w2[0]][w2[1]] = 0;
            mat[w2[1]][w2[0]] = 0;

            create_dsf_tree_iterative(mat, dsf_tree);
            auto res = chain_decomposition(mat, dsf_tree);

            if (res.second.size() == 1) {
                found = true;
                mat[res.second[0][0]][res.second[0][1]] = 0;
                mat[res.second[0][1]][res.second[0][0]] = 0;

                auto visit_count = [&mat] (unsigned int start) {
                    std::vector<unsigned int> visited(mat.size(), 0);
                    std::vector<unsigned int> q { start };
                    unsigned int visited_count = 0;

                    while (q.size()) {
                        unsigned int cur = q.back();
                        q.pop_back();

                        if (visited[cur]) continue;
                        visited[cur] = 1;
                        visited_count++;

                        for (size_t i = 0; i < mat.size(); ++i) {
                            if (mat[cur][i]) {
                                q.push_back(i);
                            }
                        }
                    }

                    return visited_count;
                };

                auto r1 = visit_count(res.second[0][0]);
                auto r2 = visit_count(res.second[0][1]);

                a.part1 = std::to_string(r1 * r2);
            }

            mat[w2[0]][w2[1]] = 1;
            mat[w2[1]][w2[0]] = 1;
        }

        mat[w1[0]][w1[1]] = 1;
        mat[w1[1]][w1[0]] = 1;
    }

    a.part2 = "N/A";

    return a;
}
