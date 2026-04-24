#include <Game.h>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <algorithm>
#include <stdexcept>

// colors to be used
static const sf::Color WHITE{ 255, 255, 255 };
static const sf::Color GRAY{ 170, 170, 170 };
static const sf::Color LIGHT{ 220, 20, 20 };   // light squares
static const sf::Color DARK{ 30, 30, 30 };   // dark squares
static const sf::Color RED_P{ 200, 30, 30 };   // Red piece fill
static const sf::Color RED_O{ 240, 90, 90 };   // Red piece outline
static const sf::Color BLK_P{ 25, 25, 25 };   // Black piece fill
static const sf::Color BLK_O{ 90, 90, 90 };   // Black piece outline
static const sf::Color KING{ 255, 215, 0 };   // king indicator dot
static const sf::Color SELECT{ 255, 255, 0, 160 }; // selected square
static const sf::Color STEP{ 0, 200, 0, 200 }; // move dot
static const sf::Color JUMP{ 255, 120, 0, 230 }; // jump dot
static const sf::Color MUST{ 255, 220, 0, 180 }; // must capture border
static const sf::Color BTN_NEW{ 70, 130, 180 };   // new game
static const sf::Color BTN_QUIT{ 150, 50, 50 };   // quit
static const sf::Color OVERLAY{ 0, 0, 0, 180 }; // game over
static const sf::Color SIDEBAR_BG{ 35, 35, 35 };   // sidebar background
static const sf::Color SIDEBAR_LINE{ 60, 60, 60 };   // sidebar separation lines

// constructor
Game::Game() : m_currentPlayer(PieceColor::Red),
m_gameState(GameState::Playing), m_playerState(PlayerState::Idle),
m_window(sf::VideoMode({ static_cast<unsigned>(WIN_W), static_cast<unsigned>(WIN_H) }), "Checkers")
{
	// cap fps to 60
	m_window.setFramerateLimit(60);

	// load a font
	m_fontLoaded = m_font.openFromFile("arial.ttf");
	
	startNewGame();
}

// main SFML event loop
void Game::run() {
	while (m_window.isOpen()) {
		while (const std::optional event = m_window.pollEvent()) {
			// close window button
			if (event->is<sf::Event::Closed>()) {
				m_window.close();
			}

			// keyboard shortcuts
			else if (const auto* key_press = event->getIf<sf::Event::KeyPressed>()) {
				if (key_press->code == sf::Keyboard::Key::Escape) {
					m_window.close();
				}
				if (key_press->code == sf::Keyboard::Key::N) {
					startNewGame();
				}
			}

			// mouse clicks
			else if (const auto* mouse_button = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mouse_button->button == sf::Mouse::Button::Left) {
					// mouse position stored in sfml vector
					int mouse_x = mouse_button->position.x;
					int mouse_y = mouse_button->position.y;

					// board click
					int row;
					int col;
					if (m_gameState == GameState::Playing && pixelToGrid(mouse_x, mouse_y, row, col)) {
						onSquareClick(row, col);
					}

					// sidebar button clicks
					// new game button
					// the if checks if the mouse position is inside the button
					if (mouse_x >= BTN_X && mouse_x <= BTN_X + BTN_W &&
						mouse_y >= BTN_NEW_Y && mouse_y <= (BTN_NEW_Y + BTN_H)) {
						startNewGame();
					}

					// quit button
					// same if checks here but for the quit buttons
					if (mouse_x >= BTN_X && mouse_x <= BTN_X + BTN_W &&
						mouse_y >= BTN_QUIT_Y && mouse_y <= (BTN_QUIT_Y + BTN_H)) {
						m_window.close();
					}
				}
			}

		}
		draw();
	}
}

// game logic
void Game::startNewGame() {
	m_board.initBoard();
	m_currentPlayer = PieceColor::Red;
	m_gameState = GameState::Playing;
	m_playerState = PlayerState::Idle;
	m_selectRow = -1;
	m_selectCol = -1;
	m_validMoves.clear();
	m_mustCapture.clear();
	m_moveLog.clear();
	m_statusMsg = "Red's turn";
}

void Game::checkMustCapture() {
	m_mustCapture = m_board.getPiecesWithJumps(m_currentPlayer);
}

void Game::onSquareClick(int row, int col) {
	// chain jump handling
	if (m_playerState == PlayerState::ChainJumping) {
		for (const Move& move : m_validMoves) {
			if (move.toRow == row && move.toCol == col) {
				executeMove(move);
				return;
			}
		}
		m_statusMsg = "Chain jumping";
		return;
	}

	Piece* clicked = m_board.getPiece(row, col);

	// selecting a piece handling
	if (m_playerState == PlayerState::PieceSelected) {
		for (const Move& move : m_validMoves) {
			if (move.toRow == row && move.toCol == col) {
				executeMove(move);
				return;
			}
		}

		if (clicked && clicked->getColor() == m_currentPlayer) {
			selectPiece(row, col);
			
			return;
		}
		// if an empty/enemy square was clicked then deselect squares
		else {
			m_selectRow = -1;
			m_selectCol = -1;
			m_validMoves.clear();
			m_playerState = PlayerState::Idle;

			// set status message
			if (m_currentPlayer == PieceColor::Red) {
				m_statusMsg = "Red's turn";
			}
			else if (m_currentPlayer == PieceColor::Black) {
				m_statusMsg = "Black's turn";
			}
			return;
		}
	}

	selectPiece(row, col);
}

void Game::selectPiece(int row, int col) {
	Piece* piece = m_board.getPiece(row, col);
	
	// ensure a nullptr isn't being read
	if (!piece || piece->getColor() != m_currentPlayer) 
		return;
	// mandatory capture selection handling
	if (!m_mustCapture.empty()) {
		auto it = std::find_if(m_mustCapture.begin(), m_mustCapture.end(),
			[row, col](const std::pair<int, int>& piece_coords) {
				return piece_coords.first == row && piece_coords.second == col;
			});
		if (it == m_mustCapture.end()) {
			m_statusMsg = "Mandatory capture on board";
			return;
		}
	}

	std::vector<Move> all_moves = piece->getValidMoves(m_board);
	
	if (m_mustCapture.empty()) {
		m_validMoves = all_moves;
	}
	else {
		m_validMoves = jumpsOnly(all_moves);
	}

	if (m_validMoves.empty()) {
		m_statusMsg = "The selected piece has no legal moves";
		return;
	}

	m_selectRow = row;
	m_selectCol = col;
	m_playerState = PlayerState::PieceSelected;
	m_statusMsg = "Click a highlighted square to move";
}

void Game::executeMove(const Move& move) {
	try {
		m_board.applyMove(move);
	}
	catch (const std::exception& e) {
		m_statusMsg = std::string("Error: ") + e.what();
		return;
	}

	bool wasJump = move.isJump;
	bool promoted = false;

	// check promotion, red promotes at row 7, black promotes at row 0
	Piece* piece = m_board.getPiece(move.toRow, move.toCol);
	// ensures no nullptr is passed
	if (piece && !piece->getIsKing()) {
		bool redPromote = piece->getColor() == PieceColor::Red && move.toRow == 7;
		bool blackPromote = piece->getColor() == PieceColor::Black && move.toRow == 0;
		// if either was promoted, tell the board
		if (redPromote || blackPromote) {
			m_board.promoteAt(move.toRow, move.toCol);
			promoted = true;

			// turn is ended upon promotion
			wasJump = false;
		}
	}

	logMove(move, promoted, m_currentPlayer);

	if (wasJump) {
		Piece* jump_piece = m_board.getPiece(move.toRow, move.toCol);
		if (jump_piece) {
			std::vector<Move> move_vector = jumpsOnly(jump_piece->getValidMoves(m_board));
			if (!move_vector.empty()) {
				m_playerState = PlayerState::ChainJumping;
				m_selectRow = move.toRow;
				m_selectCol = move.toCol;
				m_validMoves = move_vector;
				m_statusMsg = "Keep jumping!";
				return;
			}
		}
	}

	switchTurn();
}

void Game::switchTurn() {
	// what to switch m_currentPlayer to?
	if (m_currentPlayer == PieceColor::Red) {
		m_currentPlayer = PieceColor::Black;
		m_statusMsg = "Black's turn";
	}
	else {
		m_currentPlayer = PieceColor::Red;
		m_statusMsg = "Red's turn";
	}

	m_playerState = PlayerState::Idle;
	m_selectRow = -1;
	m_selectCol = -1;
	m_validMoves.clear();

	if (checkWin()) 
		return;

	checkMustCapture();
}

bool Game::checkWin() {
	if (!m_board.playerHasMoves(m_currentPlayer)) {
		m_gameState = GameState::GameOver;
		std::string winner;
		// store winner in a string
		if (m_currentPlayer == PieceColor::Red) {
			winner = "Black";
		}
		else {
			winner = "Red";
		}
		m_statusMsg = winner + " wins!";
		return true;
	}
	return false;
}

std::vector<Move> Game::jumpsOnly(const std::vector<Move>& moves) {
	std::vector<Move> result;
	for (const Move& move : moves) {
		if (move.isJump) {
			result.push_back(move);
		}
	}
	return result;
}

void Game::logMove(const Move& move, bool promoted, PieceColor move_player) {
	std::ostringstream oss;
	
	if (move_player == PieceColor::Black) {
		oss << "Black";
	}
	else {
		oss << "Red";
	}
	
	oss << ": (" << move.fromRow << "," << move.fromCol << ")"
		<< (move.isJump ? " x " : " - ") // dash for move, x for capture/jump
		<< "(" << move.toRow << "," << move.toCol << ")";

	if (promoted) {
		oss << "[K]";
	}
	
	m_moveLog.push_back(oss.str());
}

// Drawing to screen

void Game::draw() {
	m_window.clear(sf::Color::Black);
	drawBoard();
	drawHighlights();
	drawPieces();
	drawSidebar();
	if (m_gameState == GameState::GameOver)
		drawGameOver();
	m_window.display();
}

void Game::drawBoard() {
	sf::RectangleShape square(sf::Vector2f(SQUARE, SQUARE));
	
	// loop for each square on board
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			square.setPosition({ static_cast<float>(col * SQUARE), static_cast<float>(row * SQUARE) });
			
			// choose color alternating
			if ((row + col) % 2 == 0) {
				square.setFillColor(LIGHT);
			}
			else {
				square.setFillColor(DARK);
			}

			m_window.draw(square);
		}
	}
}

void Game::drawHighlights() {
	sf::RectangleShape square(sf::Vector2f(SQUARE, SQUARE));
	sf::RectangleShape border(sf::Vector2f(SQUARE - 6, SQUARE - 6));

	// must capture border
	if (m_playerState != PlayerState::ChainJumping) {
		border.setFillColor(sf::Color::Transparent);
		border.setOutlineThickness(3.f);
		border.setOutlineColor(MUST);

		for (const auto& [row, col] : m_mustCapture) {
			border.setPosition({ static_cast<float>(col * SQUARE + 3), static_cast<float>(row * SQUARE + 3) });
			m_window.draw(border);
		}
	}

	// selected square highlight
	if (m_selectRow >= 0) {
		square.setFillColor(SELECT);
		square.setPosition({ static_cast<float>(m_selectCol * SQUARE), static_cast<float>(m_selectRow * SQUARE) });
		m_window.draw(square);
	}

	// move dots
	const float DOT_R = 10.0f; // dot radius
	sf::CircleShape dot(DOT_R);
	dot.setOrigin({ DOT_R, DOT_R });
	for (const Move& move : m_validMoves) {
		if (move.isJump) {
			dot.setFillColor(JUMP);
		}
		else {
			dot.setFillColor(STEP);
		}
		dot.setPosition({ move.toCol * SQUARE + SQUARE / 2.0f, move.toRow * SQUARE + SQUARE / 2.0f });

		m_window.draw(dot);
	}
}


void Game::drawPieces() {
	// load piece textures
	sf::Texture red_reg_text("Assets/Red Piece.png");
	sf::Texture blk_reg_text("Assets/Black Piece.png");
	sf::Texture red_king_text("Assets/Red King.png");
	sf::Texture blk_king_text("Assets/Black King.png");

	// smooth textures
	red_reg_text.setSmooth(true);
	blk_reg_text.setSmooth(true);
	red_king_text.setSmooth(true);
	blk_king_text.setSmooth(true);

	// make sprites from textures
	sf::Sprite red_reg(red_reg_text);
	sf::Sprite blk_reg(blk_reg_text);
	sf::Sprite red_king(red_king_text);
	sf::Sprite blk_king(blk_king_text);

	// fix scale to board size
	red_reg.scale({ 0.5f, 0.5f });
	blk_reg.scale({ 0.5f, 0.5f });
	red_king.scale({ 0.5f, 0.5f });
	blk_king.scale({ 0.5f, 0.5f });

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			// skip this square if empty (nullptr)
			Piece* piece = m_board.getPiece(row, col);
			if (!piece) 
				continue; 

			// position of Piece
			float cell_x = (col * SQUARE + SQUARE / 2.0f) - 62.0f;
			float cell_y = (row * SQUARE + SQUARE / 2.0f) - 60.0f;
			
			// set colors of piece
			if (piece->getColor() == PieceColor::Red) {
				if (piece->getIsKing()) {
					red_king.setPosition({ cell_x, cell_y });
					m_window.draw(red_king);
				}
				else {
					red_reg.setPosition({ cell_x, cell_y });
					m_window.draw(red_reg);

				}
			}
			else if (piece->getColor() == PieceColor::Black) {
				if (piece->getIsKing()) {
					blk_king.setPosition({ cell_x, cell_y });
					m_window.draw(blk_king);
				}
				else {
					blk_reg.setPosition({ cell_x, cell_y });
					m_window.draw(blk_reg);
				}


			}
		}
	}
}

void Game::drawSidebar() {
	// background panel
	sf::RectangleShape bg(sf::Vector2f(static_cast<float>(WIN_W - SIDEBAR_X), static_cast<float>(WIN_H)));
	bg.setPosition({ static_cast<float>(SIDEBAR_X), 0.0f });
	bg.setFillColor(SIDEBAR_BG);
	m_window.draw(bg);

	float x = static_cast<float>(SIDEBAR_X) + 10.0f;
	float y = 10.0f;
	const float LINE = 22.0f;

	// title
	drawText("CHECKERS", x, y, 22, WHITE);
	y += 56.0f; // update vertical draw position

	// dividing line
	sf::RectangleShape div(sf::Vector2f(210.0f, 1.0f));
	div.setFillColor(SIDEBAR_LINE);
	div.setPosition({ x,y });
	m_window.draw(div);
	y += 10.0f;

	// player turn indicator
	if (m_currentPlayer == PieceColor::Red) {
		drawText("Red's Turn", x, y, 16, sf::Color(240, 80, 80));
	}
	else if (m_currentPlayer == PieceColor::Black) {
		drawText("Black's Turn", x, y, 16, sf::Color(160, 160, 160));
	}
	
	y += LINE + 4.0f;

	// Piece counts
	drawText("Red   pieces: " + std::to_string(m_board.getRedCount()),
		x, y, 14, sf::Color(240, 80, 80));
	y += LINE;
	drawText("Black pieces: " + std::to_string(m_board.getBlackCount()),
		x, y, 14, sf::Color(160, 160, 160));
	y += LINE + 6.f;

	// Divider
	div.setPosition({ x,y });
	m_window.draw(div);
	y += 10.0f;

	sf::Color msgColor;
	// Status message
	if (m_gameState == GameState::GameOver) {
		// golden color to announce winner
		msgColor = sf::Color(255, 215, 0);
	}
	else {
		// yellow color for regular status message
		msgColor = sf::Color(220, 220, 100);
	}

	drawText(m_statusMsg, x, y, 13, msgColor);
	y += LINE + 10.0f;

	// another divider
	div.setPosition({ x, y }); m_window.draw(div);
	y += 10.f;

	// Move log
	drawText("Move log", x, y, 14, WHITE);
	y += LINE + 2.0f;

	const int MAX_LOG = 12;

	int start;
	if (static_cast<int>(m_moveLog.size()) > MAX_LOG) {
		start = static_cast<int>(m_moveLog.size()) - MAX_LOG;
	}
	else {
		start = 0;
	}

	for (int i = start; i < static_cast<int>(m_moveLog.size()); i++) {
		sf::Color log = (i % 2 == 0) ? GRAY : sf::Color(200, 200, 200);
		drawText(m_moveLog[i], x, y, 12, log);
		y += 17.0f;
	}

	// buttons
	drawButton(static_cast<float>(BTN_X), static_cast<float>(BTN_NEW_Y), static_cast<float>(BTN_W), static_cast<float>(BTN_H),
		"New Game (N)", BTN_NEW);
	drawButton(static_cast<float>(BTN_X), static_cast<float>(BTN_QUIT_Y), static_cast<float>(BTN_W), static_cast<float>(BTN_H),
		"Quit (Esc)", BTN_QUIT);
}

void Game::drawGameOver() {
	sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(BOARD), static_cast<float>(WIN_H)));
	overlay.setPosition({ 0.0f, 0.0f });
	overlay.setFillColor(OVERLAY);
	m_window.draw(overlay);

	if (!m_fontLoaded) return;

	sf::Text text(m_font, m_statusMsg, 42);
	text.setFillColor(sf::Color(255, 215, 0));
	text.setStyle(sf::Text::Bold);
	sf::FloatRect text_bounds = text.getLocalBounds();

	text.setOrigin({ text_bounds.position.x + text_bounds.size.x / 2.0f,
					text_bounds.position.y + text_bounds.size.y / 2.0f });
	text.setPosition({ BOARD / 2.0f, WIN_H / 2.0f - 30.0f });
	m_window.draw(text);

	sf::Text sub(m_font, "Press N for a new game", 20);
	sub.setFillColor(WHITE);
	sf::FloatRect sub_bounds = sub.getLocalBounds();
	sub.setOrigin({ sub_bounds.position.x + sub_bounds.size.x / 2.0f,
					sub_bounds.position.y + sub_bounds.size.y / 2.0f });
	sub.setPosition({ BOARD / 2.0f, WIN_H / 2.0f + 300.f });
	m_window.draw(sub);
}

void Game::drawButton(float x, float y, float w, float h,
	const std::string& label, sf::Color bg) {
	sf::RectangleShape button(sf::Vector2f(w, h));
	button.setPosition({ x, y });
	button.setFillColor(bg);
	button.setOutlineThickness(1.0f);
	button.setOutlineColor(sf::Color(255, 255, 255, 60));
	m_window.draw(button);

	if (!m_fontLoaded) return;

	sf::Text text(m_font, label, 15);
	text.setFillColor(WHITE);
	sf::FloatRect text_bounds = text.getLocalBounds();
	text.setOrigin({ text_bounds.position.x + text_bounds.size.x / 2.0f,
					 text_bounds.position.y + text_bounds.size.y / 2.0f });
	text.setPosition({ x + w / 2.0f, y + h / 20.0f + 20 });
	m_window.draw(text);
}

void Game::drawText(const std::string& str, float x, float y, unsigned int size,
	sf::Color color) {
	if (!m_fontLoaded) return;
	sf::Text text(m_font, str, size);
	text.setFillColor(color);
	text.setPosition({ x, y });
	m_window.draw(text);
}

bool Game::pixelToGrid(int x, int y, int& row, int& col) const {
	if (x < 0 || x >= BOARD || y < 0 || y >= WIN_H) {
		return false;
	}
	col = x / SQUARE;
	row = y / SQUARE;
	return row >= 0 && row < 8 && col >= 0 && col < 8;
}