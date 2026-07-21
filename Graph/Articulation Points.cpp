#include <iostream>
#include <vector>

using namespace std;

/**
 * Finds all Articulation Points (Cut Vertices) in an undirected graph.
 * Time Complexity: O(V + E)
 */
struct ArticulationFinder {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> dfn, low;
    vector<bool> is_ap;

    ArticulationFinder(int n) : n(n), timer(0), adj(n), dfn(n, -1), low(n, -1), is_ap(n, false) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int parent = -1) {
        dfn[u] = low[u] = ++timer;
        int children = 0;
        
        for (int v : adj[u]) {
            if (v == parent) continue;
            
            if (dfn[v] != -1) {
                // Back-edge
                low[u] = min(low[u], dfn[v]);
            } else {
                // Tree-edge
                children++;
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                
                // Condition for non-root nodes
                if (parent != -1 && low[v] >= dfn[u]) {
                    is_ap[u] = true;
                }
            }
        }
        
        // Condition for the root node
        if (parent == -1 && children > 1) {
            is_ap[u] = true;
        }
    }

    vector<int> find_aps() {
        for (int i = 0; i < n; ++i) {
            if (dfn[i] == -1) {
                dfs(i);
            }
        }
        
        vector<int> aps;
        for (int i = 0; i < n; ++i) {
            if (is_ap[i]) aps.push_back(i);
        }
        return aps;
    }
};

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    int n = 5;
    ArticulationFinder af(n);
    
    // Graph: 0-1-2 triangle, and 2-3-4 triangle connected at 2
    af.add_edge(0, 1);
    af.add_edge(1, 2);
    af.add_edge(2, 0);
    af.add_edge(2, 3);
    af.add_edge(3, 4);
    af.add_edge(4, 2);
    
    vector<int> aps = af.find_aps();
    
    cout << "Articulation Points found: ";
    for (int u : aps) cout << u << " ";
    cout << "\n";
    // Expected: 2 (removing 2 splits the triangles)
    
    return 0;
}
