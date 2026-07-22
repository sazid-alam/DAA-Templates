#include <vector>
#include <algorithm>

using namespace std;

/**
 * Coin Change (LeetCode 322)
 *
 * Return minimum number of coins to make amount, or -1 if impossible.
 *
 * Time:  O(n * amount)
 * Space: O(amount)
 */
int coinChangeMinCoins(const vector<int>& coins, int amount) {
    const int INF = amount + 1;
    vector<int> dp(amount + 1, INF);
    dp[0] = 0;

    for (int coin : coins) {
        for (int x = coin; x <= amount; ++x) {
            dp[x] = min(dp[x], dp[x - coin] + 1);
        }
    }

    return (dp[amount] == INF) ? -1 : dp[amount];
}
