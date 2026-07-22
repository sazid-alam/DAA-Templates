#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

/**
 * Minimum Subset Sum Difference
 *
 * Partition nums into two subsets with minimum absolute difference of sums.
 *
 * Time:  O(n * totalSum)
 * Space: O(totalSum)
 */
int minimumSubsetSumDifference(const vector<int>& nums) {
    int totalSum = accumulate(nums.begin(), nums.end(), 0);
    vector<char> dp(totalSum + 1, 0);
    dp[0] = 1;

    for (int x : nums) {
        for (int s = totalSum; s >= x; --s) {
            dp[s] = dp[s] || dp[s - x];
        }
    }

    int best = totalSum;
    for (int s = 0; s <= totalSum / 2; ++s) {
        if (dp[s]) best = min(best, totalSum - 2 * s);
    }
    return best;
}
