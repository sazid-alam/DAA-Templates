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
 * Finds and reconstructs a negative-weight cycle.
 * Note: If you want to find ANY negative cycle regardless of reachability from a source,
 * initialize all dist[] to 0 instead of INF.
 */
vector<int> find_negative_cycle(int n, const vector<Edge>& edges) {
    vector<long long> dist(n, 0); // Initialize to 0 to catch disconnected cycles
    vector<int> parent(n, -1);
    int last_updated_node = -1;
    
    // Relax edges V times
    for (int i = 0; i < n; ++i) {
        last_updated_node = -1;
        for (const auto& edge : edges) {
            if (dist[edge.u] + edge.weight < dist[edge.v]) {
                dist[edge.v] = dist[edge.u] + edge.weight;
                parent[edge.v] = edge.u;
                last_updated_node = edge.v; // Keep track of where an update happened
            }
        }
    }
    
    // If no node was updated on the V-th iteration, there is no negative cycle
    if (last_updated_node == -1) {
        return {};
    }
    
    // We have a cycle. The last updated node might not be IN the cycle itself 
    // (it could be a node reachable from the cycle). 
    // To guarantee we are inside the cycle, trace backwards n times.
    int cycle_start = last_updated_node;
    for (int i = 0; i < n; ++i) {
        cycle_start = parent[cycle_start];
    }
    
    // Now reconstruct the cycle
    vector<int> cycle;
    for (int curr = cycle_start; ; curr = parent[curr]) {
        cycle.push_back(curr);
        if (curr == cycle_start && cycle.size() > 1) {
            break;
        }
    }
    
    // The path is traced backwards, so reverse it
    reverse(cycle.begin(), cycle.end());
    
    return cycle;
}
