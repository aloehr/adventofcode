#include<algorithm>
#include<array>
#include<iostream>
#include<map>
#include<queue>
#include<set>
#include<string>
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
    static std::vector<Dir> dirs {
        Dir::NORTH,
        Dir::EAST,
        Dir::SOUTH,
        Dir::WEST,
    };

    static std::vector<DirT> rel_dirs {
        { 0, -1},
        { 1,  0},
        { 0,  1},
        {-1,  0},
    };

    static std::map<char, Dir> slope_to_dir {
        {'^', Dir::NORTH},
        {'>', Dir::EAST},
        {'v', Dir::SOUTH},
        {'<', Dir::WEST},
    };

    std::vector<PosT> ret;
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
                cur.connections.push_back({n, res.second});
            }
        }
    }

    return graph;
}

void delete_graph(GraphT& g) {
    for (auto it = g.begin(); it != g.end(); ++it) {
        delete it->second;
    }

    g.clear();
}

struct QueueItem {
    Node* n;
    unsigned int steps;
    unsigned int depth;

    QueueItem(Node* n, unsigned int steps, unsigned int depth) : n(n), steps(steps), depth(depth) {}
};

unsigned int dfs(const GraphT g, Node* start, Node* dest) {
    std::vector<QueueItem> q;

    std::vector<unsigned int > visited_stack;
    std::vector<unsigned int> visited(g.size(), 0);

    unsigned int longest_path_steps = 0;

    // init state
    q.emplace_back(start, 0, 0);

    while (q.size()) {
        QueueItem cur = q.back();
        q.pop_back();

        if (cur.n == dest) {
            longest_path_steps = std::max(longest_path_steps, cur.steps);
            continue;
        }

        // pop items from visited_stack and mark them as unvisited until stack_size
        // matches current depth
        while (visited_stack.size() != cur.depth) {
            visited[visited_stack.back()] = 0;
            visited_stack.pop_back();
        }

        visited[cur.n->id] = 1;
        visited_stack.push_back(cur.n->id);

        for (const auto& p : cur.n->connections) {
            if (visited[p.first->id]) continue;
            q.emplace_back(p.first, cur.steps + p.second, cur.depth + 1);
        }
    }

    return longest_path_steps;
}

void optimize_graph_node_merging(GraphT& g, const std::vector<Node*>& preserve) {
    bool merged_something;

    do {
        merged_something = false;
        for (auto it = g.begin(); it != g.end(); ++it) {
            Node *cur = it->second;
            if (cur->connections.size() != 1) continue;

            Node *o = cur->connections[0].first;
            int weight_between_cur_and_o = cur->connections[0].second;

            // we can't merge cur into o if it is in preserve but we can check if the other side of the
            // edge is in preserve as well and if that isn't the cause we can merge
            // the other node into cur instead
            if (std::find(preserve.cbegin(), preserve.cend(), cur) != preserve.cend()) {
                if (std::find(preserve.cbegin(), preserve.cend(), o) != preserve.cend()) {
                    continue;
                } else {
                    std::swap(cur, o);
                }
            }

            // if we get here we gonna merge cur into o

            auto find_cur = [cur](const auto& a) {
                return a.first == cur;
            };

            // remove connection from o to cur
            o->connections.erase(std::find_if(o->connections.cbegin(), o->connections.cend(), find_cur));

            // add connections from cur towards other nodes to o's connections
            // and redirect other nodes connections that point towards cur to o
            for (auto& p : cur->connections) {
                if (p.first == o) continue;

                // add connection to node o
                o->connections.push_back({p.first, p.second + weight_between_cur_and_o});
                auto res = std::find_if(p.first->connections.begin(), p.first->connections.end(), find_cur);

                // redirect other nodes connections from cur to o
                res->first = o;
                res->second += weight_between_cur_and_o;
            }

            // remove cur from graph
            g.erase(cur->pos);
            delete cur;

            merged_something = true;
            break;
        }
    } while (merged_something);

    // we have to redo the node id's because we possibly removed nodes
    // and we want to make sure node id's have no gaps.
    // This is important for the visited vector in the dfs search.
    // It assumes that node id < g.size()
    unsigned int next_free_node_id = 0;

    for (auto& kv : g) {
        kv.second->id = next_free_node_id++;
    }
}

// we remove edges that if they ever would be taken, we couldn't reach the dest anymore.
void optimize_graph_prune_edges(GraphT& g, Node* start, Node* dest) {
    std::queue<std::pair<Node*, Node*>> q;
    std::set<Node*> visited;

    q.push({start, nullptr});

    while (q.size()) {
        Node *cur, *from;
        std::tie(cur, from) = q.front();
        q.pop();

        visited.insert(cur);

        auto it = std::find_if(cur->connections.cbegin(), cur->connections.cend(), [from](const auto& a) { return a.first == from; });
        if (it != cur->connections.cend()) {
            cur->connections.erase(it);
        }

        for (auto& kv : cur->connections) {
            if (visited.count(kv.first)) continue;
            if (kv.first->connections.size() > 3) continue;
            q.push({kv.first, cur});
        }
    }
}

aoch::Result solve_day23(aoch::Input& in) {
    aoch::Result a;

    PosT start = {0, 0};
    start[0] = std::find(in[0].cbegin(), in[0].cend(), '.') - in[0].cbegin();

    PosT dest = {0, static_cast<unsigned int>(in.size() - 1)};
    dest[0] = std::find(in[in.size() - 1].cbegin(), in[in.size() - 1].cend(), '.') - in[in.size() - 1].cbegin();

    auto g = create_graph(in, {start, dest});

    a.part1 = std::to_string(dfs(g, g[start], g[dest]));
    delete_graph(g);


    // part 2
    // convert all slopes into normal path tiles
    for (auto& l : in) {
        for (auto& c : l) {
            if (c != '.' && c != '#') c = '.';
        }
    }

    g = create_graph(in, {start, dest});
    optimize_graph_node_merging(g, {g[start], g[dest]});
    optimize_graph_prune_edges(g, g[start], g[dest]);

    a.part2 = std::to_string(dfs(g, g[start], g[dest]));
    delete_graph(g);

    return a;
}
