/*
The "Popular Nodes" Problem (Unique Sink Component)
The Problem: Find all nodes that can be reached by every other node in the graph. (Classic example: POJ 2186 - Popular Cows).
The Trick: In a DAG, a node can only be reached by everyone if it is the only sink (out-degree 0). If there are multiple sinks, they can't reach each other.

Condense the graph into SCCs.

Find all SCCs with an out-degree of 0.

If there is exactly one such SCC, all nodes inside it are the answer. If there are zero or >1, the answer is empty.


*/


#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Base Kosaraju Struct
struct KosarajuSCC {
    int n, scc_count; vector<vector<int>> adj, rev_adj; vector<int> order, scc_id; vector<bool> visited;
    KosarajuSCC(int n) : n(n), scc_count(0), adj(n), rev_adj(n), scc_id(n, -1), visited(n, false) {}
    void add_edge(int u, int v) { adj[u].push_back(v); rev_adj[v].push_back(u); }
    void dfs1(int u) { visited[u] = true; for (int v : adj[u]) if (!visited[v]) dfs1(v); order.push_back(u); }
    void dfs2(int u) { visited[u] = true; scc_id[u] = scc_count; for (int v : rev_adj[u]) if (!visited[v]) dfs2(v); }
    void build_sccs() {
        fill(visited.begin(), visited.end(), false);
        for (int i = 0; i < n; i++) if (!visited[i]) dfs1(i);
        fill(visited.begin(), visited.end(), false);
        for (int i = n - 1; i >= 0; i--) { int u = order[i]; if (!visited[u]) { dfs2(u); scc_count++; } }
    }
};

/**
 * Finds all nodes that are reachable by every other node in the graph.
 */
vector<int> find_popular_nodes(const KosarajuSCC& kosa) {
    int num_sccs = kosa.scc_count;
    vector<int> out_deg(num_sccs, 0);
    
    // Calculate out-degrees for each SCC
    for (int u = 0; u < kosa.n; ++u) {
        for (int v : kosa.adj[u]) {
            if (kosa.scc_id[u] != kosa.scc_id[v]) {
                out_deg[kosa.scc_id[u]]++;
            }
        }
    }
    
    int sink_scc = -1;
    int zero_out_count = 0;
    
    // Count how many SCCs have an out-degree of 0
    for (int i = 0; i < num_sccs; ++i) {
        if (out_deg[i] == 0) {
            zero_out_count++;
            sink_scc = i;
        }
    }
    
    // If there is more than 1 sink (or no sinks, which shouldn't happen in a DAG),
    // no node can be reached by EVERYONE.
    if (zero_out_count != 1) return {};
    
    // Otherwise, collect all original nodes belonging to the unique sink SCC
    vector<int> popular_nodes;
    for (int i = 0; i < kosa.n; ++i) {
        if (kosa.scc_id[i] == sink_scc) {
            popular_nodes.push_back(i);
        }
    }
    
    return popular_nodes;
}

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    int n = 4;
    KosarajuSCC kosa(n);
    
    // Graph: 0 -> 1, 1 -> 2, 2 -> 1, 3 -> 1
    // Nodes 1 and 2 form an SCC. Nodes 0 and 3 point to it.
    // Therefore, 1 and 2 can be reached by everyone.
    kosa.add_edge(0, 1);
    kosa.add_edge(1, 2);
    kosa.add_edge(2, 1);
    kosa.add_edge(3, 1);
    
    kosa.build_sccs();
    vector<int> popular = find_popular_nodes(kosa);
    
    cout << "Nodes reachable by everyone: ";
    for (int u : popular) cout << u << " ";
    cout << "\n";
    // Expected output: 1 2
    
    return 0;
}
