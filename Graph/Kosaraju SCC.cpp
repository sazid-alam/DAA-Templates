#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Kosaraju's Algorithm for finding Strongly Connected Components (SCCs).
 * Time Complexity: O(V + E)
 */
struct KosarajuSCC {
    int n, scc_count;
    vector<vector<int>> adj, rev_adj;
    vector<int> order, scc_id;
    vector<bool> visited;

    // Constructor: n is the number of vertices (0-indexed)
    KosarajuSCC(int n) : n(n), scc_count(0), adj(n), rev_adj(n), 
                         scc_id(n, -1), visited(n, false) {}

    // Adds a directed edge from u to v
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        rev_adj[v].push_back(u);
    }

    // Step 1: Standard DFS to compute post-order traversal
    void dfs1(int u) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) dfs1(v);
        }
        order.push_back(u);
    }

    // Step 2: DFS on the reversed graph to extract components
    void dfs2(int u) {
        visited[u] = true;
        scc_id[u] = scc_count; // Assign current component ID
        for (int v : rev_adj[u]) {
            if (!visited[v]) dfs2(v);
        }
    }

    // Executes the algorithm
    void build_sccs() {
        // Phase 1: Build the post-order sequence
        fill(visited.begin(), visited.end(), false);
        for (int i = 0; i < n; i++) {
            if (!visited[i]) dfs1(i);
        }

        // Phase 2: Process in reverse post-order on the transposed graph
        fill(visited.begin(), visited.end(), false);
        for (int i = n - 1; i >= 0; i--) {
            int u = order[i];
            if (!visited[u]) {
                dfs2(u);
                scc_count++; // Increment ID for the next component
            }
        }
    }
};

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    int n = 5;
    KosarajuSCC kosa(n);
    
    // Create a graph with two SCCs:
    // SCC 1: 0 -> 1 -> 2 -> 0
    // SCC 2: 3 -> 4 -> 3
    // Connection between them: 2 -> 3
    kosa.add_edge(0, 1);
    kosa.add_edge(1, 2);
    kosa.add_edge(2, 0);
    kosa.add_edge(2, 3);
    kosa.add_edge(3, 4);
    kosa.add_edge(4, 3);
    
    // Run Kosaraju's Algorithm
    kosa.build_sccs();
    
    cout << "Total Strongly Connected Components: " << kosa.scc_count << "\n\n";
    
    // Group vertices by their SCC ID for easy printing
    vector<vector<int>> components(kosa.scc_count);
    for (int i = 0; i < n; ++i) {
        components[kosa.scc_id[i]].push_back(i);
    }
    
    for (int i = 0; i < kosa.scc_count; ++i) {
        cout << "SCC " << i << " contains vertices: ";
        for (int v : components[i]) {
            cout << v << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
