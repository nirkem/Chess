#include "../headers/Board.h"
#include "../headers/Pawn.h"
#include "../headers/Rook.h"
#include "../headers/Knight.h"
#include "../headers/Queen.h"
#include "../headers/Bishop.h"
#include "../headers/King.h"
#include <iostream>
#include <cctype>
#include <cstdlib>

using namespace std;


// ------- Ctor Dtor ---------
Board::Board() {
    place_pieces();
}

Board::~Board() {
    for (auto& column : grid.sq) {
        for (Piece* p : column) {
            delete p;
        }
    }
}


// ------- Creating pieces ---------
void Board::place_pieces() {
    for (int white = 0; white <= 1; white++) {
        int back_row = white ? 0 : 7;
        int pawn_row = white ? 1 : 6;

        grid.sq[0][back_row] = new Rook(white);
        grid.sq[1][back_row] = new Knight(white);
        grid.sq[2][back_row] = new Bishop(white);
        grid.sq[3][back_row] = new Queen(white);
        grid.sq[4][back_row] = new King(white);
        grid.sq[5][back_row] = new Bishop(white);
        grid.sq[6][back_row] = new Knight(white);
        grid.sq[7][back_row] = new Rook(white);

        for (int col = 0; col < 8; col++) {
            grid.sq[col][pawn_row] = new Pawn(white);
        }
    }
}


// ------- Display ---------
void Board::display_board() const {
    // Each square is drawn 3 cells wide and 3 lines tall, each cell 2 characters
    const string light = "  ";
    const string dark = "██"; // two full blocks

    cout << endl << endl;
    for (int row = 7; row >= 0; row--) {
        for (int line = 0; line < 3; line++) {
            for (int col = 0; col < 8; col++) {
                const string& fill = ((col + row) % 2 == 0) ? dark : light; // a1 is dark
                Piece* piece = grid.at(col, row);

                cout << fill;
                if (line == 1 && piece != nullptr) cout << piece->get_symbol() << ' ';
                else cout << fill;
                cout << fill;
            }
            if (line == 1) cout << " | " << row + 1;
            cout << endl;
        }
    }

    cout << "  ";
    for (int col = 0; col < 8; col++) cout << "_     ";
    cout << endl << "  ";
    for (int col = 0; col < 8; col++) cout << char('a' + col) << "     ";
    cout << endl;
}


// ------- Reading moves ---------
// "e2e4" or "e7e8Q": origin square, destination square, optional promotion piece
bool Board::parse(const string& text, Move& m) const {
    if (text.size() != 4 && text.size() != 5) return false;
    if (text[0] < 'a' || text[0] > 'h' || text[2] < 'a' || text[2] > 'h') return false;
    if (text[1] < '1' || text[1] > '8' || text[3] < '1' || text[3] > '8') return false;

    m.from_col = text[0] - 'a';
    m.from_row = text[1] - '1';
    m.to_col = text[2] - 'a';
    m.to_row = text[3] - '1';
    m.promotion = (text.size() == 5) ? char(toupper(text[4])) : 0;
    return true;
}


// ------- Rules ---------
bool Board::square_attacked(const Grid& g, int col, int row, bool by_white) const {
    for (int c = 0; c < 8; c++) {
        for (int r = 0; r < 8; r++) {
            Piece* p = g.at(c, r);
            if (p != nullptr && p->isWhite() == by_white && (c != col || r != row) && p->attacks(g, c, r, col, row)) {
                return true;
            }
        }
    }
    return false;
}

bool Board::king_in_check(const Grid& g, bool white) const {
    for (int c = 0; c < 8; c++) {
        for (int r = 0; r < 8; r++) {
            Piece* p = g.at(c, r);
            if (p != nullptr && p->kind() == 'K' && p->isWhite() == white) {
                return square_attacked(g, c, r, !white);
            }
        }
    }
    return false;
}

// A pawn capturing a pawn that just moved two squares past it
bool Board::is_en_passant(const Move& m) const {
    Piece* piece = grid.at(m.from_col, m.from_row);
    if (piece == nullptr || piece->kind() != 'P' || m.to_col != en_passant_col) return false;

    int forward = piece->isWhite() ? 1 : -1;
    int passed_row = piece->isWhite() ? 5 : 2; // the square the other pawn skipped
    Piece* passed_pawn = grid.at(m.to_col, m.from_row);

    return m.to_row == passed_row && m.to_row - m.from_row == forward &&
           abs(m.to_col - m.from_col) == 1 && grid.at(m.to_col, m.to_row) == nullptr &&
           passed_pawn != nullptr && passed_pawn->kind() == 'P' && passed_pawn->isWhite() != piece->isWhite();
}

bool Board::is_castling(const Move& m) const {
    Piece* piece = grid.at(m.from_col, m.from_row);
    return piece != nullptr && piece->kind() == 'K' && m.from_row == m.to_row && abs(m.to_col - m.from_col) == 2;
}

// King to g1/c1 (g8/c8): neither piece has moved, nothing in between,
// and the king isn't in check and doesn't pass through or land on an attacked square.
bool Board::castling_allowed(const Move& m) const {
    Piece* king = grid.at(m.from_col, m.from_row);
    bool white = king->isWhite();
    int home = white ? 0 : 7;
    if (king->getHasMoved() || m.from_col != 4 || m.from_row != home) return false;

    int rook_col = (m.to_col == 6) ? 7 : 0;
    Piece* rook = grid.at(rook_col, home);
    if (rook == nullptr || rook->kind() != 'R' || rook->isWhite() != white || rook->getHasMoved()) return false;

    for (int c = min(4, rook_col) + 1; c < max(4, rook_col); c++) {
        if (grid.at(c, home) != nullptr) return false;
    }

    int step = (m.to_col > 4) ? 1 : -1;
    for (int c = 4; c != m.to_col + step; c += step) {
        if (square_attacked(grid, c, home, !white)) return false;
    }
    return true;
}

// Does the move follow the moving piece's rules? (Doesn't look at checks yet.)
bool Board::follows_rules(const Move& m) const {
    Piece* piece = grid.at(m.from_col, m.from_row);
    if (is_castling(m)) return castling_allowed(m);
    if (is_en_passant(m)) return true;
    return piece->can_move(grid, m.from_col, m.from_row, m.to_col, m.to_row);
}

// The position after the move. Only rearranges pointers: nothing is created or deleted,
// so it's safe for trying moves. The captured piece (if any) is returned through `captured`.
Grid Board::after(const Move& m, Piece** captured) const {
    Grid next = grid;
    Piece* piece = next.sq[m.from_col][m.from_row];
    *captured = next.sq[m.to_col][m.to_row];

    if (is_en_passant(m)) {
        *captured = next.sq[m.to_col][m.from_row];
        next.sq[m.to_col][m.from_row] = nullptr;
    }
    if (is_castling(m)) {
        int rook_from = (m.to_col > m.from_col) ? 7 : 0;
        int rook_to = (m.to_col > m.from_col) ? 5 : 3;
        next.sq[rook_to][m.from_row] = next.sq[rook_from][m.from_row];
        next.sq[rook_from][m.from_row] = nullptr;
    }

    next.sq[m.to_col][m.to_row] = piece;
    next.sq[m.from_col][m.from_row] = nullptr;
    return next;
}

bool Board::is_legal(const Move& m) const {
    if (!follows_rules(m)) return false;
    Piece* captured;
    return !king_in_check(after(m, &captured), grid.at(m.from_col, m.from_row)->isWhite());
}

bool Board::has_legal_move(bool white) const {
    for (int fc = 0; fc < 8; fc++) {
        for (int fr = 0; fr < 8; fr++) {
            Piece* p = grid.at(fc, fr);
            if (p == nullptr || p->isWhite() != white) continue;

            for (int tc = 0; tc < 8; tc++) {
                for (int tr = 0; tr < 8; tr++) {
                    if (tc == fc && tr == fr) continue;
                    if (is_legal(Move{ fc, fr, tc, tr, 0 })) return true;
                }
            }
        }
    }
    return false;
}


// ------- Playing a move ---------
bool Board::move(const string& text, int turn) {

    if (game_over) return false;

    Move m;
    if (!parse(text, m)) {
        cout << "Moves look like e2e4, or e7e8Q for a promotion." << endl;
        return false;
    }

    // get moving piece
    Piece* piece = grid.at(m.from_col, m.from_row);
    if (piece == nullptr) {
        cout << "No piece at origin." << endl;
        return false;
    }
    if (piece->isWhite() && turn == 1) {
        cout << "Black trying to move white piece" << endl;
        return false;
    }
    if (!piece->isWhite() && turn == 0) {
        cout << "White trying to move black piece" << endl;
        return false;
    }
    if (m.from_col == m.to_col && m.from_row == m.to_row) {
        cout << "The piece has to move." << endl;
        return false;
    }

    bool white = piece->isWhite();
    bool promoting = piece->kind() == 'P' && m.to_row == (white ? 7 : 0);
    if (m.promotion != 0 && !promoting) {
        cout << "Only a pawn reaching the last rank can promote." << endl;
        return false;
    }

    if (!follows_rules(m)) {
        if (is_castling(m)) cout << "Castling isn't allowed here." << endl;
        else cout << "That piece can't move there." << endl;
        return false;
    }
    if (!is_legal(m)) {
        cout << (white ? "White" : "Black") << " king would be in check!" << endl;
        return false;
    }
    if (promoting && string("QRBN").find(m.promotion) == string::npos) {
        cout << "Pawn promotion: add Q, R, B or N, for example " << text.substr(0, 4) << "Q" << endl;
        return false;
    }

    // perform the move
    bool castling = is_castling(m);
    Piece* captured = nullptr;
    grid = after(m, &captured);
    delete captured;

    piece->set_has_moved(true);
    if (castling) {
        grid.at((m.to_col > m.from_col) ? 5 : 3, m.to_row)->set_has_moved(true);
    }
    en_passant_col = (piece->kind() == 'P' && abs(m.to_row - m.from_row) == 2) ? m.to_col : -1;

    if (promoting) {
        Piece* promoted;
        switch (m.promotion) {
            case 'R': promoted = new Rook(white); break;
            case 'B': promoted = new Bishop(white); break;
            case 'N': promoted = new Knight(white); break;
            default:  promoted = new Queen(white); break;
        }
        promoted->set_has_moved(true);
        delete piece;
        grid.sq[m.to_col][m.to_row] = promoted;
    }

    check_game_state(turn);
    return true;
}

void Board::check_game_state(int turn) {
    // after `turn` moved, look at the opponent's position
    bool opponent_white = (turn == 1);
    string name = opponent_white ? "White" : "Black";

    in_check = king_in_check(grid, opponent_white);
    bool can_move = has_legal_move(opponent_white);

    if (in_check && !can_move) {
        game_over = true;
        game_result = "Checkmate! " + string(turn == 0 ? "White" : "Black") + " wins.";
    }
    else if (!can_move) {
        game_over = true;
        game_result = "Stalemate! " + name + " has no legal moves. It's a draw.";
    }
    else if (in_check) {
        cout << name << " king is in check!" << endl;
    }
}
