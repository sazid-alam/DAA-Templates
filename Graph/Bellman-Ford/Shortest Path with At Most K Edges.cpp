#include <iostream>
#include <vector>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int u, v;
    long long weight;
};

/**
 * Finds shortest paths using strictly AT MOST K edges.
 */
vector<long long> bellman_ford_k_edges(int source, int n, int k, const vector<Edge>& edges) {
    vector<long long> dist(n, INF);
    dist[source] = 0;
    
    // Run exactly K iterations
    for (int i = 0; i < k; ++i) {
        // Create a copy to prevent "chaining" edge updates in the same iteration
        vector<long long> next_dist = dist;
        
        for (const auto& edge : edges) {
            if (dist[edge.u] != INF && dist[edge.u] + edge.weight < next_dist[edge.v]) {
                next_dist[edge.v] = dist[edge.u] + edge.weight;
            }
        }
        
        // Commit the state for the next phase
        dist = next_dist;
    }
    
    return dist;
}
