#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Board.h"

// enum to see if game is in play or not
enum GameState {
	Playing,
	GameOver
};

// enum to track player's current action
enum PlayerState {
	Idle,
	PieceSelected,
	ChainJumping
};

class Game {
public:
	Game();
	void run();

private:
	// these functions deal with game logic
	void startNewGame(); // reset everything and start new game
	
	void checkMustCapture(); // update m_mustCapture for current player

	void onSquareClick(int row, int col); // deals with what happens when a square is clicked

	void selectPiece(int row, int col); // highlights selected piece and valid moves

	void executeMove(const Move& move); // move, check for promotion, and call the move to log

	void switchTurn(); // switch turn and make checks

	bool checkWin(); // check if game is over, if so update gameState and statusMsg

	std::vector<Move> jumpsOnly(const std::vector<Move>& moves); // take a list of moves and only get the jumps

	void logMove(const Move& move, bool promoted, PieceColor mover); // add line to move log

	// these functions draw to the screen
	void draw(); // draws everything, calls the others

	void drawBoard();

	void drawHighlights(); // selects, move dots, must-captures

	void drawPieces();

	void drawSidebar(); // info, everything that is not the board

	void drawGameOver(); // game over overlay

	void drawButton(float x, float y, float w, float h, const std::string& label, sf::Color bg);

	void drawText(const std::string& str, float x, float y, unsigned int size, sf::Color color);

	bool pixelToGrid(int x, int y, int& row, int& col) const; // window pixel to board row/col

	// attributes

	// layout constants
	static constexpr int SQUARE = 80; // pixels per square
	static constexpr int BOARD = 8 * SQUARE; // total board size
	static constexpr int SIDEBAR_X = BOARD + 5; // left edge of sidebar buffer
	static constexpr int WIN_W = BOARD + 235; // width of window
	static constexpr int WIN_H = BOARD + 16; // height of window
	static constexpr float BTN_X = SIDEBAR_X + 10.0; // left edge of buttons
	static constexpr float BTN_W = 210.0; // width of buttons
	static constexpr float BTN_H = 42.0; // height of buttons
	static constexpr float BTN_NEW_Y = WIN_H - 110.0; // y position of new game button
	static constexpr float BTN_QUIT_Y = WIN_H - 58.0; // y position of quit button

	// game state attributes
	Board m_board;
	PieceColor m_currentPlayer; // track who is current player via color
	GameState m_gameState;
	PlayerState m_playerState;

	// -1 represents that no piece is selected
	int m_selectRow = -1; // current piece's row
	int m_selectCol = -1; // current piece's column

	std::vector<Move> m_validMoves; // selected piece's legal moves

	std::vector<std::pair<int, int>> m_mustCapture; // mandatory capture pieces
	
	std::vector<std::string> m_moveLog; // move log to appear on sidebar
	std::string m_statusMsg; // message to player

	sf::RenderWindow m_window;
	sf::Font m_font; // font that is loaded
	bool m_fontLoaded = false; // if a font was successfully loaded or not
};