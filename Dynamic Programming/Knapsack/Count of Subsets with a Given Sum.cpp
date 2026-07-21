#include <vector>

using namespace std;

/**
 * Count of Subsets with a Given Sum
 *
 * Return the number of subsets whose sum is exactly target.
 *
 * Time:  O(n * target)
 * Space: O(target)
 */
long long countSubsetsWithSum(const vector<int>& nums, int target) {
    vector<long long> dp(target + 1, 0);
    dp[0] = 1;

    for (int x : nums) {
        for (int s = target; s >= x; --s) {
            dp[s] += dp[s - x];
        }
    }

    return dp[target];
}
