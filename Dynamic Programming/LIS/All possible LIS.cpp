#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

/**
 * ============================================================================
 * PRINT ALL POSSIBLE LONGEST INCREASING SUBSEQUENCES (LIS)
 * ============================================================================
 * 
 * THE PROBLEM:
 * Given an array, find and print EVERY unique sequence that qualifies as a 
 * Longest Increasing Subsequence (LIS).
 * 
 * THE TRICK:
 * 1. Run standard DP to find `dp[i]` (length of LIS ending at index `i`) and 
 *    track the absolute `max_len`.
 * 2. Identify all "start points" (indices `i` where `dp[i] == max_len`).
 * 3. Use backtracking/DFS. From an index `curr`, look backwards for any 
 *    preceding index `prev` such that `arr[prev] < arr[curr]` and 
 *    `dp[prev] == dp[curr] - 1`. 
 * 4. Accumulate paths. Since we trace backwards, reverse each path at the end, 
 *    and use a `std::set` to automatically weed out duplicate identical sequences 
 *    if multiple paths yield the exact same values.
 * ============================================================================
 */
struct AllPossibleLIS {
    vector<vector<int>> all_sequences;

    void backtrack(int curr_idx, const vector<int>& arr, const vector<vector<int>>& prev_links, 
                   vector<int>& current_path, set<vector<int>>& unique_results) {
        
        current_path.push_back(arr[curr_idx]);

        // Base case: If this index has no more previous links, we reached the start of an LIS
        if (prev_links[curr_idx].empty()) {
            vector<int> complete_path = current_path;
            reverse(complete_path.begin(), complete_path.end()); // Put left-to-right
            
            if (unique_results.find(complete_path) == unique_results.end()) {
                unique_results.insert(complete_path);
                all_sequences.push_back(complete_path);
            }
        } else {
            // Recurse through all valid predecessor indices
            for (int prev_idx : prev_links[curr_idx]) {
                backtrack(prev_idx, arr, prev_links, current_path, unique_results);
            }
        }

        // Backtrack
        current_path.pop_back();
    }

    vector<vector<int>> find_all_lis(const vector<int>& arr) {
        int n = arr.size();
        if (n == 0) return {};

        vector<int> dp(n, 1);
        int max_len = 1;

        // Step 1: Standard LIS DP
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (arr[j] < arr[i]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
            max_len = max(max_len, dp[i]);
        }

        // Step 2: Build backward links to all valid predecessors
        // prev_links[i] will store all indices `j` (where j < i) that can legally precede `i` in an LIS
        vector<vector<int>> prev_links(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (arr[j] < arr[i] && dp[j] == dp[i] - 1) {
                    prev_links[i].push_back(j);
                }
            }
        }

        // Step 3: Find all endpoints (indices where dp[i] == max_len) and backtrack
        set<vector<int>> unique_results;
        all_sequences.clear();

        for (int i = 0; i < n; ++i) {
            if (dp[i] == max_len) {
                vector<int> current_path;
                backtrack(i, arr, prev_links, current_path, unique_results);
            }
        }

        return all_sequences;
    }
};

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    AllPossibleLIS solver;
    
    // Array with multiple valid LIS paths of the same max length
    vector<int> arr = {1, 4, 3, 2, 5};
    // Max length is 3. Possible LIS: {1, 4, 5}, {1, 3, 5}, {1, 2, 5}
    
    vector<vector<int>> results = solver.find_all_lis(arr);
    
    cout << "All Possible LIS Sequences:\n";
    for (const auto& seq : results) {
        cout << "[ ";
        for (int x : seq) {
            cout << x << " ";
        }
        cout << "]\n";
    }
    
    return 0;
}
