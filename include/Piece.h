// Abstract class for all the pieces in game 
/*
piece is abstract(only virtual getValid Moves)
Regular Piece and KIng piece inherit from it 
inheritance, polymorphism and abstraction
*/ 

#pragma once
#include <vector>
using namespace std;

// declare board so piece can use it as a param
class Board;

enum class PieceColor { Red, Black };

// move is a single possible move on the checkers board used by all pieces
struct Move {
	int fromRow, fromCol;
	int toRow, toCol;
	bool isJump;
	int capRow, capCol;
};

// abstract base class
class Piece {
public:
	// init object
	Piece(PieceColor color, int row, int col);

	// Vertual distructor so the subclass destructors run right 
	virtual ~Piece() = default;

	// =0 so every subclass has to impliment its own version of it 
	virtual vector<Move> getValidMoves(const Board& board) const = 0;
	
	// for storing coords
	void setPosition(int row, int col);

	PieceColor getColor() const { retuurn m_color; }
	int getRow() const { return m_row; }
	int getCol() const { return m_col; }
	bool getIsKing() const { return m_isKing; }

		// when get to other side promote 
		// replace regpiece with king piece
		// polymorph

		void promoteToKing() { m_isKing = true; }

protected:
	PieceColor m_color; // red or black 
	int m_row;//board
	int m_col;//board
	bool m_isKing; // king

};