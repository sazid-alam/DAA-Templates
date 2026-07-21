#include <vector>

using namespace std;

/**
 * Coin Change II (LeetCode 518)
 *
 * Return number of combinations to make amount using unlimited coins.
 *
 * Time:  O(n * amount)
 * Space: O(amount)
 */
long long coinChangeCombinationCount(const vector<int>& coins, int amount) {
    vector<long long> dp(amount + 1, 0);
    dp[0] = 1;

    for (int coin : coins) {
        for (int x = coin; x <= amount; ++x) {
            dp[x] += dp[x - coin];
        }
    }

    return dp[amount];
}
