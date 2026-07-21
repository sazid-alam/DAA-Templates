#include <iostream>
#include <vector>

using namespace std;

const long long INF = 1e18;

/**
 * Standard Floyd-Warshall with path reconstruction.
 * 
 * @param n   Number of nodes (0-indexed).
 * @param adj Initial adjacency matrix (adj[i][j] = weight, adj[i][i] = 0, INF if no edge).
 * @return    A pair {distances, next_node_matrix}.
 */
pair<vector<vector<long long>>, vector<vector<int>>> floyd_warshall(int n, vector<vector<long long>>& adj) {
    vector<vector<long long>> dist = adj;
    vector<vector<int>> nxt(n, vector<int>(n, -1));
    
    // Initialize the 'nxt' matrix for path reconstruction
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j && adj[i][j] != INF) {
                nxt[i][j] = j;
            }
        }
    }
    
    // Core Floyd-Warshall DP
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] < INF && dist[k][j] < INF) { // Prevent INF + INF overflow
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        nxt[i][j] = nxt[i][k]; // To go from i to j, first go where i goes to reach k
                    }
                }
            }
        }
    }
    
    // Optional: Identify negative cycles. 
    // If distance from a node to itself is negative, it's part of a negative cycle.
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] < INF && dist[k][j] < INF && dist[k][k] < 0) {
                    dist[i][j] = -INF; // Path is corrupted by a negative cycle
                }
            }
        }
    }
    
    return {dist, nxt};
}

/**
 * Reconstructs the path from u to v.
 */
vector<int> reconstruct_path(int u, int v, const vector<vector<int>>& nxt, const vector<vector<long long>>& dist) {
    if (dist[u][v] == INF || dist[u][v] == -INF) return {}; // Unreachable or infinite negative cycle
    
    vector<int> path = {u};
    while (u != v) {
        u = nxt[u][v];
        path.push_back(u);
    }
    return path;
}
