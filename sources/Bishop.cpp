#include "../headers/Bishop.h"

Bishop::Bishop(bool is_white) : Piece('B', is_white) {}

bool Bishop::attacks(const Grid& g, int from_col, int from_row, int to_col, int to_row) const {
    // along a diagonal, nothing in between
    if (std::abs(to_col - from_col) != std::abs(to_row - from_row)) return false;
    return path_clear(g, from_col, from_row, to_col, to_row);
}
