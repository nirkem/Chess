#pragma once
#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(int col, int row, bool is_white);
    bool move(int new_col, int new_row, int player, Piece* const (&brd)[24][24]) override;
};