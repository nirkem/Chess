#include "../headers/Rook.h"
#include <iostream>

Rook::Rook(int col, int row, bool is_white) : Piece(col, row, is_white) {
    if (is_white) this->symbol = 'R';
    else this->symbol = 'r';
}

bool Rook::move(int new_col, int new_row, int player, Piece* const (&brd)[24][24]) {
    if (new_row != row && new_col != col) {
        std::cout << "Invalid move for rook." << std::endl;
        return false;
    }

    // Check for blocked path
    if (new_row > row) {
        for (int r = row + 3; r < new_row; r+=3) {
            if (brd[col][r] != nullptr) {
                std::cout << "Path blocked for rook." << std::endl;
                return false;
            }
        }
    } else if (new_row < row) {
        for (int r = new_row + 3; r < row; r+=3) {
            if (brd[col][r] != nullptr) {
                std::cout << "Path blocked for rook." << std::endl;
                return false;
            }
        }
    } else if (new_col > col) {
        for (int c = col + 3; c < new_col; c+=3) {
            if (brd[c][row] != nullptr) {
                std::cout << "Path blocked for rook." << std::endl;
                return false;
            }
        }
    } else if (new_col < col) {
        for (int c = new_col + 3; c < col; c+=3) {
            if (brd[c][row] != nullptr) {
                std::cout << "Path blocked for rook." << std::endl;
                return false;
            }
        }
    }

    // Check for capturing
    if (brd[new_col][new_row] != nullptr) {
        Piece* piece_to_kill = brd[new_col][new_row];
        if (piece_to_kill->isWhite() == is_white) {
            std::cout << "Invalid move for rook." << std::endl;
            std::cout << "Cannot capture own piece." << std::endl;
            return false;
        }
    }

    // Regular move
    col = new_col;
    row = new_row;
    has_moved = true;
    return true;
}