#pragma once

class Piece;

// The 8x8 board: sq[col][row], col 0 = file a, row 0 = rank 1.
// It's a plain value, so copying it is a cheap way to try a move without touching the real board.
struct Grid {
    Piece* sq[8][8]{};

    Piece* at(int col, int row) const { return sq[col][row]; }
};
