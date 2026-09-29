#include "../headers/Pawn.h"
#include <iostream>

Pawn::Pawn(int col, int row, bool is_white) : Piece(col, row, is_white) {
    if (is_white) this->symbol = 'P';
    else this->symbol = 'p';
}

bool Pawn::move(int new_col, int new_row, int player, Piece* const (&brd)[24][24]) {
    // handle WHITE
    if (is_white) {

        if (new_row >= row ||
            new_row < row - 6 ||
            new_col < col - 3 ||
            new_col > col + 3 ||
            (new_col == col + 3 && new_row == row - 6) ||
            (new_col == col - 3 && new_row == row - 6) ||
            (new_row < row - 3 && has_moved)) {
                std::cout << "Invalid move for white pawn." << std::endl;
                return false;
        }
        // handle move
        if (new_col == col) {
            if (brd[col][row - 3] != nullptr) return false;
            if (new_row == row - 3) {
                col = new_col;
                row = new_row;
                has_moved = true;
                return true;
            }
            else if (new_row == row - 6) {
                if (brd[new_col][new_row] != nullptr) return false;
                else {
                    col = new_col;
                    row = new_row;
                    has_moved = true;
                    return true;
                }
            }
        } // handle KILL move
        else {
            Piece* piece_to_kill = brd[new_col][new_row];
            if (piece_to_kill == nullptr || piece_to_kill->isWhite()) return false;
            else {
                col = new_col;
                row = new_row;
                has_moved = true;
                return true;
            }
        }
    }
    // handle BLACK
    else {
        if (new_row <= row ||
            new_row > row + 6 ||
            new_col < col - 3 ||
            new_col > col + 3 ||
            (new_col == col + 3 && new_row == row + 6) ||
            (new_col == col - 3 && new_row == row + 6) ||
            (new_row > row + 3 && has_moved)) {
                std::cout << "Invalid move for black pawn." << std::endl;
                return false;
        }
        // handle move
        if (new_col == col) {
            if (brd[col][row + 3] != nullptr) {
                std::cout << "Path blocked for black pawn." << std::endl;
                Piece* blocking_piece = brd[new_col][new_row];
                std::cout << "Blocking piece symbol: " << (blocking_piece ? blocking_piece->get_symbol() : ' ') << std::endl;
                return false;
            } 
            if (new_row == row + 3) {
                col = new_col;
                row = new_row;
                has_moved = true;
                return true;
            }
            else if (new_row == row + 6) {
                if (brd[new_col][new_row] != nullptr) {
                    std::cout << "Path blocked for black pawn. cannot jump 2 squares" << std::endl;
                    Piece* blocking_piece = brd[new_col][new_row];
                    std::cout << "Blocking piece symbol: " << (blocking_piece ? blocking_piece->get_symbol() : ' ') << std::endl;
                    return false;
                } 
                else {
                    col = new_col;
                    row = new_row;
                    has_moved = true;
                    return true;
                }
            }
        } // handle KILL move
        else {
            Piece* piece_to_kill = brd[new_col][new_row];
            if (piece_to_kill == nullptr || !piece_to_kill->isWhite()) return false;
            else {
                col = new_col;
                row = new_row;
                has_moved = true;
                return true;
            }
        }
    }
    return true;
}