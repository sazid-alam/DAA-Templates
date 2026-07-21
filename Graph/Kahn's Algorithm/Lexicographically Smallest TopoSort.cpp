#include <iostream>
#include <vector>
#include <queue>

using namespace std;

/**
 * Lexicographically smallest topological sort.
 */
vector<int> lexicographical_toposort(int n, const vector<vector<int>>& adj) {
    vector<int> in_degree(n, 0);
    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) {
            in_degree[v]++;
        }
    }
    
    // Min-heap to always pick the smallest available node index
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 0; i < n; ++i) {
        if (in_degree[i] == 0) {
            pq.push(i);
        }
    }
    
    vector<int> topo_order;
    
    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();
        topo_order.push_back(u);
        
        for (int v : adj[u]) {
            in_degree[v]--;
            if (in_degree[v] == 0) {
                pq.push(v);
            }
        }
    }
    
    if (topo_order.size() != n) return {}; // Cycle detected
    return topo_order;
}
