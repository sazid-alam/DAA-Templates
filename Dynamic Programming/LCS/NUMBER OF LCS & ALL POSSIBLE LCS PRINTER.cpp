#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>

using namespace std;

/**
 * ============================================================================
 * NUMBER OF LCS & ALL POSSIBLE LCS PRINTER
 * ============================================================================
 * 
 * THE PROBLEM:
 * Given two strings `s` and `t`:
 * 1. Find the total number of distinct Longest Common Subsequences (LCS).
 * 2. Print all unique LCS sequences.
 * 
 * THE TRICK:
 * 1. Standard LCS DP: Build the 2D DP table where `dp[i][j]` is the length of 
 *    the LCS between `s[0...i-1]` and `t[0...j-1]`.
 * 2. Counting Paths (`count[i][j]`): Build a parallel count table. If characters 
 *    match (`s[i-1] == t[j-1]`), `count[i][j] = count[i-1][j-1]`. If not, we sum 
 *    the counts from the upper cell and the left cell (handling overlaps 
 *    where `dp[i-1][j] == dp[i][j-1]` to prevent double-counting).
 * 3. Backtracking: Use a DFS/backtracking approach from `(s.length(), t.length())` 
 *    back to `(0, 0)` to extract all paths, using a `std::set` to ensure unique strings.
 * ============================================================================
 */
struct LCSSolver {
    int max_len;
    long long total_count;
    set<string> unique_lcs;

    // Helper DFS to trace back and collect all unique LCS strings
    void backtrack(int i, int j, const string& s, const string& t, 
                   const vector<vector<int>>& dp, string& current, set<string>& result_set) {
        
        // Base case: Reached the boundary of either string
        if (i == 0 || j == 0) {
            string temp = current;
            reverse(temp.begin(), temp.end());
            result_set.insert(temp);
            return;
        }

        // If characters match, it's part of the optimal LCS path
        if (s[i - 1] == t[j - 1]) {
            current.push_back(s[i - 1]);
            backtrack(i - 1, j - 1, s, t, dp, current, result_set);
            current.pop_back(); // Backtrack
        } else {
            // If characters don't match, follow whichever direction(s) maintain the max LCS length
            if (dp[i - 1][j] >= dp[i][j - 1]) {
                backtrack(i - 1, j, s, t, dp, current, result_set);
            }
            if (dp[i][j - 1] >= dp[i - 1][j]) {
                backtrack(i, j - 1, s, t, dp, current, result_set);
            }
        }
    }

    void solve(const string& s, const string& t) {
        int n = s.length(), m = t.length();
        
        // Step 1: Standard LCS Length Table
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                if (s[i - 1] == t[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        max_len = dp[n][m];

        // Step 2: Number of LCS Paths Table (`cnt[i][j]`)
        vector<vector<long long>> cnt(n + 1, vector<long long>(m + 1, 0));
        for (int i = 0; i <= n; ++i) cnt[i][0] = 1;
        for (int j = 0; j <= m; ++j) cnt[0][j] = 1;

        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                if (s[i - 1] == t[j - 1]) {
                    cnt[i][j] = cnt[i - 1][j - 1];
                } else {
                    if (dp[i - 1][j] >= dp[i][j - 1]) cnt[i][j] += cnt[i - 1][j];
                    if (dp[i][j - 1] >= dp[i - 1][j]) cnt[i][j] += cnt[i][j - 1];
                    
                    // If both directions give the same LCS length, avoid double counting 
                    // the overlapping subproblem (inclusion-exclusion principle)
                    if (dp[i - 1][j] == dp[i][j - 1] && dp[i - 1][j] > 0) {
                        cnt[i][j] -= cnt[i - 1][j - 1];
                    }
                }
            }
        }
        total_count = cnt[n][m];

        // Step 3: Extract all unique LCS strings via backtracking
        unique_lcs.clear();
        string current = "";
        backtrack(n, m, s, t, dp, current, unique_lcs);
    }
};

// ---------------------------------------------------------
// Example Usage
// ---------------------------------------------------------
int main() {
    LCSSolver solver;
    
    string s = "abaaa";
    string t = "baab";
    
    solver.solve(s, t);
    
    cout << "Maximum LCS Length: " << solver.max_len << "\n";
    cout << "Total Number of LCS: " << solver.total_count << "\n\n";
    
    cout << "All Unique LCS Strings:\n";
    for (const string& seq : solver.unique_lcs) {
        cout << seq << "\n";
    }
    
    return 0;
}
