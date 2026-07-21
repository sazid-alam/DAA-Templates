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
 * Finds the strictly second shortest path from source to all nodes.
 */
pair<vector<long long>, vector<long long>> second_shortest_path(int source, int n, const vector<vector<Edge>>& adj) {
    vector<long long> dist1(n, INF); // Shortest distance
    vector<long long> dist2(n, INF); // Strictly second shortest distance
    
    using PII = pair<long long, int>;
    priority_queue<PII, vector<PII>, greater<PII>> pq;
    
    dist1[source] = 0;
    pq.push({0, source});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        // Lazy deletion: if 'd' is worse than our current second best, discard it
        if (d > dist2[u]) continue;
        
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            long long weight = edge.weight;
            long long new_d = d + weight;
            
            // Case 1: We found a strictly shorter path
            if (new_d < dist1[v]) {
                // The old shortest becomes the new second shortest
                dist2[v] = dist1[v];
                dist1[v] = new_d;
                pq.push({dist1[v], v});
                pq.push({dist2[v], v});
            } 
            // Case 2: We found a path longer than the shortest, but better than current second shortest
            else if (new_d > dist1[v] && new_d < dist2[v]) {
                dist2[v] = new_d;
                pq.push({dist2[v], v});
            }
        }
    }
    
    return {dist1, dist2};
}
