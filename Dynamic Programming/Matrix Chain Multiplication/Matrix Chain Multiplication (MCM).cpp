#include <vector>
#include <limits>

using namespace std;

/**
 * Matrix Chain Multiplication (MCM)
 *
 * Input:
 * dims = [d0, d1, d2, ..., dn]
 * Matrix i has size dims[i-1] x dims[i], for i in [1..n]
 *
 * Returns minimum scalar multiplications needed to multiply all matrices.
 *
 * Time:  O(n^3)
 * Space: O(n^2)
 */
long long matrixChainMinCost(const vector<int>& dims) {
    int n = (int)dims.size() - 1;
    if (n <= 1) return 0;

    const long long INF = numeric_limits<long long>::max() / 4;
    vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, 0));

    for (int len = 2; len <= n; ++len) {
        for (int i = 1; i + len - 1 <= n; ++i) {
            int j = i + len - 1;
            dp[i][j] = INF;
            for (int k = i; k < j; ++k) {
                long long cost = dp[i][k] + dp[k + 1][j]
                               + 1LL * dims[i - 1] * dims[k] * dims[j];
                if (cost < dp[i][j]) dp[i][j] = cost;
            }
        }
    }

    return dp[1][n];
}
