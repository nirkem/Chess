// The chess engine, exposed to the web page. Built with Emscripten (see web/build.bat)
// into docs/chess.js + docs/chess.wasm, which docs/app.js loads.

#include "../headers/Board.h"
#include <emscripten/emscripten.h>
#include <string>

static Board* board = nullptr;
static int turn = 0; // 0 = white, 1 = black
static std::string json;

static std::string quoted(const std::string& s) {
    std::string out = "\"";
    for (char c : s) {
        if (c == '"' || c == '\\') out += '\\';
        out += c;
    }
    return out + "\"";
}

extern "C" {

EMSCRIPTEN_KEEPALIVE void new_game() {
    delete board;
    board = new Board();
    turn = 0;
}

// A move in the same format as the console game: "e2e4", or "e7e8Q" for a promotion.
EMSCRIPTEN_KEEPALIVE int play(const char* move) {
    bool ok = board->move(move, turn);
    if (ok) turn = 1 - turn;
    return ok;
}

// {"board": 64 characters from a8 to h1 ('.' = empty), "turn": "w" or "b", "check", "over",
//  "result", "message" (from the last play), "moves": the side to move's legal moves}
EMSCRIPTEN_KEEPALIVE const char* state() {
    std::string squares;
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) squares += board->piece_at(col, row);
    }

    json = "{\"board\":" + quoted(squares) +
           ",\"turn\":" + (turn == 0 ? "\"w\"" : "\"b\"") +
           ",\"check\":" + (board->is_in_check() ? "true" : "false") +
           ",\"over\":" + (board->is_game_over() ? "true" : "false") +
           ",\"result\":" + quoted(board->get_game_result()) +
           ",\"message\":" + quoted(board->get_message()) +
           ",\"moves\":" + quoted(board->legal_moves(turn == 0)) + "}";
    return json.c_str();
}

}
