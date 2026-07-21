#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * ============================================================================
 * LEXICOGRAPHICALLY SMALLEST LONGEST INCREASING SUBSEQUENCE (LIS)
 * ============================================================================
 * 
 * THE PROBLEM:
 * Find a Longest Increasing Subsequence (LIS) of an array such that, 
 * when comparing the actual values of the sequence from left to right, 
 * it is the lexicographically smallest among all possible valid LIS choices.
 * (Note: This is different from choosing the earliest indices; this minimizes 
 * the actual values sequence lexicographically, e.g., preferring [1, 2] over [1, 3]).
 * 
 * THE TRICK:
 * 1. Standard LIS DP: Find `dp[i]` = length of LIS ending at index `i`.
 * 2. Group indices by their LIS length into buckets: `lists[len]` stores all 
 *    original indices `i` where `dp[i] == len`. Because of how DP transitions work, 
 *    the values in these buckets will automatically be sorted in descending 
 *    order of their values.
 * 3. To make the sequence lexicographically smallest, we want the *smallest possible 
 *    value* at each position of our LIS. We iterate backwards from the maximum LIS 
 *    length down to 1. For each length, we greedily pick the valid index from the 
 *    bucket that has a value strictly smaller than our previously chosen element 
 *    and appears *after* it in the original array, while preferring the smallest 
 *    possible value.
 * ============================================================================
 */
struct LexicographicalLIS {
    
    vector<int> find_lexicographically_smallest_lis(const vector<int>& arr) {
        int n = arr.size();
        if (n == 0) return {};

        vector<int> dp(n, 1);
        int max_len = 1;

        // Step 1: Standard LIS DP to find lengths ending at each index
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (arr[j] < arr[i]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
            max_len = max(max_len, dp[i]);
        }

        // Step 2: Group indices by their LIS length
        // buckets[l] will contain indices `i` where dp[i] == l
        vector<vector<int>> buckets(max_len + 1);
        for (int i = 0; i < n; ++i) {
            buckets[dp[i]].push_back(i);
        }

        // Step 3: Greedily pick elements backwards to build the sequence
        vector<int> result_indices;
        int last_val = 2e9; // Infinity placeholder
        int last_idx = n;   // Out of bounds placeholder

        for (int len = max_len; len >= 1; --len) {
            int chosen_idx = -1;
            int chosen_val = 2e9;

            // Look through all candidate indices of the current LIS length
            for (int idx : buckets[len]) {
                // Conditions to pick:
                // 1. Must appear before our previously chosen element index (`idx < last_idx`)
                // 2. Its value must be strictly smaller than the previous element (`arr[idx] < last_val`)
                // 3. We want the absolute smallest value possible (`arr[idx] < chosen_val`)
                if (idx < last_idx && arr[idx] < last_val) {
                    if (arr[idx] < chosen_val) {
                        chosen_val = arr[idx];
                        chosen_idx = idx;
                    }
                }
            }

            result_indices.push_back(chosen_idx);
            last_val = chosen_val;
            last_idx = chosen_idx;
        }

        // Since we collected from max_len down to 1, reverse to get left-to-right order
        reverse(result_indices.begin(), result_indices.end());

        vector<int> lis_sequence;
        for (int idx : result_indices) {
            lis_sequence.push_back(arr[idx]);
        }

        return lis_sequence;
    }
};

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    LexicographicalLIS solver;
    
    // Example array where multiple LIS of the same length exist
    vector<int> arr = {3, 2, 6, 4, 5, 1};
    
    // Possible LIS of length 3: {3, 4, 5}, {2, 4, 5}, {1, ..., ...} etc.
    // Lexicographically smallest values sequence should be chosen.
    vector<int> result = solver.find_lexicographically_smallest_lis(arr);
    
    cout << "Lexicographically Smallest LIS: ";
    for (int x : result) {
        cout << x << " ";
    }
    cout << "\n";
    
    return 0;
}
