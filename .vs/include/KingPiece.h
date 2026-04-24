
//king piece.h the king checkers piece
// COP 3003
// created by board at Board:: promoteAt()
// all 4 diagonal directions 


#pragma once
#include "Piece.h"

class KingPiece : public Piece {
public:
	// Passes color and starting position to the Piece constructor
	   // KingPiece will then mark itself as a king in KingPiece.cpp	
	KingPiece(PieceColor color, int row, int col);

	virtual ~KingPiece() = default;

	// returns all the moves for king piece 
	std::vector<Move> getValidMoves(const Board& board) const override;
};
