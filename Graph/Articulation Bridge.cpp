#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Finds all Bridges (Cut Edges) in an undirected graph.
 * Time Complexity: O(V + E)
 */
struct BridgeFinder {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> dfn, low;
    vector<pair<int, int>> bridges;

    BridgeFinder(int n) : n(n), timer(0), adj(n), dfn(n, -1), low(n, -1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // Undirected
    }

    void dfs(int u, int parent = -1) {
        dfn[u] = low[u] = ++timer;
        
        for (int v : adj[u]) {
            if (v == parent) continue; // Ignore the edge we just came from
            
            if (dfn[v] != -1) {
                // Back-edge found: update low[u]
                low[u] = min(low[u], dfn[v]);
            } else {
                // Tree-edge: recurse
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                
                // Bridge Condition
                if (low[v] > dfn[u]) {
                    // Ensure the smaller node is first for easier sorting/printing later
                    bridges.push_back({min(u, v), max(u, v)});
                }
            }
        }
    }

    void find_bridges() {
        for (int i = 0; i < n; ++i) {
            if (dfn[i] == -1) {
                dfs(i);
            }
        }
    }
};

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    int n = 5;
    BridgeFinder bf(n);
    
    // Cycle 0-1-2, plus a tail 2-3-4
    // 2-3 and 3-4 should be bridges
    bf.add_edge(0, 1);
    bf.add_edge(1, 2);
    bf.add_edge(2, 0);
    bf.add_edge(2, 3);
    bf.add_edge(3, 4);
    
    bf.find_bridges();
    
    cout << "Bridges found:\n";
    for (auto edge : bf.bridges) {
        cout << edge.first << " - " << edge.second << "\n";
    }
    // Expected: 3 - 4, then 2 - 3
    
    return 0;
}
