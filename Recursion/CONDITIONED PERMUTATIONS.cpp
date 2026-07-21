#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/**
 * ============================================================================
 * CONDITIONED PERMUTATIONS 
 * ============================================================================
 * 
 * THE CONSTRAINTS (Based on your logic):
 * 1. Generate permutations of numbers from 1 to N.
 * 2. Adjacent numbers must have DIFFERENT parities (even/odd alternating).
 * 3. The absolute difference between adjacent numbers must be LESS THAN 5.
 * 
 * THE REFACTORING:
 * Instead of creating a `new_ava` array to track available numbers, we use 
 * a boolean array `used`. 
 * Instead of creating a `new_curr` array, we push the number to `curr`, 
 * go down the recursive tree, and then pop it off (Backtracking). 
 * This reduces the memory complexity at each step from O(N) to O(1).
 * ============================================================================
 */
struct ConditionedPermutation {
    int n;
    vector<int> curr;           // Replaces 'x' (Current state)
    vector<bool> used;          // Replaces 'y' / 'ava' (O(1) lookup for availability)
    vector<vector<int>> results; // Stores all valid permutations

    // Constructor initializes the bounds and the used array (1-indexed)
    ConditionedPermutation(int n) : n(n), used(n + 1, false) {}

    void solve() {
        // Base case: We successfully placed 'n' elements
        if (curr.size() == n) {
            results.push_back(curr);
            return;
        }

        // Try all possible numbers from 1 to n (Iterating through 'ava')
        for (int u = 1; u <= n; ++u) {
            
            // If the number is already in our permutation, skip it
            if (used[u]) continue; 

            // If the permutation isn't empty, enforce your custom constraints
            if (!curr.empty()) {
                int last = curr.back(); // Same as curr[curr.size() - 1]
                
                bool alternating_parity = (last % 2 != u % 2);
                bool diff_less_than_5 = (abs(last - u) < 5);

                // If it fails either constraint, prune this branch (skip to next number)
                if (!alternating_parity || !diff_less_than_5) {
                    continue;
                }
            }

            // --- THE BACKTRACKING CORE ---
            
            // 1. CHOOSE (Apply state)
            curr.push_back(u);
            used[u] = true;

            // 2. EXPLORE (Recurse)
            solve();

            // 3. UN-CHOOSE (Revert state so the next loop iteration is clean)
            curr.pop_back();
            used[u] = false;
        }
    }

    void print_results() {
        if (results.empty()) {
            cout << "No valid permutations found for N = " << n << ".\n";
            return;
        }

        for (const auto& perm : results) {
            cout << "[ ";
            for (int val : perm) {
                cout << val << " ";
            }
            cout << "]\n";
        }
        cout << "Total valid paths found: " << results.size() << "\n";
    }
};

// ---------------------------------------------------------
// Example Use Cases
// ---------------------------------------------------------
int main() {
    
    // USE CASE 1: N = 4
    // Should easily find sequences like [1, 2, 3, 4] and [4, 3, 2, 1] 
    // since differences are small.
    cout << "--- Test Case: N = 4 ---\n";
    ConditionedPermutation solver1(4);
    solver1.solve();
    solver1.print_results();
    cout << "\n";

    // USE CASE 2: N = 6
    // The "diff < 5" constraint starts heavily pruning the tree here.
    // For example, [1, 6] is invalid because abs(1-6) = 5 (which is not < 5), 
    // even though 1 and 6 have different parities.
    cout << "--- Test Case: N = 6 ---\n";
    ConditionedPermutation solver2(6);
    solver2.solve();
    solver2.print_results();
    cout << "\n";

    return 0;
}
