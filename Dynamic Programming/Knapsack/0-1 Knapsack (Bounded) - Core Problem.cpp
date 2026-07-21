#include <vector>
#include <algorithm>

using namespace std;

/**
 * 0/1 Knapsack (Bounded) - Core Problem
 *
 * Given item weights, item values, and capacity W,
 * return maximum total value where each item can be taken at most once.
 *
 * Time:  O(n * W)
 * Space: O(W)
 */
int knapsack01(const vector<int>& weights, const vector<int>& values, int W) {
    int n = (int)weights.size();
    vector<int> dp(W + 1, 0);

    for (int i = 0; i < n; ++i) {
        for (int cap = W; cap >= weights[i]; --cap) {
            dp[cap] = max(dp[cap], values[i] + dp[cap - weights[i]]);
        }
    }

    return dp[W];
}
