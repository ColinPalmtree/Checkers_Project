// Piece abstract class class definition
#pragma once
#include <string>
#include <string_view>

class Piece {
public:
	// constructor
	Piece(int player_num, std::string color, int row, int col);

	void setNum(player_num);
	int getNum();

	void setColor(color);
	std::string getColor();

	void setRow(row);
	int getRow();

	void setCol(col);
	int getCol();

	virtual move();
	// getValidMoves();   what return type? we returning some array or whatever with all the possible new coords?
	void displayPiece(); // need to wait until GUI is implemented for this to be of use

private:
	int m_player_num; // determines player1 (0) or player2 (1)
	std::string m_color; // determines color of piece when displayed on board
	/* 
	* I think we should decide the details of position variable(s)
	*     - should it be an array/vector?
	*     - should it be two separate int coords? (row, col)
	*/
	int m_row; // row where piece is
	int m_col; // column where piece is
};