#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const long long INF = 1e18; // Large enough to prevent overflow, acts as infinity

// Structure to represent a directed edge
struct Edge {
    int to;
    long long weight;
};

/**
 * Runs Dijkstra's algorithm from a given source node.
 * 
 * @param source The starting node (0-indexed or 1-indexed based on graph setup).
 * @param n      The total number of nodes.
 * @param adj    The adjacency list representing the graph.
 * @return       A vector containing the shortest distance to each node.
 */
vector<long long> dijkstra(int source, int n, const vector<vector<Edge>>& adj) {
    // Initialize distances to infinity
    vector<long long> dist(n, INF);
    
    // Min-heap to store {distance, node}. 
    // greater<> ensures the smallest distance is always at the top.
    using PII = pair<long long, int>;
    priority_queue<PII, vector<PII>, greater<PII>> pq;
    
    // Start with the source node
    dist[source] = 0;
    pq.push({0, source});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        // Lazy deletion: If we already found a shorter path to 'u', discard this stale pair
        if (d > dist[u]) continue;
        
        // Explore all neighbors of 'u'
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            long long weight = edge.weight;
            
            // Relaxation step: If a shorter path to 'v' is found
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    
    return dist;
}

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    int n = 5; // Number of nodes (0 to 4)
    vector<vector<Edge>> adj(n);
    
    // Adding directed edges: adj[from].push_back({to, weight})
    adj[0].push_back({1, 4});
    adj[0].push_back({2, 1});
    adj[2].push_back({1, 2});
    adj[1].push_back({3, 1});
    adj[2].push_back({3, 5});
    adj[3].push_back({4, 3});
    
    int source = 0;
    vector<long long> shortest_paths = dijkstra(source, n, adj);
    
    for (int i = 0; i < n; ++i) {
        if (shortest_paths[i] == INF) {
            cout << "Node " << i << " is unreachable\n";
        } else {
            cout << "Distance from " << source << " to " << i << " is " << shortest_paths[i] << "\n";
        }
    }
    
    return 0;
}
