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
 * Standard Dijkstra to find shortest distances from a starting node.
 */
vector<long long> get_distances(int start, int n, const vector<vector<Edge>>& adj) {
    vector<long long> dist(n, INF);
    using PII = pair<long long, int>;
    priority_queue<PII, vector<PII>, greater<PII>> pq;
    
    dist[start] = 0;
    pq.push({0, start});
    
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

/**
 * Finds and prints the lexicographically smallest shortest path.
 * 
 * @param source The starting node.
 * @param target The destination node.
 * @param n      Total number of nodes.
 * @param adj    Adjacency list of the graph.
 * @return       A vector containing the sequence of nodes in the path.
 */
vector<int> get_lexicographically_smallest_path(int source, int target, int n, const vector<vector<Edge>>& adj) {
    // 1. Get distances from the source
    vector<long long> distS = get_distances(source, n, adj);
    
    // If the target is unreachable, return an empty path
    if (distS[target] == INF) {
        return {};
    }
    
    // 2. Create the reversed graph to get distances FROM the target
    vector<vector<Edge>> rev_adj(n);
    for (int u = 0; u < n; ++u) {
        for (const auto& edge : adj[u]) {
            rev_adj[edge.to].push_back({u, edge.weight});
        }
    }
    
    // 3. Get distances from the target in the reversed graph
    vector<long long> distT = get_distances(target, n, rev_adj);
    long long shortest_dist = distS[target];
    
    // 4. Greedily construct the lexicographically smallest path
    vector<int> path;
    int curr = source;
    path.push_back(curr);
    
    while (curr != target) {
        int next_node = -1;
        
        for (const auto& edge : adj[curr]) {
            int v = edge.to;
            long long weight = edge.weight;
            
            // Check if this edge is strictly on ANY shortest path to the target
            if (distS[curr] + weight + distT[v] == shortest_dist) {
                // Pick the neighbor with the smallest index
                if (next_node == -1 || v < next_node) {
                    next_node = v;
                }
            }
        }
        
        // Move to the chosen neighbor
        curr = next_node;
        path.push_back(curr);
    }
    
    return path;
}

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    int n = 6; 
    vector<vector<Edge>> adj(n);
    
    // Create a graph with two shortest paths from 0 to 5, both with total weight 4.
    // Path 1: 0 -> 3 -> 5 (Lexicographically larger)
    // Path 2: 0 -> 1 -> 5 (Lexicographically smaller)
    adj[0].push_back({3, 2});
    adj[0].push_back({1, 2});
    adj[3].push_back({5, 2});
    adj[1].push_back({5, 2});
    
    // Add some noise paths that are longer
    adj[0].push_back({2, 1});
    adj[2].push_back({5, 5});
    
    int source = 0, target = 5;
    vector<int> path = get_lexicographically_smallest_path(source, target, n, adj);
    
    if (path.empty()) {
        cout << "No path exists from " << source << " to " << target << "\n";
    } else {
        cout << "Lexicographically smallest shortest path: ";
        for (int i = 0; i < path.size(); ++i) {
            cout << path[i] << (i + 1 == path.size() ? "" : " -> ");
        }
        cout << "\n";
    }
    
    return 0;
}
