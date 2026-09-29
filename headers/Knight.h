#pragma once
#include "Piece.h"

class Knight : public Piece {
public:
    Knight(int col, int row, bool is_white);
    bool move(int new_col, int new_row, int player, Piece* const (&brd)[24][24]) override;
};