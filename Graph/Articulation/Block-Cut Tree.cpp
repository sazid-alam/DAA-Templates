#include <iostream>
#include <vector>
#include <stack>
#include <set>

using namespace std;

struct Edge { int u, v; };

/**
 * ============================================================================
 * THE BLOCK-CUT TREE (BCC Tree)
 * ============================================================================
 * 
 * THE PROBLEM:
 * You are asked questions like: "If node C is destroyed, can node A still reach 
 * node B?" or "How many distinct simple paths exist between A and B?" 
 * (Classic: Codeforces "Tourist" or USACO problems).
 * 
 * THE TRICK:
 * Compress the graph into a bipartite tree consisting of two types of nodes:
 * 1. Square Nodes: Represent the Biconnected Components (BCCs).
 * 2. Round Nodes: Represent the original Articulation Points.
 * 
 * An edge exists only between an Articulation Point and a BCC that contains it. 
 * Non-articulation points just belong to exactly one BCC.
 * 
 * If A and B are in different parts of the graph, every path between them must 
 * pass through the Articulation Points on the path between their respective 
 * BCCs in the Block-Cut tree.
 * ============================================================================
 */
struct BlockCutTreeBuilder {
    int n, timer, bcc_count;
    vector<vector<int>> adj;
    vector<int> dfn, low;
    stack<Edge> st;
    
    vector<bool> is_ap;
    vector<vector<int>> bcc_nodes; // Lists of original nodes in each BCC
    
    // The resulting Block-Cut Tree
    // To keep it clean, tree nodes 0 to n-1 represent the original Articulation Points.
    // Tree nodes n to n+bcc_count-1 represent the BCCs (Blocks).
    vector<vector<int>> tree_adj;
    vector<int> node_to_tree_id; // Maps original node to its position in the tree

    BlockCutTreeBuilder(int n) : n(n), timer(0), bcc_count(0), 
                                 adj(n), dfn(n, -1), low(n, -1), is_ap(n, false), node_to_tree_id(n, -1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int parent = -1) {
        dfn[u] = low[u] = ++timer;
        int children = 0;
        
        for (int v : adj[u]) {
            if (v == parent) continue;
            
            if (dfn[v] != -1) {
                if (dfn[v] < dfn[u]) {
                    st.push({u, v});
                    low[u] = min(low[u], dfn[v]);
                }
            } else {
                children++;
                st.push({u, v});
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                
                if ((parent == -1 && children > 1) || (parent != -1 && low[v] >= dfn[u])) {
                    is_ap[u] = true;
                    set<int> unique_nodes; // Extract unique nodes from edges
                    while (true) {
                        Edge e = st.top(); st.pop();
                        unique_nodes.insert(e.u);
                        unique_nodes.insert(e.v);
                        if ((e.u == u && e.v == v) || (e.u == v && e.v == u)) break;
                    }
                    bcc_nodes.push_back(vector<int>(unique_nodes.begin(), unique_nodes.end()));
                    bcc_count++;
                }
            }
        }
    }

    void build_tree() {
        // 1. Find APs and BCCs
        for (int i = 0; i < n; ++i) {
            if (dfn[i] == -1) {
                dfs(i);
                if (!st.empty()) {
                    set<int> unique_nodes;
                    while (!st.empty()) {
                        Edge e = st.top(); st.pop();
                        unique_nodes.insert(e.u);
                        unique_nodes.insert(e.v);
                    }
                    bcc_nodes.push_back(vector<int>(unique_nodes.begin(), unique_nodes.end()));
                    bcc_count++;
                }
            }
        }

        // 2. Construct Block-Cut Tree
        int total_tree_nodes = n + bcc_count; 
        tree_adj.assign(total_tree_nodes, vector<int>());
        
        for (int i = 0; i < bcc_count; ++i) {
            int bcc_tree_node = n + i; // ID for this BCC in the tree
            
            for (int u : bcc_nodes[i]) {
                if (is_ap[u]) {
                    // Connect the AP to the BCC it belongs to
                    tree_adj[u].push_back(bcc_tree_node);
                    tree_adj[bcc_tree_node].push_back(u);
                    node_to_tree_id[u] = u; // APs map to themselves in the tree
                } else {
                    // Non-APs uniquely belong to this single BCC
                    node_to_tree_id[u] = bcc_tree_node;
                }
            }
        }
    }
};

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    int n = 5;
    BlockCutTreeBuilder bct(n);
    
    // Graph: 0-1-2 triangle, and 2-3-4 triangle. 
    // They share node 2 (which will be the Articulation Point)
    bct.add_edge(0, 1); bct.add_edge(1, 2); bct.add_edge(2, 0);
    bct.add_edge(2, 3); bct.add_edge(3, 4); bct.add_edge(4, 2);
    
    bct.build_tree();
    
    cout << "Total BCCs found: " << bct.bcc_count << "\n\n";
    
    for (int i = 0; i < n; ++i) {
        cout << "Original Node " << i << (bct.is_ap[i] ? " IS " : " is NOT ") 
             << "an Articulation Point.\n";
        cout << "  -> Mapped to Tree Node ID: " << bct.node_to_tree_id[i] << "\n";
    }
    
    cout << "\nBlock-Cut Tree Edges (AP <-> BCC):\n";
    for (int u = 0; u < n; ++u) { // Iterate over APs
        if (bct.is_ap[u]) {
            for (int bcc_node : bct.tree_adj[u]) {
                // bcc_node is guaranteed to be >= n
                cout << "AP Node " << u << " is inside BCC Node " << bcc_node << " (BCC index " << bcc_node - n << ")\n";
            }
        }
    }
    
    return 0;
}




#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/**
 * ============================================================================
 * BLOCK-CUT TREE QUERY SOLVER (LCA & Tree Prefix Sums)
 * ============================================================================
 * 
 * THE QUERIES:
 * 1. "If node C is destroyed, can A still reach B?"
 *    -> Translation: Is C an Articulation Point AND does it lie strictly on 
 *       the path between id(A) and id(B) in the Block-Cut Tree?
 * 
 * 2. "How many mandatory bottlenecks exist between A and B?"
 *    -> Translation: Count the number of Articulation Points on the path 
 *       between id(A) and id(B). If this is 0, they are in the same BCC 
 *       (multiple independent paths exist).
 * 
 * THE TRICK:
 * We use Binary Lifting to find the Lowest Common Ancestor (LCA).
 * - A node X is on the path between U and V if: dist(U, V) == dist(U, X) + dist(X, V)
 * - We can count APs by assigning a weight of 1 to AP nodes and 0 to BCC nodes,
 *   then using prefix sums down the tree: 
 *   path_sum = pref[U] + pref[V] - 2 * pref[LCA] + weight[LCA]
 * ============================================================================
 */
struct BCTQuerySolver {
    int tree_nodes, LOG;
    vector<vector<int>> up;
    vector<int> depth;
    vector<int> ap_pref; // Prefix sum of APs from root to node
    vector<int> comp_id; // To handle disconnected graphs (forests)
    
    // References to the BlockCutTreeBuilder's output
    const vector<vector<int>>& tree_adj;
    const vector<bool>& is_ap;
    const vector<int>& node_to_tree_id;

    BCTQuerySolver(int max_tree_nodes, 
                   const vector<vector<int>>& adj, 
                   const vector<bool>& ap_flags, 
                   const vector<int>& mapping) 
        : tree_nodes(max_tree_nodes), tree_adj(adj), is_ap(ap_flags), node_to_tree_id(mapping) {
        
        LOG = ceil(log2(tree_nodes)) + 1;
        up.assign(tree_nodes, vector<int>(LOG, -1));
        depth.assign(tree_nodes, 0);
        ap_pref.assign(tree_nodes, 0);
        comp_id.assign(tree_nodes, -1);
    }

    // DFS to initialize Binary Lifting and Prefix Sums
    void dfs(int u, int p, int d, int c_id) {
        up[u][0] = p;
        depth[u] = d;
        comp_id[u] = c_id;
        
        // Check if the current tree node represents an Articulation Point
        // In our BCT construction, nodes 0 to N-1 are APs.
        int weight = (u < is_ap.size() && is_ap[u]) ? 1 : 0;
        ap_pref[u] = (p == -1 ? 0 : ap_pref[p]) + weight;

        for (int i = 1; i < LOG; ++i) {
            if (up[u][i - 1] != -1) {
                up[u][i] = up[up[u][i - 1]][i - 1];
            } else {
                up[u][i] = -1;
            }
        }

        for (int v : tree_adj[u]) {
            if (v != p) {
                dfs(v, u, d + 1, c_id);
            }
        }
    }

    // Call this after building the solver to process all trees in the forest
    void build() {
        int current_comp = 0;
        for (int i = 0; i < tree_nodes; ++i) {
            if (comp_id[i] == -1 && !tree_adj[i].empty()) {
                dfs(i, -1, 0, current_comp++);
            }
        }
    }

    // Standard LCA implementation
    int get_lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        int diff = depth[u] - depth[v];
        
        for (int i = 0; i < LOG; ++i) {
            if ((diff >> i) & 1) {
                u = up[u][i];
            }
        }
        
        if (u == v) return u;
        
        for (int i = LOG - 1; i >= 0; --i) {
            if (up[u][i] != up[v][i]) {
                u = up[u][i];
                v = up[v][i];
            }
        }
        return up[u][0];
    }

    // Calculates the number of edges between u and v in the tree
    int get_dist(int u, int v) {
        int lca = get_lca(u, v);
        return depth[u] + depth[v] - 2 * depth[lca];
    }

    /**
     * QUERY 1: If original node C is destroyed, can original node A reach B?
     */
    bool can_reach_if_destroyed(int A, int B, int C) {
        int idA = node_to_tree_id[A];
        int idB = node_to_tree_id[B];
        int idC = node_to_tree_id[C];

        // 1. If A and B are in different disconnected components originally
        if (comp_id[idA] != comp_id[idB]) return false;
        
        // 2. If A or B is the node being destroyed
        if (A == C || B == C) return false;

        // 3. If C is NOT an Articulation Point, removing it only isolates itself
        if (!is_ap[C]) return true;

        // 4. If C IS an AP, check if it lies strictly on the path between idA and idB
        // In a tree, X is on the path between U and V iff dist(U,V) == dist(U,X) + dist(X,V)
        if (get_dist(idA, idB) == get_dist(idA, idC) + get_dist(idC, idB)) {
            return false; // C blocks the only way between them
        }
        
        return true;
    }

    /**
     * QUERY 2: How many Articulation Points exist STRICTLY between A and B?
     * If 0, A and B share a Biconnected Component (multiple paths exist).
     */
    int get_mandatory_bottlenecks(int A, int B) {
        int idA = node_to_tree_id[A];
        int idB = node_to_tree_id[B];
        
        if (comp_id[idA] != comp_id[idB]) return -1; // Unreachable

        int lca = get_lca(idA, idB);
        int lca_weight = (lca < is_ap.size() && is_ap[lca]) ? 1 : 0;
        
        // Total APs on the tree path inclusive of endpoints
        int total_aps = ap_pref[idA] + ap_pref[idB] - 2 * ap_pref[lca] + lca_weight;
        
        // Subtract if A or B themselves are APs (we only want strict bottlenecks between them)
        if (is_ap[A]) total_aps--;
        if (is_ap[B]) total_aps--;
        
        return total_aps;
    }
};

// ---------------------------------------------------------
// Assume bct is a fully built BlockCutTreeBuilder object 
// from the previous template.
// 
// int total_tree_nodes = bct.n + bct.bcc_count;
// BCTQuerySolver solver(total_tree_nodes, bct.tree_adj, bct.is_ap, bct.node_to_tree_id);
// solver.build();
// 
// bool ok = solver.can_reach_if_destroyed(0, 4, 2);
// int bottlenecks = solver.get_mandatory_bottlenecks(0, 4);
// ---------------------------------------------------------
