#include "../headers/King.h"

King::King(bool is_white) : Piece('K', is_white) {}

bool King::attacks(const Grid& g, int from_col, int from_row, int to_col, int to_row) const {
    // one square in any direction (castling is handled by the Board)
    (void)g;
    int col_diff = std::abs(to_col - from_col);
    int row_diff = std::abs(to_row - from_row);
    return (col_diff | row_diff) != 0 && col_diff <= 1 && row_diff <= 1;
}
