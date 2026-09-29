#include "../headers/Rook.h"

Rook::Rook(bool is_white) : Piece('R', is_white) {}

bool Rook::attacks(const Grid& g, int from_col, int from_row, int to_col, int to_row) const {
    // along a rank or a file, nothing in between
    if (from_col != to_col && from_row != to_row) return false;
    return path_clear(g, from_col, from_row, to_col, to_row);
}
