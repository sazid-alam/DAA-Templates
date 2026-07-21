/*
Maximum Value Path in Graph with Cycles (Coin Collector)
The Problem: Every node has a value (coins). You can start anywhere and end anywhere. What is the maximum value you can collect? You can visit nodes multiple times (via cycles), but you only collect their value once. (Classic: CSES "Coin Collector").
The Trick:
Condense into SCCs. The value of a super-node is the sum of values of all nodes inside it.
Run a DP on the resulting DAG.
Bonus: Kosaraju's algorithm guarantees that scc_id is assigned in forward topological order. This means scc_id 0 only points to larger IDs, so we can just run a simple for loop from $0$ to scc_count - 1 without explicitly topological-sorting again!

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
 * Finds the maximum sum of node values along any valid path.
 */
long long max_value_path(const KosarajuSCC& kosa, const vector<long long>& values) {
    int num_sccs = kosa.scc_count;
    
    // 1. Calculate total value for each SCC
    vector<long long> scc_values(num_sccs, 0);
    for (int i = 0; i < kosa.n; ++i) {
        scc_values[kosa.scc_id[i]] += values[i];
    }
    
    // 2. Build the Condensed DAG
    vector<vector<int>> dag(num_sccs);
    for (int u = 0; u < kosa.n; ++u) {
        for (int v : kosa.adj[u]) {
            if (kosa.scc_id[u] != kosa.scc_id[v]) {
                dag[kosa.scc_id[u]].push_back(kosa.scc_id[v]);
            }
        }
    }
    
    // 3. DP to find longest path on DAG. 
    // Since Kosaraju guarantees scc_id is a valid topological order, we just iterate 0 to num_sccs-1
    vector<long long> dp = scc_values;
    long long max_ans = 0;
    
    for (int i = 0; i < num_sccs; ++i) {
        max_ans = max(max_ans, dp[i]);
        for (int v : dag[i]) {
            // Push the current best sum forward
            dp[v] = max(dp[v], dp[i] + scc_values[v]);
        }
    }
    
    return max_ans;
}

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    int n = 5;
    KosarajuSCC kosa(n);
    vector<long long> values = {4, 2, 5, 2, 1}; // Values for nodes 0 to 4
    
    // Cycle: 0 -> 1 -> 2 -> 0 (Total = 4+2+5 = 11)
    // Path out of cycle: 2 -> 3 -> 4 (Total = 11+2+1 = 14)
    kosa.add_edge(0, 1);
    kosa.add_edge(1, 2);
    kosa.add_edge(2, 0);
    kosa.add_edge(2, 3);
    kosa.add_edge(3, 4);
    
    kosa.build_sccs();
    cout << "Maximum coins collected: " << max_value_path(kosa, values) << "\n";
    // Expected output: 14
    
    return 0;
}
