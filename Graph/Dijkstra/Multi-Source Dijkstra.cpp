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
 * Runs Multi-Source Dijkstra.
 * 
 * @param sources A list of starting nodes.
 * @param n       Total number of nodes.
 * @param adj     Adjacency list.
 * @return        A vector where dist[i] is the shortest distance from ANY source to 'i'.
 */
vector<long long> multi_source_dijkstra(const vector<int>& sources, int n, const vector<vector<Edge>>& adj) {
    vector<long long> dist(n, INF);
    using PII = pair<long long, int>;
    priority_queue<PII, vector<PII>, greater<PII>> pq;
    
    // Initialize all sources simultaneously
    for (int src : sources) {
        dist[src] = 0;
        pq.push({0, src});
    }
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) continue;
        
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            long long weight = edge.weight;
            
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    
    return dist;
}
