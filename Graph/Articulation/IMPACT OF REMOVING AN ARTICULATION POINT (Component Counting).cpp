#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * ============================================================================
 * IMPACT OF REMOVING AN ARTICULATION POINT (Component Counting)
 * ============================================================================
 * 
 * THE PROBLEM:
 * Given a graph (possibly disconnected), if you remove vertex V and all its 
 * attached edges, how many connected components will the graph break into? 
 * You need to answer this efficiently for EVERY vertex in the graph.
 * (Classic: UVA 10199 - Tourist Guide / SPOJ - SUBMERGE variation)
 * 
 * THE TRICK:
 * We don't just set a boolean `is_ap`. We count *how many* subtrees are 
 * strictly disconnected from the ancestors. 
 * In Tarjan's DFS, every time `low[v] >= dfn[u]` for a child `v`, it means 
 * `v`'s subtree has no back-edge going above `u`. Thus, removing `u` cuts 
 * off `v`'s subtree into its own isolated component.
 * 
 * Math for a node U:
 * - If U is the DFS root: It breaks into `C` components, where `C` is the 
 *   number of its DFS children.
 * - If U is NOT the root: It breaks into `1 + K` components, where `K` is 
 *   the number of children where `low[v] >= dfn[u]` (the +1 is for the 
 *   "ancestor" part of the graph above U).
 * - Total components in graph = (Initial Graph Components - 1) + Impact(U).
 * ============================================================================
 */
struct APImpactSolver {
    int n, timer, initial_components;
    vector<vector<int>> adj;
    vector<int> dfn, low, impact;

    APImpactSolver(int n) : n(n), timer(0), initial_components(0), 
                            adj(n), dfn(n, -1), low(n, -1), impact(n, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int parent = -1) {
        dfn[u] = low[u] = ++timer;
        int children = 0;
        int isolated_subtrees = 0; // Number of subtrees cut off if u is removed

        for (int v : adj[u]) {
            if (v == parent) continue;
            
            if (dfn[v] != -1) { // Back-edge
                low[u] = min(low[u], dfn[v]);
            } else { // Tree-edge
                children++;
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                
                // If the child cannot reach above u, it gets isolated
                if (low[v] >= dfn[u]) {
                    isolated_subtrees++;
                }
            }
        }

        // Calculate impact based on whether it is a root or not
        if (parent == -1) {
            impact[u] = children;
        } else {
            impact[u] = isolated_subtrees + 1;
        }
    }

    vector<int> get_components_after_removal() {
        for (int i = 0; i < n; ++i) {
            if (dfn[i] == -1) {
                initial_components++;
                dfs(i);
            }
        }

        vector<int> total_components_if_removed(n);
        for (int i = 0; i < n; ++i) {
            // Formula: Initial components, minus 1 (the component U was in), 
            // plus the pieces U breaks its component into.
            total_components_if_removed[i] = initial_components - 1 + impact[i];
        }
        return total_components_if_removed;
    }
};

int main() {
    int n = 5;
    APImpactSolver solver(n);
    
    // Graph: 0-1-2 triangle attached to a line 2-3-4
    // Node 2 is a major AP. Node 3 is a minor AP.
    solver.add_edge(0, 1);
    solver.add_edge(1, 2);
    solver.add_edge(2, 0);
    solver.add_edge(2, 3);
    solver.add_edge(3, 4);

    vector<int> res = solver.get_components_after_removal();
    
    for (int i = 0; i < n; ++i) {
        cout << "Removing node " << i << " results in " << res[i] << " components.\n";
    }
    // Expected output:
    // Node 2 -> 3 components ({0,1}, {3}, {4} - wait, 4 is connected to 3, so {0,1} and {3,4}) => 2 components
    // Node 3 -> 2 components ({0,1,2}, {4})
    // Nodes 0, 1, 4 -> 1 component
    
    return 0;
}
