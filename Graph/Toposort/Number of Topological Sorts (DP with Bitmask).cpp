#include <iostream>
#include <vector>

using namespace std;

/**
 * Computes the number of valid topological sorts using Bitmask DP.
 * Time Complexity: O(N * 2^N). Valid for N <= 20.
 */
long long count_toposorts(int n, const vector<vector<int>>& adj) {
    // in_mask[i] stores a bitmask of all nodes that have an edge pointing TO 'i' (prerequisites)
    vector<int> in_mask(n, 0);
    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) {
            in_mask[v] |= (1 << u);
        }
    }
    
    int total_states = 1 << n;
    vector<long long> dp(total_states, 0);
    
    // Base state: 1 way to have an empty graph
    dp[0] = 1; 
    
    // Iterate over all subsets of nodes
    for (int mask = 0; mask < total_states; ++mask) {
        if (dp[mask] == 0) continue; // Unreachable state
        
        // Try to append node 'i' to the current subset
        for (int i = 0; i < n; ++i) {
            // If 'i' is not yet in the mask, AND all prerequisites of 'i' are already in the mask
            if (!(mask & (1 << i)) && (mask & in_mask[i]) == in_mask[i]) {
                int next_mask = mask | (1 << i);
                dp[next_mask] += dp[mask];
            }
        }
    }
    
    return dp[total_states - 1]; // Number of ways to form the complete graph
}
