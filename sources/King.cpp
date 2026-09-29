#include "../headers/King.h"
#include <iostream>

King::King(int col, int row, bool is_white) : Piece(col, row, is_white) {
    if (is_white) this->symbol = 'K';
    else this->symbol = 'k';
}

bool King::move(int new_col, int new_row, int player, Piece* const (&brd)[24][24]) {
    int col_abs_diff = std::abs(new_col - col);
    int row_abs_diff = std::abs(new_row - row);

    if (col_abs_diff <= 3 && row_abs_diff <= 3) {
        // Check for obstacles
        if (brd[new_col][new_row] != nullptr) {
            if (brd[new_col][new_row]->isWhite() == isWhite()) {
                std::cout << "Cannot capture own piece." << std::endl;
                return false;
            }
        }
        col = new_col;
        row = new_row;
        has_moved = true;
        return true;
    }
    return false;
}

bool King::isInCheck(Piece* const (&brd)[24][24]) {
    // Check if the king is in check by any opponent piece

    // Walk outward from the king in all 8 directions.
    // Straight lines: Rook or Queen. Diagonals: Bishop or Queen (or Pawn, one step).
    // One step in any direction: the enemy King.
    int directions[8][2] = { {1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {1,-1}, {-1,1}, {-1,-1} };
    for (auto& dir : directions) {
        bool diagonal = dir[0] != 0 && dir[1] != 0;
        int n = 1;
        while (true) {
            int new_col = col + dir[0] * 3 * n;
            int new_row = row + dir[1] * 3 * n;
            if (new_col < 1 || new_col > 23 || new_row < 1 || new_row > 23) break; // out of bounds
            Piece* otherPiece = brd[new_col][new_row];
            if (otherPiece != nullptr) {
                if (otherPiece->isWhite() != isWhite()) {
                    char symbol = otherPiece->get_symbol();
                    if (diagonal && (symbol == 'b' || symbol == 'B' || symbol == 'q' || symbol == 'Q')) {
                        return true;
                    }
                    if (!diagonal && (symbol == 'r' || symbol == 'R' || symbol == 'q' || symbol == 'Q')) {
                        return true;
                    }
                    if (n == 1 && (symbol == 'k' || symbol == 'K')) {
                        return true;
                    }
                    // Check for pawn attack (white pawns move up the board, black pawns down)
                    if (diagonal && n == 1 &&
                        ((isWhite() && symbol == 'p' && dir[1] == -1) || (!isWhite() && symbol == 'P' && dir[1] == 1))) {
                        return true;
                    }
                }
                break; // blocked by any piece
            }
            n++;
        }
    }
    // Check for Knight attacks
    int knight_moves[8][2] = { {6,3}, {6,-3}, {-6,3}, {-6,-3}, {3,6}, {3,-6}, {-3,6}, {-3,-6} };
    for (auto& move : knight_moves) {
        int new_col = col + move[0];
        int new_row = row + move[1];
        if (new_col >= 1 && new_col <= 23 && new_row >= 1 && new_row <= 23) {
            if (brd[new_col][new_row] != nullptr && brd[new_col][new_row]->isWhite() != isWhite()) {
                char symbol = brd[new_col][new_row]->get_symbol();
                if (symbol == 'n' || symbol == 'N') {
                    return true;
                }
            }
        }
    }
    return false;
}