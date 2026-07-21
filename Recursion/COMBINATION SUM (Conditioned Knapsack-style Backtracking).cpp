#include <iostream>
#include <vector>

using namespace std;

/**
 * ============================================================================
 * COMBINATION SUM (Conditioned Knapsack-style Backtracking)
 * ============================================================================
 * 
 * THE PROBLEM:
 * Given an array of integers and a target sum, find all unique combinations 
 * that add up to the target. 
 * Variation 1: You can use the same number an UNLIMITED number of times.
 * Variation 2: You can use each number only ONCE.
 * 
 * THE TRICK:
 * Keep a running `target`.
 * - If target == 0: valid combination found.
 * - If target < 0: dead end, backtrack immediately.
 * 
 * To allow UNLIMITED re-use: when recurring, pass `i` as the start index.
 * To allow SINGLE use: when recurring, pass `i + 1` as the start index, 
 * and apply the standard duplicate-skipping trick (`i > start && nums[i] == nums[i-1]`).
 * ============================================================================
 */
struct CombinationSumSolver {

    // ---------------------------------------------------------
    // Unlimited Reuse of Elements
    // ---------------------------------------------------------
    void combination_sum_unlimited(int start, int target, const vector<int>& nums, vector<int>& current, vector<vector<int>>& result) {
        if (target == 0) {
            result.push_back(current);
            return;
        }
        
        for (int i = start; i < nums.size(); ++i) {
            if (target - nums[i] < 0) break; // Optimization: requires sorted array
            
            current.push_back(nums[i]);
            // Pass 'i' instead of 'i + 1' to allow reusing the same element
            combination_sum_unlimited(i, target - nums[i], nums, current, result);
            current.pop_back();
        }
    }

    // ---------------------------------------------------------
    // Single Use of Elements (With Duplicate Handling)
    // ---------------------------------------------------------
    void combination_sum_single(int start, int target, const vector<int>& nums, vector<int>& current, vector<vector<int>>& result) {
        if (target == 0) {
            result.push_back(current);
            return;
        }
        
        for (int i = start; i < nums.size(); ++i) {
            if (target - nums[i] < 0) break; // Optimization: requires sorted array
            
            // Skip duplicates at the same recursive depth
            if (i > start && nums[i] == nums[i - 1]) continue;
            
            current.push_back(nums[i]);
            // Pass 'i + 1' to move to the next element
            combination_sum_single(i + 1, target - nums[i], nums, current, result);
            current.pop_back();
        }
    }
};
