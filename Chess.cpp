#include <iostream>
#include <vector>
#include <string>
#include "headers/Board.h"
#include <conio.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

using namespace std;

int main() {
    // the board is drawn with Unicode block characters
    SetConsoleOutputCP(CP_UTF8);

    cout << "Welcome to Nir's Chess game!" << endl << endl;
    cout << "In this game, we perform moves by specifying the origin square and the destination square." << endl;
    cout << "For example, to move Pawn d2 to square d4, you need to enter: d2d4 in the console." << endl << endl;
    cout << "In case of a pawn promotion, enter the move followed by the symbol of the new piece." << endl;
    cout << "For example, to move Pawn b7 to b8 and promote to queen, you need to enter: b7b8Q in the console" << endl << endl;
    cout << "To castle, move the king two squares toward the rook, for example: e1g1 or e1c1." << endl;
    cout << "En passant is played like any other pawn capture, for example: e5d6." << endl << endl;
    cout << "On the board, white pieces are represented by capital letters and black by lowercase letters." << endl;
    cout << "Remember that white always starts the game." << endl << endl;

    cout << "Have Fun!" << endl;
    cout << "Press any key to start..." << endl;
    _getch();

    // display board
    Board* board = new Board();
    board->display_board();
    bool playing{true};
    bool move_succeeded;

    string move;
    int turn{0};

    while (playing) {
        cout << (turn == 0 ? "White" : "Black") << ", please enter a move: ";
        if (!(cin >> move)) break; // input closed
        move_succeeded = board->move(move, turn);
        if (!board->get_message().empty()) cout << board->get_message() << endl;

        if (move_succeeded) {
            board->display_board();
            turn = (turn == 0) ? 1 : 0;

            if (board->is_game_over()) {
                cout << board->get_game_result() << endl;
                playing = false;
            }
        } else {
            cout << "Illegal Move" << endl;
            continue;
        }

    }

    delete(board);
    return 0;
}