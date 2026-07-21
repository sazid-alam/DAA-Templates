#include <vector>
#include <algorithm>

using namespace std;

/**
 * Longest Bitonic Subsequence
 *
 * Return maximum length of a subsequence that first increases then decreases.
 *
 * Time:  O(n^2)
 * Space: O(n)
 */
int longestBitonicSubsequenceLength(const vector<int>& nums) {
    int n = (int)nums.size();
    if (n == 0) return 0;

    vector<int> lis(n, 1), lds(n, 1);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (nums[j] < nums[i]) lis[i] = max(lis[i], lis[j] + 1);
        }
    }

    for (int i = n - 1; i >= 0; --i) {
        for (int j = n - 1; j > i; --j) {
            if (nums[j] < nums[i]) lds[i] = max(lds[i], lds[j] + 1);
        }
    }

    int best = 1;
    for (int i = 0; i < n; ++i) {
        best = max(best, lis[i] + lds[i] - 1);
    }
    return best;
}
