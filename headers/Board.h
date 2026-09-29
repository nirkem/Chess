#pragma once
#include <vector>
#include "Piece.h"
#include "King.h"
#include <string>
#include <algorithm>
using namespace std;

class Board {
    private:
        // Attributes:
        bool in_check = false;
        bool game_over = false;
        string game_result;
        vector<int> legal_col_row;
        Piece* brd[24][24]{};
        King* white_king;
        King* black_king;
        vector<Piece*> pieces;
        vector<Piece*> white_pieces;
        vector<Piece*> white_Pawns;
        vector<Piece*> black_pieces;
        vector<Piece*> black_Pawns;

        // Methods:
        void place_pieces();
        void create_pawns();
        void create_rooks();
        void create_bishops();
        void create_knights();
        void create_royalty();
        void delete_piece(Piece& piece);
        void add_piece(Piece* piece);
        bool try_move(Piece* piece, int dst_col, int dst_row, int turn, bool keep);
        bool has_legal_move(int turn);
        void checkForCheck(int turn);

    public:
        // ctor and dtor
        Board();
        ~Board();
        // Copy ctor
        Board(const Board& other);
        Board& operator=(const Board& other);
        // Move ctor
        Board(Board&& other) noexcept;
        Board& operator=(Board&& other) noexcept;

        void display_board();
        bool move(string move, int turn);
        bool is_game_over() const { return game_over; }
        string get_game_result() const { return game_result; }

        vector<Piece*> get_pieces();
        vector<Piece*> get_white_pieces();
        vector<Piece*> get_white_Pawns();
        vector<Piece*> get_black_pieces();
        vector<Piece*> get_black_Pawns();
};
