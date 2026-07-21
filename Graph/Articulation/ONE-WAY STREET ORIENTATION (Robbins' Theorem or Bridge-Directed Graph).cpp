#include <iostream>
#include <vector>

using namespace std;

/**
 * ============================================================================
 * ONE-WAY STREET ORIENTATION (Robbins' Theorem / Bridge-Directed Graph)
 * ============================================================================
 * 
 * THE PROBLEM:
 * A city consists of two-way streets (an undirected graph). The mayor wants 
 * to convert the streets into one-way streets to improve traffic. 
 * You must orient the edges such that the graph remains as strongly connected 
 * as possible (i.e., if A could reach B originally, A should still reach B).
 * (Classic: Codeforces 118E - Bertown roads / CSES - One Way Streets)
 * 
 * THE TRICK:
 * A perfectly strongly connected orientation is ONLY possible if the graph 
 * has NO bridges (Robbins' Theorem). 
 * If it has bridges, the best we can do is make all 2-edge-connected 
 * components strongly connected, and point the bridges in any direction.
 * 
 * We do this naturally using Tarjan's DFS tree:
 * 1. Tree Edges: Orient them DOWN the DFS tree (from ancestor to child).
 * 2. Back Edges: Orient them UP the DFS tree (from descendant to ancestor).
 * This elegantly creates a cycle for every back-edge, ensuring maximum 
 * strong connectivity!
 * ============================================================================
 */
struct Edge { int u, v; };

struct OneWayOrientationSolver {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> dfn, low;
    
    vector<Edge> directed_edges;
    bool has_bridges;

    OneWayOrientationSolver(int n) : n(n), timer(0), has_bridges(false), 
                                     adj(n), dfn(n, -1), low(n, -1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int parent = -1) {
        dfn[u] = low[u] = ++timer;
        
        for (int v : adj[u]) {
            if (v == parent) continue;
            
            if (dfn[v] != -1) { // Back-edge
                low[u] = min(low[u], dfn[v]);
                
                // Crucial logic: Only orient the back-edge if it goes UP the tree.
                // Since an undirected edge is visited from both sides, we only 
                // push it when we are at the bottom looking up.
                if (dfn[v] < dfn[u]) {
                    directed_edges.push_back({u, v});
                }
            } else { // Tree-edge
                // Orient DOWN the tree
                directed_edges.push_back({u, v});
                
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                
                // If this tree edge is a bridge, a perfect strongly connected 
                // orientation is impossible for the whole graph.
                if (low[v] > dfn[u]) {
                    has_bridges = true;
                }
            }
        }
    }

    void solve() {
        for (int i = 0; i < n; ++i) {
            if (dfn[i] == -1) {
                dfs(i);
            }
        }
    }
};

int main() {
    int n = 4;
    OneWayOrientationSolver solver(n);
    
    // A square: 0-1-2-3-0 (No bridges, can be fully strongly connected)
    solver.add_edge(0, 1);
    solver.add_edge(1, 2);
    solver.add_edge(2, 3);
    solver.add_edge(3, 0);

    solver.solve();

    if (solver.has_bridges) {
        cout << "Warning: Graph has bridges. Cannot be made fully strongly connected.\n";
        cout << "However, the following orientation is the optimal configuration:\n";
    } else {
        cout << "Graph is 2-edge-connected. Valid strongly connected orientation:\n";
    }

    for (const auto& edge : solver.directed_edges) {
        cout << edge.u << " -> " << edge.v << "\n";
    }
    
    return 0;
}
