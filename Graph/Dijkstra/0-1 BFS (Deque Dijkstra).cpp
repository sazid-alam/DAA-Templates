#include <iostream>
#include <vector>
#include <deque>

using namespace std;

const int INF = 1e9; // int is usually sufficient for 0-1 BFS

struct Edge {
    int to;
    int weight; // Guaranteed to be 0 or 1
};

/**
 * 0-1 BFS to find shortest paths in O(V + E) time.
 */
vector<int> zero_one_bfs(int source, int n, const vector<vector<Edge>>& adj) {
    vector<int> dist(n, INF);
    deque<int> dq;
    
    dist[source] = 0;
    dq.push_front(source);
    
    while (!dq.empty()) {
        int u = dq.front();
        dq.pop_front();
        
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            int weight = edge.weight; // Must be 0 or 1
            
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                
                // 0-weight edges go to the front (processed sooner)
                if (weight == 0) {
                    dq.push_front(v);
                } 
                // 1-weight edges go to the back (processed later)
                else {
                    dq.push_back(v);
                }
            }
        }
    }
    
    return dist;
}
