// King Piece function definitions
#include "KingPiece.h"
#include <iostream>

// Things can change (colors, etc)
KingPiece::KingPiece(Color c, int r, int col)
    : Piece(c, r, col) 
{
    isKing = true;   // Kings start as kings
}

void KingPiece::move(int newRow, int newCol) {
   
  // Movement logic
    std::cout << "KingPiece moving from (" 
              << row << ", " << col << ") to ("
              << newRow << ", " << newCol << ")\n";

    row = newRow;
    col = newCol;
}

std::vector<Move> KingPiece::getValidMoves(const Board& board) const {
    std::vector<Move> moves;

    // Kings can move diagonally in all 4 directions
  
    int directions[4][2] = {
        {1, 1},   // down-right
        {1, -1},  // down-left
        {-1, 1},  // up-right
        {-1, -1}  // up-left
    };

    for (auto& d : directions) {
        int nr = row + d[0];
        int nc = col + d[1];

        if (board.isValidPosition(nr, nc) && board.getPiece(nr, nc) == nullptr) {
            moves.push_back({row, col, nr, nc});
        }
    }

    return moves;
}
