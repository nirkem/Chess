#include "../headers/Queen.h"

Queen::Queen(bool is_white) : Piece('Q', is_white) {}

bool Queen::attacks(const Grid& g, int from_col, int from_row, int to_col, int to_row) const {
    // like a rook or a bishop
    bool straight = from_col == to_col || from_row == to_row;
    bool diagonal = std::abs(to_col - from_col) == std::abs(to_row - from_row);
    if (!straight && !diagonal) return false;
    return path_clear(g, from_col, from_row, to_col, to_row);
}
