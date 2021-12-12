#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <set>

#include "../aoc.hpp"

// we have a small cave if the name is written in small letters
bool small_cave(int node_id, std::map<int, std::string>& node_id_names) {
    return node_id_names[node_id][0] >= 97;
}

// counts recursively all pathes from a starting point to the end node
// visited_twice == true -> p1
// visited_twice == false -> p2
int count_all_pathes(int cur_node,
        std::map<std::string, int>& node_ids,
        std::map<int, std::set<int>>& edges,
        std::map<int, std::string>& node_id_names,
        bool visited_twice = false,
        std::set<int> visited = std::set<int>()
    ) {

    // we are at the end, return 1 for having found one path to the end
    if (cur_node == node_ids["end"]) return 1;

    visited.insert(cur_node);

    // try every edge from current node to continue the path
    int path_count = 0;
    for (auto n : edges[cur_node]) {
        if (!visited.count(n) || !small_cave(n, node_id_names)) {
            path_count += count_all_pathes(n, node_ids, edges, node_id_names, visited_twice, visited);
        }

        // allows to visit one small cave twice (part 2)
        if (!visited_twice && visited.count(n) && small_cave(n, node_id_names)) {
            if (n != node_ids["start"] && n != node_ids["end"]) {
                path_count += count_all_pathes(n, node_ids, edges, node_id_names, true, visited);
            }
        }
    }

    return path_count;
}

answer solve_day12(input& in) {

    answer a;

    std::map<std::string, int> node_ids;
    std::map<int, std::set<int>> edges;
    std::map<int, std::string> node_id_names;

    int next_id = 0;

    for (auto& l : in) {
        size_t del_pos = l.find('-');

        assert(del_pos > 0 && del_pos < l.size());

        std::string n1 = l.substr(0, del_pos);
        std::string n2 = l.substr(del_pos+1);


        if (!node_ids.count(n1)) {
            node_id_names.insert(std::make_pair(next_id, n1));
            node_ids.insert(std::make_pair(n1, next_id++));
        }
        if (!node_ids.count(n2)) {
            node_id_names.insert(std::make_pair(next_id, n2));
            node_ids.insert(std::make_pair(n2, next_id++));
        }

        // bidirected graph, insert edge for both directions
        edges[node_ids[n1]].insert(node_ids[n2]);
        edges[node_ids[n2]].insert(node_ids[n1]);
    }

    int start = node_ids["start"];

    a.part1 = std::to_string(count_all_pathes(start, node_ids, edges, node_id_names, true));
    a.part2 = std::to_string(count_all_pathes(start, node_ids, edges, node_id_names));

    return a;
}
