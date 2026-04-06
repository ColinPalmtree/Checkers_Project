// Game class function definitions
#include "Game.h"
#include <iostream>

// Start a new game
void Game::startGame() {
    // Reset the board to initial positions
    m_board.resetBoard();   // calls Board function to place pieces
    // Set the first player
    m_current_turn = 1;
    // Clear move history
    m_move_history.clear();
  
    std::cout << "Game started! Player 1 goes first." << std::endl;
}
// Switch to the other player's turn
void Game::switchTurn() {
    m_current_turn = (m_current_turn == 1) ? 2 : 1;
    std::cout << "Now it is Player " << m_current_turn << "'s turn." << std::endl;
}
