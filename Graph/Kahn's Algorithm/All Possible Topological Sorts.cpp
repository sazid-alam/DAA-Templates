#include <iostream>
#include <vector>

using namespace std;

void backtrack_all_toposorts(int n, vector<vector<int>>& adj, vector<int>& in_degree, 
                             vector<int>& path, vector<bool>& visited, 
                             vector<vector<int>>& all_sorts) {
    // Base case: If all nodes are included, we found a valid sort
    if (path.size() == n) {
        all_sorts.push_back(path);
        return;
    }
    
    // Try picking any node with in_degree == 0 that hasn't been visited yet
    for (int i = 0; i < n; ++i) {
        if (in_degree[i] == 0 && !visited[i]) {
            
            // 1. CHOOSE
            visited[i] = true;
            path.push_back(i);
            for (int v : adj[i]) {
                in_degree[v]--; // "Remove" outgoing edges
            }
            
            // 2. EXPLORE
            backtrack_all_toposorts(n, adj, in_degree, path, visited, all_sorts);
            
            // 3. UN-CHOOSE (Backtrack)
            for (int v : adj[i]) {
                in_degree[v]++;
            }
            path.pop_back();
            visited[i] = false;
        }
    }
}

vector<vector<int>> get_all_toposorts(int n, vector<vector<int>>& adj) {
    vector<int> in_degree(n, 0);
    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) {
            in_degree[v]++;
        }
    }
    
    vector<vector<int>> all_sorts;
    vector<int> path;
    vector<bool> visited(n, false);
    
    backtrack_all_toposorts(n, adj, in_degree, path, visited, all_sorts);
    return all_sorts;
}
