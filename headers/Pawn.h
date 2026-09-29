#pragma once
#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(bool is_white);
    bool attacks(const Grid& g, int from_col, int from_row, int to_col, int to_row) const override;
    bool can_move(const Grid& g, int from_col, int from_row, int to_col, int to_row) const override;
};
