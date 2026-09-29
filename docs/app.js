// The page only draws and forwards clicks. Every rule is decided by the C++ engine
// (chess.wasm): which moves are legal, check, checkmate, stalemate, castling, en passant.

const FILES = "abcdefgh";
const NAMES = { K: "king", Q: "queen", R: "rook", B: "bishop", N: "knight", P: "pawn" };

const boardEl = document.getElementById("board");
const turnEl = document.getElementById("turn");
const messageEl = document.getElementById("message");
const movesEl = document.getElementById("moves");
const promotionEl = document.getElementById("promotion");
const typedForm = document.getElementById("typed");

let engine;          // { newGame, play, state }
let state;           // latest state from the engine
let selected = null; // square name like "e2"
let history = [];    // moves played, like "e2e4"
let pendingPromotion = null;

// board string index: a8 first, h1 last
const indexOf = (sq) => (8 - Number(sq[1])) * 8 + FILES.indexOf(sq[0]);
const pieceOn = (sq) => state.board[indexOf(sq)];
const isWhite = (p) => p !== "." && p === p.toUpperCase();
const ownPiece = (sq) => {
  const p = pieceOn(sq);
  return p !== "." && isWhite(p) === (state.turn === "w");
};
const targetsFrom = (sq) =>
  state.moves ? state.moves.split(" ").filter((m) => m.startsWith(sq)).map((m) => m.slice(2, 4)) : [];

function buildBoard() {
  document.querySelector(".ranks").innerHTML = [8, 7, 6, 5, 4, 3, 2, 1].map((r) => `<span>${r}</span>`).join("");
  document.querySelector(".files").innerHTML = [...FILES].map((f) => `<span>${f}</span>`).join("");

  for (let r = 8; r >= 1; r--) {
    for (const f of FILES) {
      const sq = f + r;
      const btn = document.createElement("button");
      btn.type = "button";
      btn.className = "sq" + ((FILES.indexOf(f) + r) % 2 === 1 ? " light" : "");
      btn.dataset.sq = sq;
      btn.addEventListener("click", () => onSquare(sq));
      boardEl.append(btn);
    }
  }
}

function render() {
  const targets = selected ? targetsFrom(selected) : [];
  const last = history.at(-1);
  const kingInCheck = state.check ? (state.turn === "w" ? "K" : "k") : null;

  for (const btn of boardEl.children) {
    const sq = btn.dataset.sq;
    const p = pieceOn(sq);
    btn.classList.toggle("own", !state.over && ownPiece(sq));
    btn.classList.toggle("selected", sq === selected);
    btn.classList.toggle("target", targets.includes(sq));
    btn.classList.toggle("capture", targets.includes(sq) && p !== ".");
    btn.classList.toggle("last", !!last && (last.slice(0, 2) === sq || last.slice(2, 4) === sq));
    btn.classList.toggle("check", p === kingInCheck);

    const label = p === "." ? sq : `${sq}, ${isWhite(p) ? "white" : "black"} ${NAMES[p.toUpperCase()]}`;
    btn.setAttribute("aria-label", label);
    btn.innerHTML = p === "." ? "" : `<span class="piece ${isWhite(p) ? "w" : "b"}">${p}</span>`;
  }

  const side = state.turn === "w" ? "White" : "Black";
  turnEl.classList.toggle("over", state.over);
  turnEl.querySelector(".swatch").className = "swatch" + (state.turn === "b" ? " b" : "");
  turnEl.querySelector(".turn-text").textContent = state.over ? state.result : `${side} to move`;

  movesEl.innerHTML = history.length
    ? history
        .reduce((rows, m, i) => (i % 2 ? rows[rows.length - 1].push(m) : rows.push([m]), rows), [])
        .map((row, i) => `<li><span class="n">${i + 1}.</span><span>${row[0]}</span><span>${row[1] ?? ""}</span></li>`)
        .join("")
    : `<li class="empty">No moves yet. White starts.</li>`;
  movesEl.scrollTop = movesEl.scrollHeight;
}

function say(text, warn = false) {
  messageEl.textContent = text;
  messageEl.classList.toggle("warn", warn);
}

// Sends a move to the engine. Returns whether it was played.
function play(move) {
  const ok = engine.play(move) === 1;
  state = engine.state();
  if (ok) {
    history.push(move);
    say(state.message);            // e.g. "Black king is in check!"
  } else {
    say(state.message || "Illegal move.", true);
  }
  selected = null;
  render();
  return ok;
}

function onSquare(sq) {
  if (state.over || pendingPromotion) return;

  if (selected && targetsFrom(selected).includes(sq)) {
    const move = selected + sq;
    const p = pieceOn(selected).toUpperCase();
    if (p === "P" && (sq[1] === "8" || sq[1] === "1")) {
      pendingPromotion = move;
      promotionEl.hidden = false;
      promotionEl.querySelector("button").focus();
      return;
    }
    play(move);
    return;
  }

  selected = ownPiece(sq) && sq !== selected ? sq : null;
  if (selected && targetsFrom(selected).length === 0) say(`That ${NAMES[pieceOn(sq).toUpperCase()]} has no legal moves.`);
  else say("");
  render();
}

promotionEl.addEventListener("click", (e) => {
  const btn = e.target.closest("button");
  if (!btn) return;
  const move = pendingPromotion;
  pendingPromotion = null;
  promotionEl.hidden = true;
  if (btn.dataset.piece) play(move + btn.dataset.piece);
  else { selected = null; render(); }
});

typedForm.addEventListener("submit", (e) => {
  e.preventDefault();
  const input = typedForm.elements.move;
  const move = input.value.trim();
  if (!move || state.over) return;
  if (play(move)) input.value = "";
});

document.getElementById("new-game").addEventListener("click", () => {
  engine.newGame();
  state = engine.state();
  history = [];
  selected = null;
  pendingPromotion = null;
  promotionEl.hidden = true;
  say("");
  render();
});

// Welcome screen: like the console's "Press any key to start...", any key or a tap closes it.
const welcomeEl = document.getElementById("welcome");
function closeWelcome() {
  if (welcomeEl.classList.contains("leaving")) return;
  welcomeEl.classList.add("leaving");
  document.removeEventListener("keydown", onWelcomeKey);
  setTimeout(() => { welcomeEl.hidden = true; }, 200);
}
function onWelcomeKey(e) {
  if (["Shift", "Control", "Alt", "Meta"].includes(e.key)) return;
  e.preventDefault();
  closeWelcome();
}
document.addEventListener("keydown", onWelcomeKey);
welcomeEl.addEventListener("click", closeWelcome);

createChess().then((module) => {
  const stateJson = module.cwrap("state", "string", []);
  engine = {
    newGame: module.cwrap("new_game", null, []),
    play: module.cwrap("play", "number", ["string"]),
    state: () => JSON.parse(stateJson()),
  };
  engine.newGame();
  state = engine.state();
  buildBoard();
  render();
});
