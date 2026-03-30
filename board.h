// Board.h - Manages the 8x8 grid and all pieces
#pragma once
#include "Piece.h"
#include <vector>

class Board {
public:
    // Constructor: initialize an empty board
    Board();

    // Initialize the board with starting positions of pieces
    void initBoard();

    // Apply a move on the board (from current row/col to new row/col)
    void applyMove(int fromRow, int fromCol, int toRow, int toCol);

    // Check for captures/jumps after a move
    void checkForCaptures(int row, int col);

    // Display board state (for console or debugging)
    void displayBoard() const;

    // Accessors
    Piece* getPiece(int row, int col) const;
    bool isEmpty(int row, int col) const;

    // Piece counters
    int getRedCount() const { return redCount; }
    int getBlackCount() const { return blackCount; }

private:
    Piece* grid[8][8];  // 2D array storing pointers to pieces (nullptr if empty)
    int redCount;       // Number of red pieces remaining
    int blackCount;     // Number of black pieces remaining
};
