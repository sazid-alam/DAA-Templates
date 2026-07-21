#include <iostream>
#include <vector>

using namespace std;

const long long INF = 1e18;

/**
 * Computes all-pairs bottleneck paths.
 * 
 * @param n   Number of nodes.
 * @param adj Adjacency matrix where adj[i][j] is the edge weight. INF if no edge.
 * @return    A matrix where dist[i][j] is the minimum possible maximum-edge on a path from i to j.
 */
vector<vector<long long>> floyd_warshall_minimax(int n, const vector<vector<long long>>& adj) {
    vector<vector<long long>> dist = adj;
    
    // Distance (bottleneck) to self is 0
    for (int i = 0; i < n; ++i) {
        dist[i][i] = 0;
    }
    
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    // The bottleneck of the path through 'k' is the heaviest edge on that path
                    long long bottleneck = max(dist[i][k], dist[k][j]);
                    
                    // If this route has a smaller bottleneck than the current best route, update
                    if (bottleneck < dist[i][j]) {
                        dist[i][j] = bottleneck;
                    }
                }
            }
        }
    }
    
    return dist;
}
