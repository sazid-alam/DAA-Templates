#include <vector>

using namespace std;

/**
 * Subset Sum Problem
 *
 * Return true if there exists a subset with sum exactly target.
 *
 * Time:  O(n * target)
 * Space: O(target)
 */
bool subsetSumExists(const vector<int>& nums, int target) {
    vector<char> dp(target + 1, 0);
    dp[0] = 1;

    for (int x : nums) {
        for (int s = target; s >= x; --s) {
            dp[s] = dp[s] || dp[s - x];
        }
    }

    return dp[target];
}
