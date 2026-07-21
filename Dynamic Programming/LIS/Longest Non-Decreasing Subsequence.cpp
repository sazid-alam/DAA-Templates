#include <vector>
#include <algorithm>

using namespace std;

/**
 * Longest Non-Decreasing Subsequence
 *
 * Return LNDS length in O(n log n) using patience sorting variation.
 *
 * Time:  O(n log n)
 * Space: O(n)
 */
int lndsLength(const vector<int>& nums) {
    vector<int> tails;
    tails.reserve(nums.size());

    for (int x : nums) {
        auto it = upper_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }

    return (int)tails.size();
}
