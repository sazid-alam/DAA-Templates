#include <iostream>
#include <vector>
#include <string>

using namespace std;

/**
 * ============================================================================
 * DIRECTED GRAPH EDGE CLASSIFIER (DFS)
 * ============================================================================
 * 
 * THE TRICK:
 * We maintain the `state` of each node:
 *   0 (White) -> Unvisited
 *   1 (Gray)  -> Visiting (currently in the recursion stack)
 *   2 (Black) -> Fully Visited (recursion finished)
 * 
 * We also maintain `entry_time` for each node to distinguish between 
 * Forward and Cross edges.
 * 
 * Classification Rules for edge u -> v:
 * - If state[v] == 0: Tree Edge
 * - If state[v] == 1: Back Edge (Cycle detected!)
 * - If state[v] == 2:
 *     - If entry_time[u] < entry_time[v]: Forward Edge (shortcut down the tree)
 *     - If entry_time[u] > entry_time[v]: Cross Edge (points to another branch)
 * ============================================================================
 */
struct EdgeClassifier {
    int n;
    int timer;
    vector<vector<int>> adj;
    
    vector<int> state;       // 0 = unvisited, 1 = visiting, 2 = visited
    vector<int> entry_time;  // Tracks when we first reach a node
    
    // Storage for the categorized edges
    vector<pair<int, int>> tree_edges;
    vector<pair<int, int>> back_edges;
    vector<pair<int, int>> forward_edges;
    vector<pair<int, int>> cross_edges;

    EdgeClassifier(int n) : n(n), timer(0), adj(n), state(n, 0), entry_time(n, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    void dfs(int u) {
        state[u] = 1; // Mark as visiting (Gray)
        entry_time[u] = ++timer;
        
        for (int v : adj[u]) {
            if (state[v] == 0) {
                // Node is unvisited. This edge extends the DFS tree.
                tree_edges.push_back({u, v});
                dfs(v);
            } 
            else if (state[v] == 1) {
                // Node is currently active in the recursion stack.
                // This implies a cycle.
                back_edges.push_back({u, v});
            } 
            else if (state[v] == 2) {
                // Node is fully visited. Compare entry times.
                if (entry_time[u] < entry_time[v]) {
                    // u was discovered before v, so v is a descendant.
                    forward_edges.push_back({u, v});
                } else {
                    // u was discovered after v, so v is on a different branch/tree.
                    cross_edges.push_back({u, v});
                }
            }
        }
        
        state[u] = 2; // Mark as fully visited (Black)
    }

    void classify_all_edges() {
        // Run DFS from all unvisited nodes to handle disconnected graphs/forests
        for (int i = 0; i < n; ++i) {
            if (state[i] == 0) {
                dfs(i);
            }
        }
    }

    void print_results() {
        cout << "--- Tree Edges (Forms the DFS forest) ---\n";
        for (auto e : tree_edges) cout << e.first << " -> " << e.second << "\n";
        
        cout << "\n--- Back Edges (Indicates Cycles) ---\n";
        for (auto e : back_edges) cout << e.first << " -> " << e.second << "\n";
        
        cout << "\n--- Forward Edges (Shortcuts to descendants) ---\n";
        for (auto e : forward_edges) cout << e.first << " -> " << e.second << "\n";
        
        cout << "\n--- Cross Edges (Jumps between branches) ---\n";
        for (auto e : cross_edges) cout << e.first << " -> " << e.second << "\n";
    }
};

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    int n = 6; 
    EdgeClassifier classifier(n);
    
    // Setting up a graph designed to exhibit all 4 edge types
    // DFS will likely start at 0.
    classifier.add_edge(0, 1);
    classifier.add_edge(1, 2);
    classifier.add_edge(2, 3);
    
    // Forward edge (0 -> 2 skips node 1)
    classifier.add_edge(0, 2);
    
    // Back edge (3 -> 1 creates a cycle: 1 -> 2 -> 3 -> 1)
    classifier.add_edge(3, 1);
    
    // Cross edge (4 -> 2) (Assuming node 4 is processed after the 0-1-2-3 block)
    classifier.add_edge(4, 5);
    classifier.add_edge(4, 2); 
    
    classifier.classify_all_edges();
    classifier.print_results();
    
    return 0;
}
