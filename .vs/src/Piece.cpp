#include "Piece.h"
//c Constructor
// sets color, inititial position, and marks it as not a king
Piece ::Piece(PieceColor color, int row, int col): m_color(color), m_row(row), m_col(col), m_isKing(false){}

// setpositoin
// board gets it after successful move to update the pieces 
// coordinates with the location on the board
void Piece::setPosition(int row, int col)
{
	m_row = row; 
	m_col = col;
}
