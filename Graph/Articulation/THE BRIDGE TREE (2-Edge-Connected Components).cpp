#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

/**
 * ============================================================================
 * THE BRIDGE TREE (2-Edge-Connected Components)
 * ============================================================================
 * 
 * THE PROBLEM:
 * You have a graph with cycles. You need to process queries like: 
 * "If I add one edge, what is the minimum number of bridges left?" or 
 * "What is the longest path of bridges I must cross to get from A to B?"
 * 
 * THE TRICK:
 * 1. Find all bridges.
 * 2. If you remove all bridges, the graph falls apart into connected components. 
 *    These are called 2-Edge-Connected Components (you can remove any 1 edge 
 *    inside them, and they stay connected).
 * 3. Compress each component into a single "super-node".
 * 4. Reconnect the super-nodes using the bridges. The resulting graph is always 
 *    a tree (a forest if the original graph was disconnected).
 * ============================================================================
 */
struct BridgeTreeBuilder {
    int n, timer, comp_count;
    vector<vector<int>> adj;
    vector<int> dfn, low, comp_id;
    set<pair<int, int>> bridges; // Fast lookup for bridges
    
    // The resulting Tree
    vector<vector<int>> tree_adj;

    BridgeTreeBuilder(int n) : n(n), timer(0), comp_count(0), 
                               adj(n), dfn(n, -1), low(n, -1), comp_id(n, -1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Step 1: Tarjan's to find bridges
    void dfs_bridges(int u, int parent = -1) {
        dfn[u] = low[u] = ++timer;
        for (int v : adj[u]) {
            if (v == parent) continue;
            if (dfn[v] != -1) {
                low[u] = min(low[u], dfn[v]);
            } else {
                dfs_bridges(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) {
                    bridges.insert({min(u, v), max(u, v)});
                }
            }
        }
    }

    // Step 2: Flood-fill to group 2-Edge-Connected Components
    void dfs_components(int u, int id) {
        comp_id[u] = id;
        for (int v : adj[u]) {
            if (comp_id[v] != -1) continue;
            
            // Do not cross bridges during the flood-fill
            if (bridges.count({min(u, v), max(u, v)})) continue;
            
            dfs_components(v, id);
        }
    }

    void build_tree() {
        // 1. Find all bridges
        for (int i = 0; i < n; ++i) {
            if (dfn[i] == -1) dfs_bridges(i);
        }

        // 2. Assign component IDs
        for (int i = 0; i < n; ++i) {
            if (comp_id[i] == -1) {
                dfs_components(i, comp_count++);
            }
        }

        // 3. Build the Bridge Tree
        tree_adj.assign(comp_count, vector<int>());
        for (auto bridge : bridges) {
            int u_comp = comp_id[bridge.first];
            int v_comp = comp_id[bridge.second];
            tree_adj[u_comp].push_back(v_comp);
            tree_adj[v_comp].push_back(u_comp);
        }
    }
};

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    int n = 6;
    BridgeTreeBuilder btb(n);
    
    // Cycle 1: 0-1-2
    btb.add_edge(0, 1); btb.add_edge(1, 2); btb.add_edge(2, 0);
    
    // Cycle 2: 3-4-5
    btb.add_edge(3, 4); btb.add_edge(4, 5); btb.add_edge(5, 3);
    
    // Bridge connecting the two cycles: 2-3
    btb.add_edge(2, 3);
    
    btb.build_tree();
    
    cout << "Total 2-Edge-Connected Components (Tree Nodes): " << btb.comp_count << "\n";
    
    for (int i = 0; i < n; ++i) {
        cout << "Original Node " << i << " belongs to Tree Node " << btb.comp_id[i] << "\n";
    }
    
    cout << "\nBridge Tree Edges:\n";
    for (int u = 0; u < btb.comp_count; ++u) {
        for (int v : btb.tree_adj[u]) {
            if (u < v) { // Print each undirected edge once
                cout << "Tree Node " << u << " - Tree Node " << v << "\n";
            }
        }
    }
    
    return 0;
}
