#pragma once
#include <string>
#include "Grid.h"

class Board {
    private:
        struct Move {
            int from_col, from_row, to_col, to_row;
            char promotion; // 'Q', 'R', 'B', 'N', or 0 for none
        };

        // Attributes:
        Grid grid;                  // owns the pieces standing on it
        int en_passant_col = -1;    // file of a pawn that just moved two squares, else -1
        bool in_check = false;
        bool game_over = false;
        std::string game_result;
        std::string message;        // what the last move() had to say, if anything

        // Methods:
        void place_pieces();
        bool parse(const std::string& text, Move& m) const;
        bool square_attacked(const Grid& g, int col, int row, bool by_white) const;
        bool king_in_check(const Grid& g, bool white) const;
        bool is_en_passant(const Move& m) const;
        bool is_castling(const Move& m) const;
        bool castling_allowed(const Move& m) const;
        bool follows_rules(const Move& m) const;
        Grid after(const Move& m, Piece** captured) const;
        bool is_legal(const Move& m) const;
        bool has_legal_move(bool white) const;
        void check_game_state(int turn);

    public:
        // ctor and dtor
        Board();
        ~Board();
        // The board owns its pieces, so it can't be copied
        Board(const Board& other) = delete;
        Board& operator=(const Board& other) = delete;

        void display_board() const;
        bool move(const std::string& move, int turn);
        bool is_game_over() const { return game_over; }
        std::string get_game_result() const { return game_result; }
        std::string get_message() const { return message; }
        bool is_in_check() const { return in_check; }   // the side to move is in check
        char piece_at(int col, int row) const;          // symbol, or '.' for an empty square
        std::string legal_moves(bool white) const;
};
