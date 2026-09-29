#pragma once
#include "Piece.h"

class Knight : public Piece {
public:
    Knight(bool is_white);
    bool attacks(const Grid& g, int from_col, int from_row, int to_col, int to_row) const override;
};
