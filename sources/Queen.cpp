#include "../headers/Queen.h"
#include <iostream>

Queen::Queen(int col, int row, bool is_white) : Piece(col, row, is_white) {
    if (is_white) this->symbol = 'Q';
    else this->symbol = 'q';
}

bool Queen::move(int new_col, int new_row, int player, Piece* const (&brd)[24][24]) {
    int col_abs_diff = std::abs(new_col - col);
    int row_abs_diff = std::abs(new_row - row);

    if (col_abs_diff == row_abs_diff || col == new_col || row == new_row) {
        // Check for obstacles
        if (col == new_col) {
            int step = (new_row > row) ? 3 : -3;
            for (int r = row + step; r != new_row; r += step) {
                if (brd[col][r] != nullptr) {
                    std::cout << "Path blocked for queen." << std::endl;
                    return false;
                }
            }
        } else if (row == new_row) {
            int step = (new_col > col) ? 3 : -3;
            for (int c = col + step; c != new_col; c += step) {
                if (brd[c][row] != nullptr) {
                    std::cout << "Path blocked for queen." << std::endl;
                    return false;
                }
            }
        } else {
            int col_step = (new_col > col) ? 3 : -3;
            int row_step = (new_row > row) ? 3 : -3;
            for (int c = col + col_step, r = row + row_step; c != new_col && r != new_row; c += col_step, r += row_step) {
                if (brd[c][r] != nullptr) {
                    std::cout << "Path blocked for queen." << std::endl;
                    return false;
                }
            }
        }
        // Check for capturing
        if (brd[new_col][new_row] != nullptr) {
            if (brd[new_col][new_row]->isWhite() == isWhite()) {
                std::cout << "Cannot capture own piece." << std::endl;
                return false;
            }
        }
        row = new_row;
        col = new_col;
        has_moved = true;
        return true;
    }
    return false;
}