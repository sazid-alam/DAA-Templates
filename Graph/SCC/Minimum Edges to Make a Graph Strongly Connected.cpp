/**
 * Calculates the minimum edges needed to strongly connect a graph.
 */
int min_edges_to_strongly_connect(const TarjanSCC& tarjan) {
    if (tarjan.scc_count == 1) return 0; // Already strongly connected
    
    int num_sccs = tarjan.scc_count;
    vector<int> in_deg(num_sccs, 0);
    vector<int> out_deg(num_sccs, 0);
    
    for (int u = 0; u < tarjan.n; ++u) {
        for (int v : tarjan.adj[u]) {
            if (tarjan.scc_id[u] != tarjan.scc_id[v]) {
                out_deg[tarjan.scc_id[u]]++;
                in_deg[tarjan.scc_id[v]]++;
            }
        }
    }
    
    int zero_in = 0, zero_out = 0;
    for (int i = 0; i < num_sccs; ++i) {
        if (in_deg[i] == 0) zero_in++;
        if (out_deg[i] == 0) zero_out++;
    }
    
    return max(zero_in, zero_out);
}
