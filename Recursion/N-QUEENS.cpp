#include <iostream>
#include <vector>
#include <string>

using namespace std;

/**
 * ============================================================================
 * CLASSIC CONDITIONED BACKTRACKING: N-QUEENS
 * ============================================================================
 * 
 * THE PROBLEM:
 * Place N chess queens on an N x N chessboard so that no two queens attack 
 * each other (horizontally, vertically, or diagonally).
 * 
 * THE TRICK:
 * Do not scan the board to check if a spot is safe. It is too slow (O(N) per check).
 * Instead, use 3 boolean arrays (or bitmasks) to track constraints:
 * 1. `cols[c]`: Is there a queen in column `c`?
 * 2. `diag1[r + c]`: Is there a queen on the minor diagonal? (All cells on a 
 *    bottom-left to top-right diagonal share the same `r + c` sum).
 * 3. `diag2[r - c + N - 1]`: Is there a queen on the major diagonal? (All cells 
 *    on a top-left to bottom-right diagonal share the same `r - c` difference).
 * 
 * You process row by row, so row conflicts are naturally impossible.
 * ============================================================================
 */
struct NQueensSolver {
    int n;
    vector<bool> cols, diag1, diag2;
    vector<string> board;
    vector<vector<string>> results;

    NQueensSolver(int size) : n(size), cols(size, false), 
                              diag1(2 * size, false), diag2(2 * size, false) {
        board.assign(n, string(n, '.'));
    }

    void solve(int row = 0) {
        if (row == n) {
            results.push_back(board);
            return;
        }

        for (int col = 0; col < n; ++col) {
            int d1 = row + col;
            int d2 = row - col + n - 1;

            // Check if the current column or diagonals are under attack
            if (cols[col] || diag1[d1] || diag2[d2]) continue;

            // Place Queen (Choose)
            board[row][col] = 'Q';
            cols[col] = diag1[d1] = diag2[d2] = true;

            // Move to next row (Explore)
            solve(row + 1);

            // Remove Queen (Backtrack)
            board[row][col] = '.';
            cols[col] = diag1[d1] = diag2[d2] = false;
        }
    }
};
