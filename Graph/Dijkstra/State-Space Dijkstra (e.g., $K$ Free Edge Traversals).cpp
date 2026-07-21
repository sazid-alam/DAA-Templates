#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int to;
    long long weight;
};

// Custom struct for the priority queue to keep things clean
struct State {
    long long dist;
    int u;
    int k_used;
    
    // Operator overload for the min-heap (reverse logic because priority_queue puts largest at top)
    bool operator>(const State& other) const {
        return dist > other.dist;
    }
};

/**
 * Runs State-Space Dijkstra.
 * 
 * @param source The starting node.
 * @param n      Total number of nodes.
 * @param max_k  Maximum number of "special actions" (e.g., free edges) allowed.
 * @param adj    Adjacency list.
 * @return       A 2D vector where dist[u][k] is the shortest path to node 'u' using exactly 'k' actions.
 */
vector<vector<long long>> dijkstra_state_space(int source, int n, int max_k, const vector<vector<Edge>>& adj) {
    // dist[u][k] stores the minimum distance to node 'u' having used 'k' special actions
    vector<vector<long long>> dist(n, vector<long long>(max_k + 1, INF));
    
    // Min-heap storing {distance, node, actions_used}
    priority_queue<State, vector<State>, greater<State>> pq;
    
    dist[source][0] = 0;
    pq.push({0, source, 0});
    
    while (!pq.empty()) {
        auto [d, u, k] = pq.top();
        pq.pop();
        
        // Lazy deletion
        if (d > dist[u][k]) continue;
        
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            long long weight = edge.weight;
            
            // Option 1: Standard traversal (do not use a special action)
            if (dist[u][k] + weight < dist[v][k]) {
                dist[v][k] = dist[u][k] + weight;
                pq.push({dist[v][k], v, k});
            }
            
            // Option 2: Use a special action (if we haven't reached the limit)
            // Example action: This edge costs 0.
            if (k < max_k) {
                if (dist[u][k] + 0 < dist[v][k + 1]) {
                    dist[v][k + 1] = dist[u][k] + 0;
                    pq.push({dist[v][k + 1], v, k + 1});
                }
            }
        }
    }
    
    return dist;
}

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    int n = 4;
    int max_k = 1; // We can traverse at most 1 edge for free
    vector<vector<Edge>> adj(n);
    
    adj[0].push_back({1, 100});
    adj[1].push_back({2, 100});
    adj[2].push_back({3, 100});
    adj[0].push_back({2, 10}); // Cheap bypass
    
    int source = 0, target = 3;
    auto dist = dijkstra_state_space(source, n, max_k, adj);
    
    // The answer to the target could be achieved using 0, 1, ..., max_k actions.
    // We want the absolute minimum across all valid 'k' states for the target.
    long long ans = INF;
    for (int k = 0; k <= max_k; ++k) {
        ans = min(ans, dist[target][k]);
    }
    
    cout << "Shortest distance using at most " << max_k << " free edges: " << ans << "\n";
    // In this case: 0 -> 2 (costs 10 normally), 2 -> 3 (costs 100, make it free). Total: 10.
    
    return 0;
}
