/
//// ============================================================================
//// Game.cpp  —  Game controller + SFML 3 graphical interface
//// COP 3003  |  Checkers Project  |  Team: Rose, Casado, Pearson, Bustamante
//// ============================================================================
//// NOTE: This file targets SFML 3.  Key API differences from SFML 2:
////   • pollEvent() returns std::optional<sf::Event> (no argument)
////   • Events checked via event->is<T>() / event->getIf<T>()
////   • Keyboard enums: sf::Keyboard::Key::Escape (not sf::Keyboard::Escape)
////   • Mouse enums:    sf::Mouse::Button::Left   (not sf::Mouse::Left)
////   • setPosition/setOrigin take sf::Vector2f {x, y}
////   • sf::Text constructor: sf::Text(font, string, size)
////   • sf::FloatRect members: .position and .size (not .left/.top/.width/.height)
//// ============================================================================
//#include <AIGame.h>
//#include <SFML/Graphics.hpp>
//#include <sstream>
//#include <algorithm>
//#include <stdexcept>
//
//// ════════════════════════════════════════════════════════════════════════════
////  Color palette  (file-local constants)
//// ════════════════════════════════════════════════════════════════════════════
//namespace {
//    const sf::Color CLR_LIGHT{ 240, 217, 181 };   // Cream  — light squares
//    const sf::Color CLR_DARK{ 181, 136,  99 };   // Brown  — dark squares
//    const sf::Color CLR_RED_P{ 200,  30,  30 };   // Red piece fill
//    const sf::Color CLR_RED_O{ 240,  90,  90 };   // Red piece outline
//    const sf::Color CLR_BLK_P{ 25,  25,  25 };   // Black piece fill
//    const sf::Color CLR_BLK_O{ 90,  90,  90 };   // Black piece outline
//    const sf::Color CLR_KING{ 255, 215,   0 };   // Gold — king ring
//    const sf::Color CLR_SEL{ 255, 255,   0, 160 }; // Selected square
//    const sf::Color CLR_STEP{ 0, 200,   0, 200 }; // Normal-move dot
//    const sf::Color CLR_JUMP{ 255, 120,   0, 230 }; // Capture-move dot
//    const sf::Color CLR_MUST{ 255, 220,   0, 180 }; // Must-capture border
//    const sf::Color CLR_SB_BG{ 35,  35,  35 };   // Sidebar background
//    const sf::Color CLR_SB_LINE{ 60,  60,  60 };   // Sidebar divider line
//    const sf::Color CLR_BTN_NEW{ 70, 130, 180 };   // New-game button
//    const sf::Color CLR_BTN_QUIT{ 150,  50,  50 };   // Quit button
//    const sf::Color CLR_WHITE{ 255, 255, 255 };
//    const sf::Color CLR_GRAY{ 170, 170, 170 };
//    const sf::Color CLR_OVERLAY{ 0,   0,   0, 180 }; // Game-over overlay
//}
//
//// ════════════════════════════════════════════════════════════════════════════
////  Constructor
//// ════════════════════════════════════════════════════════════════════════════
//Game::Game()
//    : m_currentPlayer(PieceColor::Red),
//    m_gameState(GameState::Playing),
//    m_selectState(SelectState::Idle),
//    // SFML 3: two-argument RenderWindow constructor (no style enum needed)
//    m_window(sf::VideoMode({ (unsigned)WIN_W, (unsigned)WIN_H }),
//        "Checkers  —  COP 3003")
//{
//    m_window.setFramerateLimit(60);
//
//     //Try common Windows font paths for the sidebar text
//     //m_fontLoaded = m_font.openFromFile("C:/Windows/Fonts/arial.ttf");
//    //if (!m_fontLoaded)
//    //    m_fontLoaded = m_font.openFromFile("C:/Windows/Fonts/segoeui.ttf");
//    //if (!m_fontLoaded)
//    //    m_fontLoaded = m_font.openFromFile("C:/Windows/Fonts/calibri.ttf");
//
//    //m_fontLoaded = m_font.openFromFile("arial.ttf");
//    //if (!m_fontLoaded)
//    //    m_fontLoaded = m_font.openFromFile("calibri.ttf");
//
//    m_fontLoaded = m_font.openFromFile("Assets/fonts/arial/arial.ttf");
//
//    startNewGame();
//}
//
//// ════════════════════════════════════════════════════════════════════════════
////  run()  —  Main SFML event loop
//// ════════════════════════════════════════════════════════════════════════════
//void Game::run() {
//    while (m_window.isOpen()) {
//
//        // ── SFML 3 event loop ─────────────────────────────────────────────────
//        // pollEvent() now returns std::optional<sf::Event>; loop while it has a value
//        while (const std::optional event = m_window.pollEvent()) {
//
//            // Window close button
//            if (event->is<sf::Event::Closed>()) {
//                m_window.close();
//
//                // Keyboard shortcuts
//            }
//            else if (const auto* kp = event->getIf<sf::Event::KeyPressed>()) {
//                if (kp->code == sf::Keyboard::Key::Escape)
//                    m_window.close();
//                if (kp->code == sf::Keyboard::Key::N)
//                    startNewGame();
//
//                // Mouse click
//            }
//            else if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
//                if (mb->button == sf::Mouse::Button::Left) {
//                    int mx = mb->position.x;   // SFML 3: position is sf::Vector2i
//                    int my = mb->position.y;
//
//                    // ── Board click ───────────────────────────────────────────
//                    int row, col;
//                    if (m_gameState == GameState::Playing && pixelToGrid(mx, my, row, col))
//                        onSquareClick(row, col);
//
//                    // ── Sidebar button clicks ─────────────────────────────────
//                    // New Game button
//                    if (mx >= BTN_X && mx <= BTN_X + BTN_W &&
//                        my >= BTN_NEW_Y && my <= BTN_NEW_Y + BTN_H)
//                        startNewGame();
//
//                    // Quit button
//                    if (mx >= BTN_X && mx <= BTN_X + BTN_W &&
//                        my >= BTN_QUIT_Y && my <= BTN_QUIT_Y + BTN_H)
//                        m_window.close();
//                }
//            }
//        }
//
//        draw();
//    }
//}
//
//// ════════════════════════════════════════════════════════════════════════════
////  Game Logic
//// ════════════════════════════════════════════════════════════════════════════
//
//void Game::startNewGame() {
//    m_board.initBoard();
//    m_currentPlayer = PieceColor::Red;
//    m_gameState = GameState::Playing;
//    m_selectState = SelectState::Idle;
//    m_selRow = m_selCol = m_jumpRow = m_jumpCol = -1;
//    m_validMoves.clear();
//    m_mustCapture.clear();
//    m_history.clear();
//    m_statusMsg = "Red's turn  (Player 1)";
//    computeMustCapture(); // EEE this is unnecessary, shouldnt ever occur logically
//}
//
//void Game::computeMustCapture() {
//    m_mustCapture = m_board.getPiecesWithJumps(m_currentPlayer);
//}
//
//// ── onSquareClick ─────────────────────────────────────────────────────────────
//// Implements the three-state selection machine:
////   ChainJumping → only the jumping piece's next jump accepted
////   PieceSelected → move to highlighted square, or re-select another piece
////   Idle → select a piece
//void Game::onSquareClick(int row, int col) {
//
//    // ── ChainJumping: locked-in piece must continue ────────────────────────────
//    if (m_selectState == SelectState::ChainJumping) {
//        for (const Move& mv : m_validMoves) {
//            if (mv.toRow == row && mv.toCol == col) {
//                executeMove(mv);
//                //return;
//            }
//        }
//        m_statusMsg = "Must keep jumping this piece!";
//        //return;
//    }
//
//    Piece* clicked = m_board.getPiece(row, col);
//
//    // ── PieceSelected: try destination or re-select ────────────────────────────
//    if (m_selectState == SelectState::PieceSelected) {
//        for (const Move& mv : m_validMoves) {
//            if (mv.toRow == row && mv.toCol == col) {
//                executeMove(mv);
//                return;
//            }
//        }
//        if (clicked && clicked->getColor() == m_currentPlayer) {
//            selectPiece(row, col);
//            return;
//        }
//        // Clicked on empty or enemy square — deselect
//        m_selRow = m_selCol = -1;
//        m_validMoves.clear();
//        m_selectState = SelectState::Idle;
//        m_statusMsg = (m_currentPlayer == PieceColor::Red)
//            ? "Red's turn  (Player 1)"
//            : "Black's turn  (Player 2)";
//        return;
//    }
//
//    // ── Idle: pick a piece ────────────────────────────────────────────────────
//    //if (!clicked) return;
//    if (clicked->getColor() != m_currentPlayer) {
//        m_statusMsg = "That's not your piece!";
//        return;
//    }
//    selectPiece(row, col);
//}
//
//// ── selectPiece ───────────────────────────────────────────────────────────────
//void Game::selectPiece(int row, int col) {
//    Piece* p = m_board.getPiece(row, col);
//    if (!p || p->getColor() != m_currentPlayer) return;
//
//    // Mandatory-capture: only jumper pieces may be selected
//    if (!m_mustCapture.empty()) {
//        auto it = std::find_if(m_mustCapture.begin(), m_mustCapture.end(),
//            [row, col](const std::pair<int, int>& pr) {
//                return pr.first == row && pr.second == col;
//            });
//        if (it == m_mustCapture.end()) {
//            m_statusMsg = "Must capture!  Select a highlighted piece.";
//            return;
//        }
//    }
//
//    std::vector<Move> all = p->getValidMoves(m_board);
//    m_validMoves = m_mustCapture.empty() ? all : jumpsOnly(all);
//
//    if (m_validMoves.empty()) {
//        m_statusMsg = "That piece has no legal moves.";
//        return;
//    }
//
//    m_selRow = row;
//    m_selCol = col;
//    m_selectState = SelectState::PieceSelected;
//    m_statusMsg = "Click a highlighted square to move.";
//}
//
//// ── executeMove ───────────────────────────────────────────────────────────────
//void Game::executeMove(const Move& mv) {
//    PieceColor mover = m_currentPlayer;   // Capture before switchTurn() can change it
//
//    try {
//        m_board.applyMove(mv);
//    }
//    catch (const std::exception& e) {
//        m_statusMsg = std::string("Error: ") + e.what();
//        return;
//    }
//
//    // EEE why the fuck would you use wasJump here and not just mv.isJump??
//    bool wasJump = mv.isJump;
//    bool promoted = false;
//
//    // ── Promotion check ────────────────────────────────────────────────────────
//    // Red promotes at row 7 (bottom); Black promotes at row 0 (top).
//    {
//        Piece* p = m_board.getPiece(mv.toRow, mv.toCol);
//        if (p && !p->getIsKing()) {
//            bool redProm = (p->getColor() == PieceColor::Red && mv.toRow == 7);
//            bool blackProm = (p->getColor() == PieceColor::Black && mv.toRow == 0);
//            if (redProm || blackProm) {
//                m_board.promoteAt(mv.toRow, mv.toCol);
//                promoted = true;
//                // Standard rule: promotion ends the turn (no more jumps)
//                wasJump = false;
//            }
//        }
//    }
//
//    logMove(mv, promoted, mover);
//    afterMoveChecks(mv.toRow, mv.toCol, wasJump);
//}
//
//// ── afterMoveChecks ───────────────────────────────────────────────────────────
//
//// EEE this should be able to be absorbed into executeMove?
//void Game::afterMoveChecks(int toRow, int toCol, bool wasJump) {
//    // Check for multi-jump continuation
//    if (wasJump) {
//        Piece* jumper = m_board.getPiece(toRow, toCol);
//        if (jumper) {
//            std::vector<Move> more = jumpsOnly(jumper->getValidMoves(m_board));
//            if (!more.empty()) {
//                m_selectState = SelectState::ChainJumping;
//                m_selRow = toRow;
//                m_selCol = toCol;
//                m_jumpRow = toRow;
//                m_jumpCol = toCol;
//                m_validMoves = more;
//                m_statusMsg = "Keep jumping!";
//                return;   // Don't switch turns yet
//            }
//        }
//    }
//
//    switchTurn();
//}
//
//// ── switchTurn ────────────────────────────────────────────────────────────────
//void Game::switchTurn() {
//    m_currentPlayer = (m_currentPlayer == PieceColor::Red)
//        ? PieceColor::Black : PieceColor::Red;
//
//    m_selectState = SelectState::Idle;
//    m_selRow = m_selCol = m_jumpRow = m_jumpCol = -1;
//    m_validMoves.clear();
//
//    if (checkWinCondition()) return;
//
//    computeMustCapture();
//
//    m_statusMsg = (m_currentPlayer == PieceColor::Red)
//        ? "Red's turn  (Player 1)"
//        : "Black's turn  (Player 2)";
//
//    //  EEEEEEEEEEEE if stuff doesnt work add this
//    if (!m_mustCapture.empty())
//        m_statusMsg += "  — MUST CAPTURE!";
//}
//
//// ── checkWinCondition ─────────────────────────────────────────────────────────
//bool Game::checkWinCondition() {
//    if (!m_board.playerHasMoves(m_currentPlayer)) {
//        m_gameState = GameState::GameOver;
//        std::string winner = (m_currentPlayer == PieceColor::Red) ? "Black" : "Red";
//        m_statusMsg = winner + " wins!";
//        return true;
//    }
//    return false;
//}
//
//// ── jumpsOnly ─────────────────────────────────────────────────────────────────
//std::vector<Move> Game::jumpsOnly(const std::vector<Move>& moves) {
//    std::vector<Move> result;
//    for (const Move& mv : moves)
//        if (mv.isJump) result.push_back(mv);
//    return result;
//}
//
//// ── logMove ───────────────────────────────────────────────────────────────────
//void Game::logMove(const Move& mv, bool promoted, PieceColor mover) {
//    std::ostringstream oss;
//    oss << (mover == PieceColor::Red ? "Red" : "Black");
//    oss << ": (" << mv.fromRow << ',' << mv.fromCol << ')'
//        << (mv.isJump ? " x " : " \xe2\x86\x92 ")   // UTF-8 arrow or "x" for jump
//        << '(' << mv.toRow << ',' << mv.toCol << ')';
//    if (promoted) oss << " [K]";
//    m_history.push_back(oss.str());
//}
//
//// ════════════════════════════════════════════════════════════════════════════
////  Rendering
//// ════════════════════════════════════════════════════════════════════════════
//
//void Game::draw() {
//    m_window.clear(sf::Color::Black);
//    drawBoard();
//    drawHighlights();
//    drawPieces();
//    drawSidebar();
//    if (m_gameState == GameState::GameOver)
//        drawGameOverOverlay();
//    m_window.display();
//}
//
//// ── drawBoard ─────────────────────────────────────────────────────────────────
//void Game::drawBoard() {
//    sf::RectangleShape sq(sf::Vector2f(CELL, CELL));
//    for (int r = 0; r < 8; r++) {
//        for (int c = 0; c < 8; c++) {
//            sq.setPosition({ (float)(c * CELL), (float)(r * CELL) });  // SFML 3 brace-init
//            sq.setFillColor((r + c) % 2 == 0 ? CLR_LIGHT : CLR_DARK);
//            m_window.draw(sq);
//        }
//    }
//}
//
//// ── drawHighlights ────────────────────────────────────────────────────────────
//void Game::drawHighlights() {
//    sf::RectangleShape sq(sf::Vector2f(CELL, CELL));
//    sf::RectangleShape border(sf::Vector2f(CELL - 6, CELL - 6));
//
//    // Must-capture borders (yellow outline on pieces that must jump)
//    if (m_selectState != SelectState::ChainJumping) {
//        border.setFillColor(sf::Color::Transparent);
//        border.setOutlineThickness(3.f);
//        border.setOutlineColor(CLR_MUST);
//        for (const auto& [r, c] : m_mustCapture) {
//            border.setPosition({ (float)(c * CELL + 3), (float)(r * CELL + 3) });
//            m_window.draw(border);
//        }
//    }
//
//    // Selected square highlight (yellow fill)
//    if (m_selRow >= 0) {
//        sq.setFillColor(CLR_SEL);
//        sq.setPosition({ (float)(m_selCol * CELL), (float)(m_selRow * CELL) });
//        m_window.draw(sq);
//    }
//
//    // Valid-move dots: orange for jumps, green for steps
//    const float DOT_R = 14.f;
//    sf::CircleShape dot(DOT_R);
//    dot.setOrigin({ DOT_R, DOT_R });   // SFML 3 brace-init
//    for (const Move& mv : m_validMoves) {
//        dot.setFillColor(mv.isJump ? CLR_JUMP : CLR_STEP);
//        dot.setPosition({ mv.toCol * CELL + CELL / 2.f,
//                          mv.toRow * CELL + CELL / 2.f });
//        m_window.draw(dot);
//    }
//}
//
//// ── drawPieces ────────────────────────────────────────────────────────────────
//void Game::drawPieces() {
//    const float PIECE_R = 30.f;
//    const float KING_R = 12.f;
//
//    sf::CircleShape outer(PIECE_R);
//    outer.setOrigin({ PIECE_R, PIECE_R });
//    outer.setOutlineThickness(3.f);
//
//    sf::CircleShape inner(KING_R);
//    inner.setOrigin({ KING_R, KING_R });
//    inner.setFillColor(CLR_KING);
//
//    for (int r = 0; r < 8; r++) {
//        for (int c = 0; c < 8; c++) {
//            Piece* p = m_board.getPiece(r, c);
//            if (!p) continue;
//
//            float cx = c * CELL + CELL / 2.f;
//            float cy = r * CELL + CELL / 2.f;
//            bool  red = (p->getColor() == PieceColor::Red);
//
//            outer.setFillColor(red ? CLR_RED_P : CLR_BLK_P);
//            outer.setOutlineColor(red ? CLR_RED_O : CLR_BLK_O);
//            outer.setPosition({ cx, cy });
//            m_window.draw(outer);
//
//            if (p->getIsKing()) {
//                inner.setPosition({ cx, cy });
//                m_window.draw(inner);
//            }
//        }
//    }
//}
//
//// ── drawSidebar ───────────────────────────────────────────────────────────────
//void Game::drawSidebar() {
//    // Background panel
//    sf::RectangleShape bg(sf::Vector2f((float)(WIN_W - SB_X), (float)WIN_H));
//    bg.setPosition({ (float)SB_X, 0.f });
//    bg.setFillColor(CLR_SB_BG);
//    m_window.draw(bg);
//
//    float x = (float)SB_X + 10.f;
//    float y = 10.f;
//    const float LINE = 22.f;
//
//    // Title
//    drawText("CHECKERS", x, y, 22, CLR_WHITE); y += 28.f;
//    drawText("COP 3003", x, y, 13, CLR_GRAY);  y += 28.f;
//
//    // Divider
//    sf::RectangleShape div(sf::Vector2f(210.f, 1.f));
//    div.setFillColor(CLR_SB_LINE);
//    div.setPosition({ x, y }); m_window.draw(div); y += 10.f;
//
//    // Turn / player indicator
//    bool redTurn = (m_currentPlayer == PieceColor::Red);
//    drawText(redTurn ? "Player 1  (Red)" : "Player 2  (Black)",
//        x, y, 16, redTurn ? sf::Color(240, 80, 80)
//        : sf::Color(160, 160, 160));
//    y += LINE + 4.f;
//
//    // Piece counts
//    drawText("Red   pieces: " + std::to_string(m_board.getRedCount()),
//        x, y, 14, sf::Color(240, 80, 80));   y += LINE;
//    drawText("Black pieces: " + std::to_string(m_board.getBlackCount()),
//        x, y, 14, sf::Color(160, 160, 160)); y += LINE + 6.f;
//
//    // Divider
//    div.setPosition({ x, y }); m_window.draw(div); y += 10.f;
//
//    // Status / warning message
//    sf::Color msgColor = (m_gameState == GameState::GameOver)
//        ? sf::Color(255, 215, 0)    // Gold — winner
//        : sf::Color(220, 220, 100); // Yellow — warnings
//    drawText(m_statusMsg, x, y, 13, msgColor); y += LINE + 10.f;
//
//    // Divider
//    div.setPosition({ x, y }); m_window.draw(div); y += 10.f;
//
//    // Move history
//    drawText("Move History", x, y, 14, CLR_WHITE); y += LINE + 2.f;
//
//    const int MAX_HIST = 12;
//    int start = (int)m_history.size() > MAX_HIST
//        ? (int)m_history.size() - MAX_HIST : 0;
//
//    for (int i = start; i < (int)m_history.size(); i++) {
//        sf::Color hc = (i % 2 == 0) ? CLR_GRAY : sf::Color(200, 200, 200);
//        drawText(m_history[i], x, y, 12, hc);
//        y += 17.f;
//    }
//
//    // Buttons (drawn at fixed positions near the bottom)
//    drawButton((float)BTN_X, (float)BTN_NEW_Y, (float)BTN_W, (float)BTN_H,
//        "New Game (N)", CLR_BTN_NEW);
//    drawButton((float)BTN_X, (float)BTN_QUIT_Y, (float)BTN_W, (float)BTN_H,
//        "Quit (Esc)", CLR_BTN_QUIT);
//
//    // Legend
//    drawText("Gold ring = King", x, (float)(BTN_QUIT_Y + BTN_H + 10), 11, CLR_GRAY);
//}
//
//// ── drawGameOverOverlay ───────────────────────────────────────────────────────
//void Game::drawGameOverOverlay() {
//    sf::RectangleShape overlay(sf::Vector2f((float)BOARD, (float)WIN_H));
//    overlay.setPosition({ 0.f, 0.f });
//    overlay.setFillColor(CLR_OVERLAY);
//    m_window.draw(overlay);
//
//    if (!m_fontLoaded) return;
//
//    // SFML 3: sf::Text(font, string, size)
//    {
//        sf::Text title(m_font, m_statusMsg, 42);
//        title.setFillColor(sf::Color(255, 215, 0));
//        title.setStyle(sf::Text::Bold);
//        sf::FloatRect tb = title.getLocalBounds();
//        // SFML 3: FloatRect has .position and .size instead of left/top/width/height
//        title.setOrigin({ tb.position.x + tb.size.x / 2.f,
//                          tb.position.y + tb.size.y / 2.f });
//        title.setPosition({ BOARD / 2.f, WIN_H / 2.f - 30.f });
//        m_window.draw(title);
//    }
//    {
//        sf::Text sub(m_font, "Press N for a new game", 20);
//        sub.setFillColor(CLR_WHITE);
//        sf::FloatRect sb = sub.getLocalBounds();
//        sub.setOrigin({ sb.position.x + sb.size.x / 2.f,
//                        sb.position.y + sb.size.y / 2.f });
//        sub.setPosition({ BOARD / 2.f, WIN_H / 2.f + 30.f });
//        m_window.draw(sub);
//    }
//}
//
//// ── drawButton ────────────────────────────────────────────────────────────────
//void Game::drawButton(float x, float y, float w, float h,
//    const std::string& label, sf::Color bg) {
//    sf::RectangleShape btn(sf::Vector2f(w, h));
//    btn.setPosition({ x, y });
//    btn.setFillColor(bg);
//    btn.setOutlineThickness(1.f);
//    btn.setOutlineColor(sf::Color(255, 255, 255, 60));
//    m_window.draw(btn);
//
//    if (!m_fontLoaded) return;
//
//    // SFML 3: sf::Text(font, string, size)
//    sf::Text txt(m_font, label, 15);
//    txt.setFillColor(CLR_WHITE);
//    sf::FloatRect tb = txt.getLocalBounds();
//    txt.setOrigin({ tb.position.x + tb.size.x / 2.f,
//                    tb.position.y + tb.size.y / 2.f });
//    txt.setPosition({ x + w / 2.f, y + h / 2.f });
//    m_window.draw(txt);
//}
//
//// ── drawText ─────────────────────────────────────────────────────────────────
//// SFML 3: font goes into the sf::Text constructor, not a separate setFont call.
//void Game::drawText(const std::string& str, float px, float py,
//    unsigned int size, sf::Color color) {
//    if (!m_fontLoaded) return;
//    sf::Text txt(m_font, str, size);  // SFML 3 constructor
//    txt.setFillColor(color);
//    txt.setPosition({ px, py });
//    m_window.draw(txt);
//}
//
//// ── pixelToGrid ───────────────────────────────────────────────────────────────
//bool Game::pixelToGrid(int px, int py, int& row, int& col) const {
//    if (px < 0 || px >= BOARD || py < 0 || py >= WIN_H) return false;
//    col = px / CELL;
//    row = py / CELL;
//    return row >= 0 && row < 8 && col >= 0 && col < 8;
//}
