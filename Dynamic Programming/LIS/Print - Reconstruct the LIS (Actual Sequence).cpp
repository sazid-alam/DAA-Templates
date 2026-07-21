#include <vector>
#include <algorithm>

using namespace std;

/**
 * Reconstruct one valid Longest Increasing Subsequence in O(n log n).
 */
vector<int> reconstructLIS(const vector<int>& nums) {
    int n = (int)nums.size();
    if (n == 0) return {};

    vector<int> tails;          // values of pile tails
    vector<int> tailsIndex;     // index in nums for each pile tail
    vector<int> parent(n, -1);  // previous index in LIS chain

    for (int i = 0; i < n; ++i) {
        int x = nums[i];
        int pos = (int)(lower_bound(tails.begin(), tails.end(), x) - tails.begin());

        if (pos == (int)tails.size()) {
            tails.push_back(x);
            tailsIndex.push_back(i);
        } else {
            tails[pos] = x;
            tailsIndex[pos] = i;
        }

        if (pos > 0) parent[i] = tailsIndex[pos - 1];
    }

    vector<int> lis;
    for (int cur = tailsIndex.back(); cur != -1; cur = parent[cur]) {
        lis.push_back(nums[cur]);
    }
    reverse(lis.begin(), lis.end());
    return lis;
}
