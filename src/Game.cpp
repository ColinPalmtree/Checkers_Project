// Template not final just example can change
#include "Game.h"
#include <iostream>

// Start a new game
void Game::startGame() {
    m_board.initBoard();   // Reset board to initial positions
    m_current_turn = 1;    // Player 1 starts
    m_move_history.clear();

    std::cout << "Game started! Player 1 goes first.\n";
}

// Switch to the other player's turn
void Game::switchTurn() {
    m_current_turn = (m_current_turn == 1) ? 2 : 1;
    std::cout << "Now it is Player " << m_current_turn << "'s turn.\n";
}

// Make a move (very simple version for checkpoint)
void Game::makeMove(const Position& from, const Position& to) {
    try {
        m_board.applyMove(from, to);
        m_move_history.push_back({from, to});
        switchTurn();
    }
    catch (const std::exception& e) {
        std::cerr << "Move failed: " << e.what() << "\n";
    }
}

// Access the board (needed for main.cpp)
Board& Game::getBoard() {
    return m_board;
}
