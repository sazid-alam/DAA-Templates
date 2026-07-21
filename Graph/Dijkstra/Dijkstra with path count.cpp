#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const long long INF = 1e18;
const int MOD = 1e9 + 7; // Used to prevent overflow for path counts

struct Edge {
    int to;
    long long weight;
};

/**
 * Runs Dijkstra's algorithm to find distances and the number of shortest paths.
 * 
 * @param source The starting node (0-indexed or 1-indexed based on graph setup).
 * @param n      The total number of nodes.
 * @param adj    The adjacency list representing the graph.
 * @return       A pair of vectors: {distances, path_counts}.
 */
pair<vector<long long>, vector<int>> dijkstra_count(int source, int n, const vector<vector<Edge>>& adj) {
    vector<long long> dist(n, INF);
    vector<int> count(n, 0);
    
    using PII = pair<long long, int>;
    priority_queue<PII, vector<PII>, greater<PII>> pq;
    
    // Initialize the source node
    dist[source] = 0;
    count[source] = 1; // There is exactly 1 way to be at the start: doing nothing
    pq.push({0, source});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        // Lazy deletion: ignore stale queue entries
        if (d > dist[u]) continue;
        
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            long long weight = edge.weight;
            
            // Case 1: We found a strictly shorter path to 'v'
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                count[v] = count[u]; // Inherit the number of paths from 'u'
                pq.push({dist[v], v});
            }
            // Case 2: We found another path of the exact same minimum distance
            else if (dist[u] + weight == dist[v]) {
                // Add the paths going through 'u' to 'v'
                count[v] = (count[v] + count[u]) % MOD;
            }
        }
    }
    
    return {dist, count};
}

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    int n = 5; 
    vector<vector<Edge>> adj(n);
    
    // Creating a graph with multiple shortest paths from 0 to 4
    adj[0].push_back({1, 2});
    adj[0].push_back({2, 2});
    adj[1].push_back({3, 2});
    adj[2].push_back({3, 2});
    adj[3].push_back({4, 1});
    adj[0].push_back({4, 5}); // Another path directly to 4 of weight 5
    
    int source = 0;
    auto [distances, counts] = dijkstra_count(source, n, adj);
    
    for (int i = 0; i < n; ++i) {
        if (distances[i] == INF) {
            cout << "Node " << i << " is unreachable\n";
        } else {
            cout << "Node " << i 
                 << " | Shortest Distance: " << distances[i] 
                 << " | Number of Shortest Paths: " << counts[i] << "\n";
        }
    }
    
    return 0;
}
