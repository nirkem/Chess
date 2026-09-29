#include "../headers/Pawn.h"

Pawn::Pawn(bool is_white) : Piece('P', is_white) {}

bool Pawn::attacks(const Grid& g, int from_col, int from_row, int to_col, int to_row) const {
    // one square diagonally forward
    (void)g;
    int forward = is_white ? 1 : -1;
    return to_row - from_row == forward && std::abs(to_col - from_col) == 1;
}

// Pawns move differently from how they capture (en passant is handled by the Board).
bool Pawn::can_move(const Grid& g, int from_col, int from_row, int to_col, int to_row) const {
    int forward = is_white ? 1 : -1;
    int start_row = is_white ? 1 : 6;

    if (to_col == from_col) {
        // straight ahead onto an empty square, or two squares from the starting rank
        if (to_row - from_row == forward) return g.at(to_col, to_row) == nullptr;
        if (to_row - from_row == 2 * forward && from_row == start_row)
            return g.at(to_col, from_row + forward) == nullptr && g.at(to_col, to_row) == nullptr;
        return false;
    }

    // diagonal capture of an enemy piece
    Piece* target = g.at(to_col, to_row);
    return attacks(g, from_col, from_row, to_col, to_row) && target != nullptr && target->isWhite() != is_white;
}
