#include <iostream>
#include <vector>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int u, v;
    long long weight;
};

/**
 * Runs Standard Bellman-Ford.
 * 
 * @param source The starting node.
 * @param n      Total number of nodes.
 * @param edges  List of all edges in the graph.
 * @return       A pair: {distances vector, boolean indicating if a negative cycle exists}
 */
pair<vector<long long>, bool> bellman_ford(int source, int n, const vector<Edge>& edges) {
    vector<long long> dist(n, INF);
    dist[source] = 0;
    
    // Step 1: Relax all edges (V - 1) times
    for (int i = 1; i <= n - 1; ++i) {
        bool any_update = false;
        for (const auto& edge : edges) {
            // Only relax if the starting node has been reached
            if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
                dist[edge.v] = dist[edge.u] + edge.weight;
                any_update = true;
            }
        }
        // Early exit optimization: if no distances changed, we are done
        if (!any_update) break; 
    }
    
    // Step 2: Check for negative-weight cycles
    // If we can still relax an edge, a negative cycle exists
    bool has_negative_cycle = false;
    for (const auto& edge : edges) {
        if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
            has_negative_cycle = true;
            break;
        }
    }
    
    return {dist, has_negative_cycle};
}
