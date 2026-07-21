#include <vector>
#include <numeric>

using namespace std;

/**
 * Target Sum (LeetCode 494)
 *
 * Count ways to assign + or - signs to make expression equal target.
 *
 * Time:  O(n * subsetTarget)
 * Space: O(subsetTarget)
 */
int targetSumWays(const vector<int>& nums, int target) {
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    if (target > totalSum || target < -totalSum) return 0;

    int transformed = totalSum + target;
    if (transformed % 2 != 0) return 0;

    int subsetTarget = transformed / 2;
    vector<long long> dp(subsetTarget + 1, 0);
    dp[0] = 1;

    for (int x : nums) {
        for (int s = subsetTarget; s >= x; --s) {
            dp[s] += dp[s - x];
        }
    }

    return (int)dp[subsetTarget];
}
