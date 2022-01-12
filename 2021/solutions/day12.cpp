#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "../aoc.hpp"

#include "aoch/string.hpp"


// counts recursively all pathes from a starting point to the end node (DFS)
// visit_twice == false allows one small cave to be visited twice (p2)
int count_all_pathes(
    const int cur_node,
    const std::vector<std::vector<int>>& edges,
    const std::vector<bool>& is_small_cave,
    const int start,
    const int end,
    std::vector<int>& visited,
    const bool visit_twice = false
) {
    visited[cur_node]++;

    // try every edge from current node to continue the path
    int path_count = 0;
    for (auto n : edges[cur_node]) {

        // if we can reached the end node, we found a new path
        if (n == end) {
            path_count += 1;
        }
        else if (!is_small_cave[n] || !visited[n]) {
            path_count += count_all_pathes(n, edges, is_small_cave, start, end, visited, visit_twice);
        }
        // p2 - if we reach this cur_node is a small cave
        //      and we will have visited it once before and it can't be an end_node either
        else if (!visit_twice && n != start) {
            path_count += count_all_pathes(n, edges, is_small_cave, start, end, visited, true);
        }
    }

    visited[cur_node]--;
    return path_count;
}

answer solve_day12(input& in) {

    answer a;

    std::map<std::string, int> node_ids;
    std::vector<bool> node_small_cave;

    // for n edges there can be at most 2n nodes
    std::vector<std::vector<int>> edges(2 * in.size());

    int next_id = 0;
    int start_id = 0;
    int end_id = 0;

    for (auto& l : in) {
        auto nodes = aoch::split(l, '-');

        for (const auto& n : nodes) {
            if (!node_ids.count(n)) {
                if (n == "start") start_id = next_id;
                else if (n == "end") end_id = next_id;

                node_small_cave.push_back(n[0] > 96);
                node_ids.insert(std::make_pair(n, next_id++));
            }
        }

        // bidirected graph, insert edge for both directions
        edges[node_ids[nodes[0]]].push_back(node_ids[nodes[1]]);
        edges[node_ids[nodes[1]]].push_back(node_ids[nodes[0]]);
    }

    auto visits = std::vector<int>(node_ids.size());
    a.part1 = std::to_string(
        count_all_pathes(start_id, edges, node_small_cave, start_id, end_id, visits, true)
    );

    a.part2 = std::to_string(
        count_all_pathes(start_id, edges, node_small_cave, start_id, end_id, visits)
    );

    return a;
}
