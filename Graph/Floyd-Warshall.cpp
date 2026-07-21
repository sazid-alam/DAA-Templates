#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

/**
 * Vanilla Floyd-Warshall Algorithm.
 * Computes the shortest paths between all pairs of vertices in O(V^3).
 * 
 * @param n    The total number of vertices (0-indexed).
 * @param dist The initial adjacency matrix. dist[i][j] should be the edge weight.
 *             If there is no edge, it should be INF.
 * @return     The all-pairs shortest path distance matrix.
 */
vector<vector<long long>> vanilla_floyd_warshall(int n, vector<vector<long long>> dist) {
    // Base case: Distance from a node to itself is always 0
    for (int i = 0; i < n; ++i) {
        dist[i][i] = 0;
    }

    // Core DP: k is the intermediate node, i is source, j is destination
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                // Prevent INF + INF overflow
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
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
    
    // Initialize matrix with INF
    vector<vector<long long>> adj(n, vector<long long>(n, INF));
    
    // Add directed edges
    adj[0][1] = 5;
    adj[0][3] = 10;
    adj[1][2] = 3;
    adj[2][3] = 1;
    
    // Run the algorithm
    vector<vector<long long>> shortest_paths = vanilla_floyd_warshall(n, adj);
    
    // Print the result matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (shortest_paths[i][j] == INF) {
                cout << "INF\t";
            } else {
                cout << shortest_paths[i][j] << "\t";
            }
        }
        cout << "\n";
    }
    
    return 0;
}
