#include <vector>
#include <algorithm>

using namespace std;

/**
 * Longest Increasing Subsequence - Core Problem
 *
 * Return LIS length in O(n log n) using patience sorting idea.
 */
int lisLength(const vector<int>& nums) {
    vector<int> tails;
    tails.reserve(nums.size());

    for (int x : nums) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }

    return (int)tails.size();
}
