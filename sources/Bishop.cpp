#include "../headers/Bishop.h"
#include <iostream>

Bishop::Bishop(int col, int row, bool is_white) : Piece(col, row, is_white) {
    if (is_white) this->symbol = 'B';
    else this->symbol = 'b';
}

bool Bishop::move(int new_col, int new_row, int player, Piece* const (&brd)[24][24]) {
    const char* color = is_white ? "white" : "black";
    int col_abs_diff = std::abs(new_col - col);
    int row_abs_diff = std::abs(new_row - row);
    if (new_row == row ||
        new_col == col ||
        col_abs_diff != row_abs_diff) {
        std::cout << "Invalid move for " << color << " bishop." << std::endl;
        return false;
    }

    // Check for blocked path (all four diagonals)
    int col_step = (new_col > col) ? 3 : -3;
    int row_step = (new_row > row) ? 3 : -3;
    for (int c = col + col_step, r = row + row_step; c != new_col; c += col_step, r += row_step) {
        if (brd[c][r] != nullptr) {
            std::cout << "Path blocked for " << color << " bishop." << std::endl;
            return false;
        }
    }

    // Check for capturing
    if (brd[new_col][new_row] != nullptr && brd[new_col][new_row]->isWhite() == is_white) {
        std::cout << "Invalid move for " << color << " bishop." << std::endl;
        std::cout << "Cannot capture own piece." << std::endl;
        return false;
    }

    // Regular move or capture
    col = new_col;
    row = new_row;
    has_moved = true;
    return true;
}
