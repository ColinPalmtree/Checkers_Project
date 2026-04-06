// Regular Piece function definitions
#include "RegularPiece.h"
#include <iostream>

RegularPiece::RegularPiece(Color c, int r, int col)
    : Piece(c, r, col)
{
    isKing = false; // Regular pieces start as non-kings
}

void RegularPiece::move(int newRow, int newCol) {
    // Forward-only movement
    std::cout << "RegularPiece moving from ("
              << m_row << ", " << m_col << ") to ("
              << newRow << ", " << newCol << ")\n";

    m_row = newRow;
    m_col = newCol;
}

std::vector<Move> RegularPiece::getValidMoves(const Board& board) const {
    std::vector<Move> moves;

    // Regular pieces move forward only:
    // RED moves "down" (increasing row)
    // BLACK moves "up" (decreasing row)
    int direction = (m_color == RED) ? 1 : -1;

    int nr1 = m_row + direction;
    int nc1 = m_col + 1;

    int nr2 = m_row + direction;
    int nc2 = m_col - 1;

    // Check diagonal-right
    if (board.isValidPosition(nr1, nc1) && board.getPiece(nr1, nc1) == nullptr) {
        moves.push_back({m_row, m_col, nr1, nc1});
    }

    // Check diagonal-left
    if (board.isValidPosition(nr2, nc2) && board.getPiece(nr2, nc2) == nullptr) {
        moves.push_back({m_row, m_col, nr2, nc2});
    }

    return moves;
}
