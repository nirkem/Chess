#pragma once
#include <cctype>
#include <cstdlib>
#include "Grid.h"

// A piece knows its color and how it moves. Where it stands is tracked by the Board,
// so the move rules below are pure checks that never change anything.
class Piece {
protected:
    char symbol;            // capital for white, lowercase for black
    bool is_white;
    bool has_moved = false; // for castling

    // true when every square strictly between the two squares is empty.
    // Only meaningful for straight or diagonal lines (rook, bishop, queen).
    static bool path_clear(const Grid& g, int from_col, int from_row, int to_col, int to_row) {
        int col_step = (to_col > from_col) - (to_col < from_col);
        int row_step = (to_row > from_row) - (to_row < from_row);
        if (col_step == 0 && row_step == 0) return false;
        for (int c = from_col + col_step, r = from_row + row_step; c != to_col || r != to_row; c += col_step, r += row_step) {
            if (g.at(c, r) != nullptr) return false;
        }
        return true;
    }

public:
    Piece(char white_symbol, bool is_white)
        : symbol(is_white ? white_symbol : char(std::tolower(white_symbol))), is_white(is_white) {}
    virtual ~Piece() = default;

    // Does this piece attack (to_col, to_row) from (from_col, from_row)?
    // Ignores what stands on the target square.
    virtual bool attacks(const Grid& g, int from_col, int from_row, int to_col, int to_row) const = 0;

    // Can it move there, ignoring checks? By default: it attacks the square and no own piece stands there.
    virtual bool can_move(const Grid& g, int from_col, int from_row, int to_col, int to_row) const {
        Piece* target = g.at(to_col, to_row);
        return attacks(g, from_col, from_row, to_col, to_row) && (target == nullptr || target->is_white != is_white);
    }

    char get_symbol() const { return symbol; }
    char kind() const { return char(std::toupper(symbol)); } // 'P', 'N', 'B', 'R', 'Q' or 'K'
    bool isWhite() const { return is_white; }
    bool getHasMoved() const { return has_moved; }
    void set_has_moved(bool moved) { has_moved = moved; }
};
