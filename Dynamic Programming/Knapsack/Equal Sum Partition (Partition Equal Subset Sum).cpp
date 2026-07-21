#include <vector>
#include <numeric>

using namespace std;

/**
 * Equal Sum Partition
 *
 * Return true if nums can be partitioned into two subsets with equal sum.
 *
 * Time:  O(n * totalSum)
 * Space: O(totalSum)
 */
bool canPartitionEqualSubsetSum(const vector<int>& nums) {
    int total = accumulate(nums.begin(), nums.end(), 0);
    if (total % 2 != 0) return false;

    int target = total / 2;
    vector<char> dp(target + 1, 0);
    dp[0] = 1;

    for (int x : nums) {
        for (int s = target; s >= x; --s) {
            dp[s] = dp[s] || dp[s - x];
        }
    }

    return dp[target];
}
