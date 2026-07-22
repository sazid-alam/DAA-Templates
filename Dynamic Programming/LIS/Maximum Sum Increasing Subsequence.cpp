#include <vector>
#include <algorithm>

using namespace std;

/**
 * Maximum Sum Increasing Subsequence
 *
 * Return the maximum possible sum of a strictly increasing subsequence.
 *
 * Time:  O(n^2)
 * Space: O(n)
 */
long long maximumSumIncreasingSubsequence(const vector<int>& nums) {
    int n = (int)nums.size();
    if (n == 0) return 0;

    vector<long long> dp(nums.begin(), nums.end());
    long long best = dp[0];

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (nums[j] < nums[i]) {
                dp[i] = max(dp[i], dp[j] + nums[i]);
            }
        }
        best = max(best, dp[i]);
    }

    return best;
}
