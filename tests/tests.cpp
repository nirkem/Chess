// Rule tests for the chess engine. Each test plays real moves on a fresh Board.
//
// Build and run from the repo root:
//   g++ -I. tests/tests.cpp sources/*.cpp -o tests/tests.exe && tests/tests.exe
// (or run the "test" task in VS Code)

#include "headers/Board.h"
#include <iostream>

static int failures = 0;

// A board plus whose turn it is. Moves alternate white/black, starting with white,
// and the game's own messages are muted so only the test results are printed.
struct Game {
    Board b;
    int turn = 0;
    bool play(const std::string& m) {
        std::streambuf* old = std::cout.rdbuf(nullptr);
        bool ok = b.move(m, turn);
        std::cout.rdbuf(old);
        if (ok) turn = 1 - turn;
        return ok;
    }
};

void expect(const char* name, bool cond) {
    std::cout << (cond ? "PASS  " : "FAIL  ") << name << "\n";
    if (!cond) failures++;
}

// Plays setup moves that must all be legal.
bool all(Game& g, std::initializer_list<const char*> moves) {
    for (auto m : moves) {
        if (!g.play(m)) {
            std::cout << "FAIL  setup move rejected: " << m << "\n";
            failures++;
            return false;
        }
    }
    return true;
}

int main() {
    std::cout << "-- Board setup and notation\n";
    { Game g; expect("d2d4 moves white's pawn, e7e5 moves black's", g.play("d2d4") && g.play("e7e5")); }
    { Game g; expect("white can't move black's pieces", !g.play("e7e5")); }
    { Game g; expect("queen starts on d1 (Qd1-h5 after e4)", all(g, {"e2e4", "a7a6", "d1h5"})); }

    std::cout << "-- Pawns\n";
    { Game g; all(g, {"a2a3", "a7a6"});
      expect("pawn can't double-step after it moved", !g.play("a3a5")); }
    { Game g; expect("first double-step works", g.play("a2a4")); }

    std::cout << "-- Captures\n";
    { Game g; bool ok = all(g, {"e2e3", "a7a6", "f1c4", "d7d5", "c4d5", "a6a5"});
      expect("bishop captures on d5, then moves on from d5", ok && g.play("d5c6")); }

    std::cout << "-- Check\n";
    { Game g; all(g, {"e2e4", "d7d5", "f1b5"});
      expect("black in check can't ignore it", !g.play("h7h6"));
      expect("a capture that doesn't answer check is rejected", !g.play("d5e4"));
      expect("board is intact after the rejected move: c7c6 blocks", g.play("c7c6"));
      expect("the d5 pawn is still there and can capture e4", g.play("a2a3") && g.play("d5e4")); }
    { Game g; all(g, {"e2e4", "e7e5", "d1h5", "a7a6", "h5e5"});
      expect("check along a file from below is detected", !g.play("d7d6"));
      expect("blocking the check works", g.play("f8e7")); }
    { Game g; all(g, {"e2e4", "e7e5", "d1h5"});
      expect("pinned pawn (f7, pinned by Qh5) can't move", !g.play("f7f6"));
      expect("an unpinned move is fine", g.play("g7g6")); }
    { Game g; all(g, {"e2e4", "e7e5", "e1e2", "f8c5"});
      expect("king can't step onto a square the c5 bishop attacks", !g.play("e2e3"));
      expect("king can step to a safe square", g.play("e2d3")); }

    std::cout << "-- End of game\n";
    { Game g; all(g, {"e2e4", "e7e5", "d1h5", "b8c6", "f1c4", "g8f6", "h5f7"});
      expect("scholar's mate is checkmate, white wins", g.b.is_game_over() &&
             g.b.get_game_result().find("Checkmate! White wins") != std::string::npos); }
    { Game g; all(g, {"f2f3", "e7e5", "g2g4", "d8h4"});
      expect("fool's mate is checkmate, black wins", g.b.is_game_over() &&
             g.b.get_game_result().find("Checkmate! Black wins") != std::string::npos); }
    { Game g; all(g, {"e2e3", "a7a5", "d1h5", "a8a6", "h5a5", "h7h5", "h2h4", "a6h6", "a5c7", "f7f6",
                      "c7d7", "e8f7", "d7b7", "d8d3", "b7b8", "d3h7", "b8c8", "f7g6", "c8e6"});
      expect("Loyd's 10-move stalemate is a draw", g.b.is_game_over() &&
             g.b.get_game_result().find("Stalemate") != std::string::npos); }
    { Game g; all(g, {"e2e4", "e7e5"});
      expect("a normal position is not game over", !g.b.is_game_over()); }

    std::cout << "-- Promotion\n";
    { Game g; all(g, {"a2a4", "b7b5", "a4b5", "h7h6", "b5b6", "h6h5", "b6a7", "h5h4"});
      expect("promotion without a letter is rejected", !g.play("a7b8"));
      expect("...and doesn't pass the turn", g.turn == 0);
      expect("a7xb8=Q works", g.play("a7b8Q"));
      expect("the new queen moves like a queen (captures the a8 rook)", g.play("h4h3") && g.play("b8a8")); }
    { Game g;
      expect("promotion letter on a normal move is rejected", !g.play("e2e4Q"));
      expect("...and doesn't pass the turn", g.turn == 0 && g.play("e2e4")); }

    std::cout << "-- Input\n";
    { Game g;
      expect("short or long input is rejected", !g.play("e2") && !g.play("") && !g.play("e2e4e5e6"));
      expect("off-board input is rejected", !g.play("i2i4") && !g.play("e0e9")); }

    std::cout << (failures ? "\nFAILURES: " : "\nall passed") ;
    if (failures) std::cout << failures;
    std::cout << "\n";
    return failures;
}
