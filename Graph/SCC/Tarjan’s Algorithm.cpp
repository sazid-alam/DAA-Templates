#include <iostream>
#include <vector>
#include <stack>

using namespace std;

struct TarjanSCC {
    int n, timer, scc_count;
    vector<vector<int>> adj;
    vector<int> dfn, low, scc_id;
    vector<bool> in_stack;
    stack<int> st;

    TarjanSCC(int n) : n(n), timer(0), scc_count(0), 
                       adj(n), dfn(n, -1), low(n, -1), 
                       scc_id(n, -1), in_stack(n, false) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    void dfs(int u) {
        dfn[u] = low[u] = ++timer;
        st.push(u);
        in_stack[u] = true;

        for (int v : adj[u]) {
            if (dfn[v] == -1) { // Unvisited
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in_stack[v]) { // Back-edge
                low[u] = min(low[u], dfn[v]);
            }
        }

        // If u is a root node, pop the stack and form an SCC
        if (low[u] == dfn[u]) {
            while (true) {
                int v = st.top();
                st.pop();
                in_stack[v] = false;
                scc_id[v] = scc_count; // Assign all nodes to this SCC ID
                if (u == v) break;
            }
            scc_count++;
        }
    }

    void build_sccs() {
        for (int i = 0; i < n; i++) {
            if (dfn[i] == -1) {
                dfs(i);
            }
        }
    }
};
