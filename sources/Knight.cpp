#include "../headers/Knight.h"

Knight::Knight(bool is_white) : Piece('N', is_white) {}

bool Knight::attacks(const Grid& g, int from_col, int from_row, int to_col, int to_row) const {
    // an L: two squares one way, one square the other. Jumps over pieces.
    (void)g;
    int col_diff = std::abs(to_col - from_col);
    int row_diff = std::abs(to_row - from_row);
    return (col_diff == 1 && row_diff == 2) || (col_diff == 2 && row_diff == 1);
}
