@echo off
rem Builds the web version: docs\chess.js + docs\chess.wasm (served by GitHub Pages from docs\).
rem Needs Emscripten on the PATH. Run from the repo root, e.g.:
rem   C:\Users\nirke\emsdk\emsdk_env.bat
rem   web\build.bat

em++ -O2 -std=c++17 -I. ^
  web/bindings.cpp sources/Board.cpp sources/Pawn.cpp sources/Bishop.cpp ^
  sources/Knight.cpp sources/Rook.cpp sources/King.cpp sources/Queen.cpp ^
  -o docs/chess.js ^
  -sMODULARIZE -sEXPORT_NAME=createChess -sENVIRONMENT=web ^
  -sEXPORTED_FUNCTIONS=_new_game,_play,_state ^
  -sEXPORTED_RUNTIME_METHODS=cwrap
