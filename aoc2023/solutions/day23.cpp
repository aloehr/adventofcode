#include<algorithm>
#include<array>
#include<iostream>
#include<map>
#include<string>
#include<tuple>
#include<utility>
#include<vector>

#include<aoch/AOCSolutionTypes.hpp>


enum Dir { NORTH, EAST, SOUTH, WEST };

using PosT = std::array<unsigned int, 2>;
using DirT = std::array<int, 2>;

struct Node {
    unsigned int id;
    PosT pos;
    std::vector<std::pair<Node*, int>> connections;
};

using GraphT = std::map<PosT, Node*>;

std::vector<PosT> possible_moves(const aoch::Input& map, const PosT pos, const PosT *exclusion = nullptr) {
    std::vector<PosT> ret;

    static std::vector<DirT> rel_dirs {
        { 0, -1},
        { 1,  0},
        { 0,  1},
        {-1,  0},
    };

    static std::vector<Dir> dirs {
        Dir::NORTH,
        Dir::EAST,
        Dir::SOUTH,
        Dir::WEST,
    };

    static std::map<char, Dir> slope_to_dir {
        {'^', Dir::NORTH},
        {'>', Dir::EAST},
        {'v', Dir::SOUTH},
        {'<', Dir::WEST},
    };

    char cur_tile = map[pos[1]][pos[0]];

    for (const auto dir : dirs) {
        if (cur_tile != '.' && dir != slope_to_dir[cur_tile]) continue;

        PosT next_pos {pos[0] + rel_dirs[dir][0], pos[1] + rel_dirs[dir][1]};

        if (next_pos[0] >= map[0].size() || next_pos[1] >= map.size()) continue;
        if (map[next_pos[1]][next_pos[0]] == '#') continue;
        if (exclusion && next_pos == *exclusion) continue;

        ret.push_back(next_pos);
    }

    return ret;
}

std::pair<PosT, unsigned int> walk_path(const aoch::Input& map, PosT cur_pos, PosT last_pos) {

    unsigned int cur_steps = 1;

    while (true) {
        auto next_moves = possible_moves(map, cur_pos, &last_pos);

        if (next_moves.size() != 1) return std::make_pair(cur_pos, cur_steps);

        last_pos = cur_pos;
        cur_pos = next_moves[0];
        cur_steps++;
    }
}

std::map<PosT, Node*> create_graph(const aoch::Input& map, const std::vector<PosT> extra_nodes) {
    std::map<PosT, Node*> graph;

    // find all intersections and create nodes for them
    unsigned int next_free_node_id = 0;
    for (unsigned int y = 0; y < map.size(); ++y) {
        for (unsigned int x = 0; x < map[0].size(); ++x) {
            if (map[y][x] == '.') {
                auto moves = possible_moves(map, {x, y});

                if (moves.size() > 2) {
                    Node *n = new Node;
                    n->pos = {x, y};
                    n->id = next_free_node_id++;
                    graph[n->pos] = n;
                }
            }
        }
    }

    // add extra nodes if they aren't added already, in this case
    // this would be start and dest nodes because they aren't intersections
    // and therefore will not be added automatically.
    for (const auto& node_pos : extra_nodes) {
        if (graph.count(node_pos)) continue;

        Node *n = new Node;
        n->pos = node_pos;
        n->id = next_free_node_id++;
        graph[node_pos] = n;
    }

    // find the connections (edges) between nodes.
    for (const auto& kv : graph) {
        Node& cur = *kv.second;

        auto moves = possible_moves(map, cur.pos);

        for (const auto& m : moves) {
            auto res = walk_path(map, m, cur.pos);

            if (graph.count(res.first)) {
                Node *n = graph[res.first];
                cur.connections.push_back(std::make_pair(n, res.second));
            }
        }
    }

    return graph;
}

unsigned int dfs(const GraphT g, Node* start, Node* dest) {
    std::vector<std::tuple<Node*, unsigned int, unsigned int>> q;
    std::vector<Node*> visited_stack;
    unsigned int longest_path_steps = 0;
    unsigned int last_depth = 0;

    q.push_back(std::make_tuple(start, 0, 0));

    while (q.size()) {
        Node* cur;
        unsigned int cur_steps;
        unsigned int cur_depth;

        std::tie(cur, cur_steps, cur_depth) = q.back();
        q.pop_back();

        if (cur_depth && cur_depth <= last_depth) {
            // remove elements from visited_stack according to the depth diff between
            // last and current depth.
            // For a depth diff of N pop the last N+1 elements in visited_stack
            unsigned int remove_count = last_depth - cur_depth + 1;
            visited_stack.resize(visited_stack.size() - remove_count);
        }

        visited_stack.push_back(cur);

        if (cur == dest) {
            if (cur_steps > longest_path_steps) {
                longest_path_steps = cur_steps;
            }
            last_depth = cur_depth;
            continue;
        }

        for (const auto& p : cur->connections) {
            if (std::find(visited_stack.cbegin(), visited_stack.cend(), p.first) != visited_stack.cend()) continue;
            q.push_back(std::make_tuple(p.first, cur_steps + p.second, cur_depth+1));
        }

        last_depth = cur_depth;
    }

    return longest_path_steps;
}

aoch::Result solve_day23(aoch::Input& in) {
    aoch::Result a;

    PosT start = {0, 0};
    start[0] = std::find(in[0].cbegin(), in[0].cend(), '.') - in[0].cbegin();

    PosT dest = {0, static_cast<unsigned int>(in.size()-1)};
    dest[0] = std::find(in[in.size()-1].cbegin(), in[in.size()-1].cend(), '.') - in[in.size()-1].cbegin();

    auto g = create_graph(in, {start, dest});
    a.part1 = std::to_string(dfs(g, g[start], g[dest]));


    // part 2
    // convert all slopes into normal path tiles
    for (auto& l : in) {
        for (auto& c : l) {
            if (c != '.' && c != '#') c = '.';
        }
    }

    g = create_graph(in, {start, dest});
    a.part2 = std::to_string(dfs(g, g[start], g[dest]));

    return a;
}
