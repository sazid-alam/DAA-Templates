#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Edge {
    int to;
    long long weight;
};

/**
 * Finds the longest path in a DAG ending at each node.
 * Useful for Critical Path Method (CPM) / task scheduling.
 */
vector<long long> longest_path_in_dag(int n, const vector<vector<Edge>>& adj) {
    vector<int> in_degree(n, 0);
    for (int u = 0; u < n; ++u) {
        for (const auto& edge : adj[u]) {
            in_degree[edge.to]++;
        }
    }
    
    queue<int> q;
    vector<long long> dist(n, 0); // Initialize longest path distances to 0
    
    for (int i = 0; i < n; ++i) {
        if (in_degree[i] == 0) {
            q.push(i);
            dist[i] = 0; 
        }
    }
    
    // Process nodes in topological order
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            long long weight = edge.weight;
            
            // DP state transition: push the maximum distance forward
            if (dist[u] + weight > dist[v]) {
                dist[v] = dist[u] + weight;
            }
            
            in_degree[v]--;
            if (in_degree[v] == 0) {
                q.push(v);
            }
        }
    }
    
    return dist;
}
