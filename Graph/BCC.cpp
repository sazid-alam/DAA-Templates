#include <iostream>
#include <vector>
#include <stack>
#include <set>

using namespace std;

struct Edge {
    int u, v;
};

/**
 * Finds all Biconnected Components in an undirected graph.
 * Time Complexity: O(V + E)
 */
struct BCCFinder {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> dfn, low;
    stack<Edge> st;
    vector<vector<Edge>> bccs; // Stores lists of edges that form each BCC

    BCCFinder(int n) : n(n), timer(0), adj(n), dfn(n, -1), low(n, -1) {}

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
                // Back-edge: push to stack if it points higher up the tree
                if (dfn[v] < dfn[u]) {
                    st.push({u, v});
                    low[u] = min(low[u], dfn[v]);
                }
            } else {
                // Tree-edge
                children++;
                st.push({u, v});
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                
                // If u is an Articulation Point for v (or root), extract the BCC
                if ((parent == -1 && children > 1) || (parent != -1 && low[v] >= dfn[u])) {
                    vector<Edge> component;
                    while (true) {
                        Edge e = st.top();
                        st.pop();
                        component.push_back(e);
                        // Stop popping when we hit the edge that triggered this
                        if ((e.u == u && e.v == v) || (e.u == v && e.v == u)) {
                            break;
                        }
                    }
                    bccs.push_back(component);
                }
            }
        }
    }

    void find_bccs() {
        for (int i = 0; i < n; ++i) {
            if (dfn[i] == -1) {
                dfs(i);
                
                // At the very end of a DFS root, if there are leftover edges in the stack,
                // they form the final component for this disconnected piece of the graph.
                if (!st.empty()) {
                    vector<Edge> component;
                    while (!st.empty()) {
                        component.push_back(st.top());
                        st.pop();
                    }
                    bccs.push_back(component);
                }
            }
        }
    }
};

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    int n = 5;
    BCCFinder bccf(n);
    
    // Graph: 0-1-2 triangle, and 2-3-4 triangle connected at 2
    bccf.add_edge(0, 1);
    bccf.add_edge(1, 2);
    bccf.add_edge(2, 0);
    bccf.add_edge(2, 3);
    bccf.add_edge(3, 4);
    bccf.add_edge(4, 2);
    
    bccf.find_bccs();
    
    cout << "Total Biconnected Components: " << bccf.bccs.size() << "\n\n";
    
    for (int i = 0; i < bccf.bccs.size(); ++i) {
        cout << "BCC " << i << " contains edges: ";
        // We use a set just to collect unique vertices involved in this BCC for printing
        set<int> nodes;
        for (auto e : bccf.bccs[i]) {
            cout << "(" << e.u << "-" << e.v << ") ";
            nodes.insert(e.u);
            nodes.insert(e.v);
        }
        cout << "| Involving nodes: ";
        for (int u : nodes) cout << u << " ";
        cout << "\n";
    }
    
    return 0;
}
