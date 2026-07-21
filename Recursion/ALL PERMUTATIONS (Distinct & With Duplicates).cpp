#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * ============================================================================
 * ALL PERMUTATIONS (Distinct & With Duplicates)
 * ============================================================================
 * 
 * THE PROBLEM:
 * Given a set of items, generate all possible arrangements (permutations).
 * E.g., [1, 2, 3] -> [1,2,3], [1,3,2], [2,1,3], [2,3,1], [3,1,2], [3,2,1].
 * 
 * THE TRICK:
 * 1. For DISTINCT elements: Use the "Swap" method. Swap the current index 
 *    with every element from `start` to the end. Recursively solve for `start + 1`.
 *    Backtrack by swapping them back.
 * 2. For DUPLICATE elements (Conditioned): The swap method gets messy. Instead, 
 *    sort the array first. Use a `used` boolean array. 
 *    To avoid duplicate permutations, enforce this rule: 
 *    "If the current element is identical to the previous one, we can only use 
 *    it IF the previous one was already used in this current recursive path."
 * ============================================================================
 */
struct PermutationSolver {
    
    // ---------------------------------------------------------
    // Method 1: Distinct Elements (O(N!) time, O(1) extra space)
    // ---------------------------------------------------------
    void generate_distinct(int start, vector<int>& nums, vector<vector<int>>& result) {
        if (start == nums.size()) {
            result.push_back(nums);
            return;
        }
        for (int i = start; i < nums.size(); ++i) {
            swap(nums[start], nums[i]);       // Choose
            generate_distinct(start + 1, nums, result); // Explore
            swap(nums[start], nums[i]);       // Un-choose (Backtrack)
        }
    }

    // ---------------------------------------------------------
    // Method 2: With Duplicates (O(N!) time, O(N) extra space)
    // ---------------------------------------------------------
    void generate_unique(vector<int>& nums, vector<bool>& used, vector<int>& current, vector<vector<int>>& result) {
        if (current.size() == nums.size()) {
            result.push_back(current);
            return;
        }
        for (int i = 0; i < nums.size(); ++i) {
            if (used[i]) continue;
            
            // Duplicate skipping condition:
            // If it's the same as the previous number AND the previous number 
            // wasn't used in this specific depth-branch, skip it.
            if (i > 0 && nums[i] == nums[i - 1] && !used[i - 1]) continue;

            used[i] = true;
            current.push_back(nums[i]);
            
            generate_unique(nums, used, current, result);
            
            used[i] = false;
            current.pop_back();
        }
    }

    // Wrapper for duplicates
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> current;
        vector<bool> used(nums.size(), false);
        sort(nums.begin(), nums.end()); // Crucial for duplicate checking
        generate_unique(nums, used, current, result);
        return result;
    }
};
