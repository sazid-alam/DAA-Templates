#include <iostream>
#include <vector>
#include <queue>

using namespace std;

/**
 * Standard Topological Sort using Kahn's Algorithm.
 * 
 * @param n   Total number of nodes (0-indexed).
 * @param adj Adjacency list.
 * @return    A valid topological order, or an empty vector if a cycle exists.
 */
vector<int> vanilla_toposort(int n, const vector<vector<int>>& adj) {
    vector<int> in_degree(n, 0);
    
    // 1. Calculate in-degrees for all nodes
    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) {
            in_degree[v]++;
        }
    }
    
    // 2. Initialize the queue with all nodes having in-degree 0
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }
    
    vector<int> topo_order;
    
    // 3. Process the nodes
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo_order.push_back(u);
        
        // Decrease in-degree of neighbors
        for (int v : adj[u]) {
            in_degree[v]--;
            // If in-degree becomes 0, it's ready to be processed
            if (in_degree[v] == 0) {
                q.push(v);
            }
        }
    }
    
    // 4. Cycle check
    if (topo_order.size() != n) {
        return {}; // Graph has a cycle, topological sort is impossible
    }
    
    return topo_order;
}
