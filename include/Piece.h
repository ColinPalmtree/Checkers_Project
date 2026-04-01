// Piece abstract class class definition
#pragma once
#include <string>
#include <string_view>

class Piece {
public:
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
	int m_position[8][4]; // stores position of piece on 8x4 dark spaces
};