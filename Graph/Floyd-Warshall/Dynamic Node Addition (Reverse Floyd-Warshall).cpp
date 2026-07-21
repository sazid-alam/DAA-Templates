#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

/**
 * Solves the dynamic node deletion/addition problem.
 * 
 * @param n             Number of nodes.
 * @param adj           Full initial adjacency matrix (no missing edges).
 * @param delete_order  The order in which nodes are deleted (0-indexed).
 * @return              The sum of shortest paths after each deletion step.
 */
vector<long long> reverse_floyd_warshall(int n, vector<vector<long long>> adj, vector<int> delete_order) {
    vector<long long> result(n);
    vector<bool> active(n, false);
    
    // We will build the graph backwards
    for (int step = n - 1; step >= 0; --step) {
        int k = delete_order[step];
        active[k] = true;
        
        long long current_sum = 0;
        
        // Treat 'k' as the newly permitted intermediate node
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                adj[i][j] = min(adj[i][j], adj[i][k] + adj[k][j]);
                
                // Only sum paths between nodes that are currently active in the graph
                if (active[i] && active[j]) {
                    current_sum += adj[i][j];
                }
            }
        }
        
        // Store the answer for this step
        result[step] = current_sum;
    }
    
    return result; // result[0] = before any deletions, result[n-1] = after n-1 deletions
}
