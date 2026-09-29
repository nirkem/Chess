#pragma once
#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(bool is_white);
    bool attacks(const Grid& g, int from_col, int from_row, int to_col, int to_row) const override;
};
