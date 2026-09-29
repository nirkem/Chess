#pragma once
#include "Piece.h"

class Rook : public Piece {
public:
    Rook(bool is_white);
    bool attacks(const Grid& g, int from_col, int from_row, int to_col, int to_row) const override;
};
