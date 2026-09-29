#include "../headers/Knight.h"
#include <iostream>

Knight::Knight(int col, int row, bool is_white) : Piece(col, row, is_white) {
    if (is_white) this->symbol = 'N';
    else this->symbol = 'n';
}

bool Knight::move(int new_col, int new_row, int player, Piece* const (&brd)[24][24]) {
    int col_abs_diff = std::abs(new_col - col);
    int row_abs_diff = std::abs(new_row - row);

    if ((col_abs_diff == 6 && row_abs_diff == 3) || (col_abs_diff == 3 && row_abs_diff == 6)) {
        // Check for obstacles
        if (brd[new_col][new_row] != nullptr) {
            if (brd[new_col][new_row]->isWhite() == isWhite()) {
                std::cout << "Cannot capture own piece." << std::endl;
                return false;
            }
        }
        col = new_col;
        row = new_row;
        has_moved = true;
        return true;
    }
    return false;
}