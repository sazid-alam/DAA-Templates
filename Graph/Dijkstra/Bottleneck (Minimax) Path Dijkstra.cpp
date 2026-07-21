#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int to;
    long long weight;
};

/**
 * Finds a path from source to all nodes that minimizes the maximum edge weight encountered.
 */
vector<long long> minimax_dijkstra(int source, int n, const vector<vector<Edge>>& adj) {
    // dist[v] now represents the MINIMUM possible MAXIMUM edge weight on a path to v
    vector<long long> dist(n, INF);
    using PII = pair<long long, int>;
    priority_queue<PII, vector<PII>, greater<PII>> pq;
    
    // The maximum edge on a path of length 0 is 0
    dist[source] = 0;
    pq.push({0, source});
    
    while (!pq.empty()) {
        auto [max_edge_so_far, u] = pq.top();
        pq.pop();
        
        if (max_edge_so_far > dist[u]) continue;
        
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            long long weight = edge.weight;
            
            // The bottleneck on the path to 'v' through 'u' is the max of the path to 'u' and the edge u->v
            long long bottleneck = max(dist[u], weight);
            
            // If we found a path with a smaller bottleneck, update and push
            if (bottleneck < dist[v]) {
                dist[v] = bottleneck;
                pq.push({dist[v], v});
            }
        }
    }
    
    return dist;
}
