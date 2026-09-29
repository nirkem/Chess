#pragma once
#include "Piece.h"

class Rook : public Piece {
public:
    Rook(int col, int row, bool is_white);
    bool move(int new_col, int new_row, int player, Piece* const (&brd)[24][24]) override;
};