#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * ============================================================================
 * ALL COMBINATIONS / SUBSETS (Powerset)
 * ============================================================================
 * 
 * THE PROBLEM:
 * 1. Combinations: Pick exactly K elements from N elements.
 * 2. Subsets: Generate all 2^N possible groupings of elements.
 * 
 * THE TRICK:
 * Do not iterate from 0 to N inside the recursion. Instead, pass a `start` 
 * index. A subset is formed by deciding whether to include `nums[i]` or not, 
 * moving forward to `i + 1`. This naturally avoids order-based duplicates 
 * (like treating [1,2] and [2,1] as different).
 * 
 * To handle duplicate numbers (e.g., [1, 2, 2]), sort first. Inside the loop, 
 * if `i > start` and `nums[i] == nums[i-1]`, continue (skip the duplicate).
 * ============================================================================
 */
struct SubsetSolver {
    
    // ---------------------------------------------------------
    // Method 1: Subsets (All possible lengths from 0 to N)
    // ---------------------------------------------------------
    void generate_subsets(int start, vector<int>& nums, vector<int>& current, vector<vector<int>>& result) {
        // Every state we reach is a valid subset
        result.push_back(current);
        
        for (int i = start; i < nums.size(); ++i) {
            // Skip duplicates (requires sorted array)
            if (i > start && nums[i] == nums[i - 1]) continue;
            
            current.push_back(nums[i]);
            generate_subsets(i + 1, nums, current, result);
            current.pop_back();
        }
    }

    // ---------------------------------------------------------
    // Method 2: Combinations (Strictly length K)
    // ---------------------------------------------------------
    void generate_combinations(int start, int k, vector<int>& nums, vector<int>& current, vector<vector<int>>& result) {
        // Base case: exactly K elements chosen
        if (current.size() == k) {
            result.push_back(current);
            return;
        }
        
        // Pruning: If there aren't enough elements left to reach length K, stop early
        if (nums.size() - start < k - current.size()) {
            return; 
        }

        for (int i = start; i < nums.size(); ++i) {
            current.push_back(nums[i]);
            generate_combinations(i + 1, k, nums, current, result);
            current.pop_back();
        }
    }
};

int main() {
    SubsetSolver solver;
    
    // Use Case 1: Subsets (Powerset) of an array with duplicates
    vector<int> nums_for_subsets = {1, 2, 2};
    // MUST sort first for duplicate skipping to work!
    sort(nums_for_subsets.begin(), nums_for_subsets.end());
    
    vector<vector<int>> subsets;
    vector<int> current_subset;
    solver.generate_subsets(0, nums_for_subsets, current_subset, subsets);
    
    cout << "--- All Unique Subsets of [1, 2, 2] ---\n";
    for (const auto& sub : subsets) {
        cout << "[ ";
        for (int x : sub) cout << x << " ";
        cout << "]\n";
    }

    // Use Case 2: Combinations (N Choose K)
    vector<int> nums_for_combos = {1, 2, 3, 4};
    int k = 2; // Choose 2
    
    vector<vector<int>> combos;
    vector<int> current_combo;
    solver.generate_combinations(0, k, nums_for_combos, current_combo, combos);
    
    cout << "\n--- Combinations (4 Choose 2) ---\n";
    for (const auto& combo : combos) {
        cout << "[ ";
        for (int x : combo) cout << x << " ";
        cout << "]\n";
    }

    return 0;
}
