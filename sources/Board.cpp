#include "../headers/Board.h"
#include "../headers/Pawn.h"
#include "../headers/Rook.h"
#include "../headers/Knight.h"
#include "../headers/Queen.h"
#include "../headers/Bishop.h"
#include "../headers/King.h"
#include <iostream>
#include <cctype>

using namespace std;


// ------- Ctor Dtor ---------
Board::Board() {
    legal_col_row = { 1, 4, 7, 10, 13, 16, 19, 22 };
    place_pieces();
}

Board::~Board() {
    for (Piece* p : pieces) {
        delete p;
    }
}

void Board::delete_piece(Piece& piece) {
    // Get pointer to the object
    Piece* ptr = &piece;

    // Delete the object
    delete ptr;

    // Remove all pointers to this object from all vectors
    auto remove_ptr = [ptr](std::vector<Piece*>& vec) {
        vec.erase(std::remove(vec.begin(), vec.end(), ptr), vec.end());
        };

    remove_ptr(pieces);
    remove_ptr(white_pieces);
    remove_ptr(white_Pawns);
    remove_ptr(black_pieces);
    remove_ptr(black_Pawns);
}

char convert_to_char(int num) {
    num--;
    num += 'a';
    return char(num);
}

int convert_to_int(char c) {
    int num = int(c) - 96;
    return num;
}

bool is_legit_string(string s) {

    if (s.size() != 4 && s.size() != 5) return false;
    if (s[1] > '8' || s[1] < '1' || s[3] > '8' || s[3] < '1') return false;

    int first_char = int(s[0]);
    int second_char = int(s[2]);

    if (first_char < 97 || first_char > 104 || second_char < 97 || second_char > 104) return false;

    return true;
}

int* convert_string_to_col_row(string move) {
    int origin_col = convert_to_int(move[0]);
    int origin_row = move[1] - '0';
    int dest_col = convert_to_int(move[2]);
    int dest_row = move[3] - '0';

    int* ans = new int[5]();

    ans[0] = origin_col;
    ans[1] = origin_row;
    ans[2] = dest_col;
    ans[3] = dest_row;
    if (move.size() > 4) ans[4] = int(move[4]);

    return ans;
}



void Board::display_board() {
    std::cout << std::endl << std::endl;
    const char white_square = ' ';      // white block
    const char black_square = char(219); // black block

    bool white = true;
    int col_counter = 0;
    int row_counter = 0;

    for (int row = 0; row < 24; row++) {
        for (int col = 0; col < 24; col++) {


            if (col_counter == 3) {
                white = !white;
                col_counter = 0;
            }
            if (brd[col][row] != nullptr) {
                // Print the piece
                cout << (brd[col][row])->get_symbol() << " ";
                col_counter++;
            }
            else {
                if (white) {
                    cout << white_square << white_square; // white square
                    col_counter++;
                }
                else {
                    cout << black_square << black_square; // black square
                    col_counter++;
                }
            }

        }

        if (std::find(legal_col_row.begin(), legal_col_row.end(), row) != legal_col_row.end()) cout << " | " << 8 - (row - 1) / 3;
        // else cout << " |   ";
        cout << endl;
        row_counter++;
        if (row_counter == 3) {
            white = !white;
            row_counter = 0;
        }
    }

    cout << "  ";
    for (int i = 1; i < 24; i++) {
        if (std::find(legal_col_row.begin(), legal_col_row.end(), i) != legal_col_row.end()) cout << "_" << " ";
        else cout << "  ";
    }

    cout << endl;
    cout << "  ";
    for (int i = 1; i < 24; i++) {
        char c = convert_to_char((i - 1) / 3 + 1);
        if (std::find(legal_col_row.begin(), legal_col_row.end(), i) != legal_col_row.end()) cout << c << " ";
        else cout << "  ";
    }
    cout << endl;
}

bool Board::move(string move, int turn) {

    // check if legal move
    if (game_over) return false;
    if (!is_legit_string(move)) {
        cout << "Moves look like e2e4, or e7e8Q for a promotion." << endl;
        return false;
    }

    // convert move to a list of ints representing the move
    int* ans = convert_string_to_col_row(move);

    // files a-h go left to right, rank 8 is at the top of the board
    int origin_col = 3 * (ans[0] - 1) + 1;
    int origin_row = 3 * (8 - ans[1]) + 1;
    int dst_col = 3 * (ans[2] - 1) + 1;
    int dst_row = 3 * (8 - ans[3]) + 1;
    char new_piece = char(toupper(ans[4]));

    delete[] ans;
    ans = nullptr;

    // get moving piece
    Piece* piece = brd[origin_col][origin_row];
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
    if (origin_col == dst_col && origin_row == dst_row) {
        cout << "The piece has to move." << endl;
        return false;
    }

    // a pawn reaching the last rank must promote
    char pawn_symbol = piece->isWhite() ? 'P' : 'p';
    int last_row = piece->isWhite() ? 1 : 22;
    bool promoting = piece->get_symbol() == pawn_symbol && dst_row == last_row;

    if (new_piece != 0 && !promoting) {
        cout << "Only a pawn reaching the last rank can promote." << endl;
        return false;
    }
    if (promoting && (new_piece == 0 || string("QRBN").find(new_piece) == string::npos)) {
        if (try_move(piece, dst_col, dst_row, turn, false))
            cout << "Pawn promotion: add Q, R, B or N, for example " << move.substr(0, 4) << "Q" << endl;
        return false;
    }

    // perform the move (reverted inside if it leaves our own king in check)
    if (!try_move(piece, dst_col, dst_row, turn, true)) return false;

    if (promoting) {
        bool white = piece->isWhite();
        Piece* promoted;
        switch (new_piece) {
            case 'R': promoted = new Rook(dst_col, dst_row, white); break;
            case 'B': promoted = new Bishop(dst_col, dst_row, white); break;
            case 'N': promoted = new Knight(dst_col, dst_row, white); break;
            default:  promoted = new Queen(dst_col, dst_row, white); break;
        }
        promoted->set_has_moved(true);
        brd[dst_col][dst_row] = promoted;
        delete_piece(*piece);
        add_piece(promoted);
    }

    checkForCheck(turn);
    return true;
}

// Moves the piece and keeps the move only if it doesn't leave its own king in check.
// With keep == false the move is always undone, so this only answers "is it legal?".
bool Board::try_move(Piece* piece, int dst_col, int dst_row, int turn, bool keep) {
    int origin_col = piece->get_col();
    int origin_row = piece->get_row();
    bool had_moved = piece->getHasMoved();
    Piece* captured = brd[dst_col][dst_row];

    if (!piece->move(dst_col, dst_row, turn, brd)) return false;

    brd[origin_col][origin_row] = nullptr;
    brd[dst_col][dst_row] = piece;

    King* own_king = piece->isWhite() ? white_king : black_king;
    bool legal = !own_king->isInCheck(brd);
    if (!legal && keep) {
        cout << (piece->isWhite() ? "White" : "Black") << " king would be in check!" << endl;
    }

    if (!legal || !keep) {
        // revert move: board squares and the piece's own position
        brd[origin_col][origin_row] = piece;
        brd[dst_col][dst_row] = captured;
        piece->set_col(origin_col);
        piece->set_row(origin_row);
        piece->set_has_moved(had_moved);
        return legal;
    }

    if (captured != nullptr) delete_piece(*captured);
    return true;
}

bool Board::has_legal_move(int turn) {
    // try every square for every piece, with the pieces' "invalid move" messages muted
    vector<Piece*> own = (turn == 0) ? white_pieces : black_pieces;
    streambuf* old_buf = cout.rdbuf(nullptr);
    bool found = false;

    for (Piece* p : own) {
        for (int c : legal_col_row) {
            for (int r : legal_col_row) {
                if (c == p->get_col() && r == p->get_row()) continue;
                if (try_move(p, c, r, turn, false)) {
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        if (found) break;
    }

    cout.rdbuf(old_buf);
    return found;
}

void Board::add_piece(Piece* piece) {
    pieces.push_back(piece);
    if (piece->isWhite()) {
        white_pieces.push_back(piece);
        if (piece->get_symbol() == 'P')
            white_Pawns.push_back(piece);
    }
    else {
        black_pieces.push_back(piece);
        if (piece->get_symbol() == 'p')
            black_Pawns.push_back(piece);
    }
}



// ------- Creating pieces ---------
void Board::create_pawns() {
    // Create black Pawns
    for (int col = 0; col < 8; col++) {
        brd[3 * col + 1][4] = new Pawn(3 * col + 1, 4, false);
        pieces.push_back(brd[(3 * col) + 1][4]);
    }

    // Create white Pawns
    for (int col = 0; col < 8; col++) {
        brd[3 * col + 1][19] = new Pawn(3 * col + 1, 19, true);
        pieces.push_back(brd[3 * col + 1][19]);
    }
}

void Board::create_rooks() {
    // Create Rook 
    brd[1][1] = new Rook(1, 1, false);
    brd[22][1] = new Rook(22, 1, false);
    brd[1][22] = new Rook(1, 22, true);
    brd[22][22] = new Rook(22, 22, true);
    pieces.push_back(brd[1][1]);
    pieces.push_back(brd[1][22]);
    pieces.push_back(brd[22][1]);
    pieces.push_back(brd[22][22]);
}

void Board::create_bishops() {
    // Create Bishop
    brd[7][1] = new Bishop(7, 1, false); brd[16][1] = new Bishop(16, 1, false);
    brd[7][22] = new Bishop(7, 22, true); brd[16][22] = new Bishop(16, 22, true);
    pieces.push_back(brd[7][1]);
    pieces.push_back(brd[16][1]);
    pieces.push_back(brd[7][22]);
    pieces.push_back(brd[16][22]);
}

void Board::create_knights() {
    // Create Knight
    brd[4][1] = new Knight(4, 1, false); brd[19][1] = new Knight(19, 1, false);
    brd[4][22] = new Knight(4, 22, true); brd[19][22] = new Knight(19, 22, true);
    pieces.push_back(brd[4][1]);
    pieces.push_back(brd[19][1]);
    pieces.push_back(brd[4][22]);
    pieces.push_back(brd[19][22]);
}

void Board::create_royalty() {
    // Create Queen
    brd[10][1] = new Queen(10, 1, false);
    brd[10][22] = new Queen(10, 22, true);
    pieces.push_back(brd[10][1]);
    pieces.push_back(brd[10][22]);

    // Create King
    white_king = new King(13, 22, true);
    black_king = new King(13, 1, false);
    brd[13][1] = black_king;
    brd[13][22] = white_king;

    pieces.push_back(black_king);
    pieces.push_back(white_king);
}

void Board::place_pieces() {
    create_pawns();
    create_rooks();
    create_bishops();
    create_knights();
    create_royalty();

    // keep vector of black and white pieces
    vector<Piece*> created = pieces;
    pieces.clear();
    for (Piece* piece : created) add_piece(piece);
}

void Board::checkForCheck(int turn) {
    // after `turn` moved, look at the opponent's position
    int opponent = (turn == 0) ? 1 : 0;
    King* king = (opponent == 0) ? white_king : black_king;
    string name = (opponent == 0) ? "White" : "Black";

    in_check = king->isInCheck(brd);
    bool can_move = has_legal_move(opponent);

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
