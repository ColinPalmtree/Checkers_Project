
//RegularPiece.h   A standard checkers piece
// COP 3003



#pragma once
#include "Piece.h"


class RegularPiece: public Piece {
public:
	// Passes color and startign position to constructor
	// m_isking starts false auto bc piece constructor
	RegularPiece(PieceColor color, int row, int col);

	virtual ~RegularPiece() = default;

	// returns all legal moves 
		// forward only diaganol 
		// and jumps 
	std::vector<Move> getValidMoves(const Board& board) const override;


};