#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int u, v;
    long long weight;
};

/**
 * Runs Bellman-Ford and returns the distances and the parent array.
 */
pair<vector<long long>, vector<int>> bellman_ford_with_parents(int source, int n, const vector<Edge>& edges) {
    vector<long long> dist(n, INF);
    vector<int> parent(n, -1);
    dist[source] = 0;
    
    // Relax all edges V - 1 times
    for (int i = 1; i <= n - 1; ++i) {
        bool any_update = false;
        for (const auto& edge : edges) {
            if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
                dist[edge.v] = dist[edge.u] + edge.weight;
                parent[edge.v] = edge.u; // Track the predecessor
                any_update = true;
            }
        }
        if (!any_update) break; // Early exit if no improvements
    }
    
    return {dist, parent};
}

/**
 * Reconstructs the shortest path from source to target using the parent array.
 * 
 * @return A vector of nodes representing the path. Returns an empty vector if unreachable.
 */
vector<int> reconstruct_path(int source, int target, const vector<int>& parent, const vector<long long>& dist) {
    // If the target was never reached, no path exists
    if (dist[target] == INF) {
        return {};
    }
    
    vector<int> path;
    for (int curr = target; curr != -1; curr = parent[curr]) {
        path.push_back(curr);
    }
    
    // The path is built backwards from target to source, so reverse it
    reverse(path.begin(), path.end());
    
    return path;
}

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    int n = 5;
    vector<Edge> edges = {
        {0, 1, -1}, {0, 2, 4},
        {1, 2, 3}, {1, 3, 2}, {1, 4, 2},
        {3, 2, 5}, {3, 1, 1}, {4, 3, -3}
    };
    
    int source = 0, target = 2;
    
    // 1. Get distances and parents
    auto [dist, parent] = bellman_ford_with_parents(source, n, edges);
    
    // 2. (Optional) Check for negative cycles before trusting the path
    bool has_negative_cycle = false;
    for (const auto& edge : edges) {
        if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
            has_negative_cycle = true;
            break;
        }
    }
    
    if (has_negative_cycle) {
        cout << "Graph contains a negative weight cycle! Paths may be invalid.\n";
    } else {
        // 3. Reconstruct and print the path
        vector<int> path = reconstruct_path(source, target, parent, dist);
        
        if (path.empty()) {
            cout << "No path exists from " << source << " to " << target << "\n";
        } else {
            cout << "Shortest path (distance " << dist[target] << "): ";
            for (int i = 0; i < path.size(); ++i) {
                cout << path[i] << (i + 1 == path.size() ? "" : " -> ");
            }
            cout << "\n";
        }
    }
    
    return 0;
}
