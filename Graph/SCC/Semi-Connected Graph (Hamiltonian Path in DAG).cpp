/*
The Problem: A directed graph is "semi-connected" if, for every pair of vertices $u$ and $v$, 
either there is a path from $u$ to $v$, OR a path from $v$ to $u$.


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
 * Checks if the graph is semi-connected.
 */
bool is_semi_connected(const KosarajuSCC& kosa) {
    int num_sccs = kosa.scc_count;
    
    // 1. Build the Condensed DAG
    vector<vector<int>> dag(num_sccs);
    for (int u = 0; u < kosa.n; ++u) {
        for (int v : kosa.adj[u]) {
            if (kosa.scc_id[u] != kosa.scc_id[v]) {
                dag[kosa.scc_id[u]].push_back(kosa.scc_id[v]);
            }
        }
    }
    
    // 2. Sort the adjacency lists to easily use binary_search
    for (int i = 0; i < num_sccs; ++i) {
        sort(dag[i].begin(), dag[i].end());
    }
    
    // 3. Because Kosaraju's IDs are a topological sort, 
    // a Hamiltonian path MUST visit them in exact order 0 -> 1 -> 2 -> ...
    for (int i = 0; i < num_sccs - 1; ++i) {
        // If there is no edge from i to i+1, the graph breaks into parallel branches
        if (!binary_search(dag[i].begin(), dag[i].end(), i + 1)) {
            return false;
        }
    }
    
    return true;
}

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    int n = 4;
    KosarajuSCC kosa(n);
    
    // Graph: 0 -> 1, 1 -> 2, 0 -> 3
    // DAG is: 0 points to 1 and 3. 1 points to 2.
    // Node 3 cannot reach 1 or 2, and 1/2 cannot reach 3. Not semi-connected!
    kosa.add_edge(0, 1);
    kosa.add_edge(1, 2);
    kosa.add_edge(0, 3);
    
    kosa.build_sccs();
    
    if (is_semi_connected(kosa)) {
        cout << "The graph is semi-connected.\n";
    } else {
        cout << "The graph is NOT semi-connected.\n";
    }
    
    return 0;
}
