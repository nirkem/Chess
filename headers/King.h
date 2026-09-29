#pragma once
#include "Piece.h"

class King : public Piece {
public:
    King(int col, int row, bool is_white);
    bool move(int new_col, int new_row, int player, Piece* const (&brd)[24][24]) override;
    bool isInCheck(Piece* const (&brd)[24][24]);
};