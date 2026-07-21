#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Helper function for DFS-based topological sort.
 * 
 * @return false if a cycle is detected, true otherwise.
 */
bool dfs_toposort(int u, const vector<vector<int>>& adj, vector<int>& state, vector<int>& topo_order) {
    state[u] = 1; // Mark as "Visiting"
    
    for (int v : adj[u]) {
        if (state[v] == 1) {
            return false; // Cycle detected!
        }
        if (state[v] == 0) {
            if (!dfs_toposort(v, adj, state, topo_order)) {
                return false;
            }
        }
    }
    
    state[u] = 2; // Mark as "Fully Visited"
    topo_order.push_back(u); // Push node ONLY after all its children are processed
    return true;
}

/**
 * Computes the topological sort using DFS.
 * 
 * @param n   Total number of nodes (0-indexed).
 * @param adj Adjacency list.
 * @return    A valid topological order, or an empty vector if a cycle exists.
 */
vector<int> get_dfs_toposort(int n, const vector<vector<int>>& adj) {
    vector<int> state(n, 0);
    vector<int> topo_order;
    
    for (int i = 0; i < n; ++i) {
        if (state[i] == 0) {
            if (!dfs_toposort(i, adj, state, topo_order)) {
                return {}; // Cycle found, toposort impossible
            }
        }
    }
    
    // The nodes were added in post-order, so we must reverse the list
    reverse(topo_order.begin(), topo_order.end());
    return topo_order;
}
