#include <vector>
#include <algorithm>

using namespace std;

/**
 * Number of Longest Increasing Subsequences (LeetCode 673)
 *
 * Returns the count of LIS.
 *
 * Time:  O(n^2)
 * Space: O(n)
 */
int countLIS(const vector<int>& nums) {
    int n = (int)nums.size();
    if (n == 0) return 0;

    vector<int> len(n, 1), ways(n, 1);
    int best = 1;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (nums[j] < nums[i]) {
                if (len[j] + 1 > len[i]) {
                    len[i] = len[j] + 1;
                    ways[i] = ways[j];
                } else if (len[j] + 1 == len[i]) {
                    ways[i] += ways[j];
                }
            }
        }
        best = max(best, len[i]);
    }

    int total = 0;
    for (int i = 0; i < n; ++i) {
        if (len[i] == best) total += ways[i];
    }
    return total;
}
