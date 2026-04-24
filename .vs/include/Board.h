/*
Board.h Manages the 8x8 checkers grid of piece pointers 


board shows composition game has a board, board has a 2d array
of piece pointers. Board is the owner of  every Piece*
and allocated them in init Board() deleting them whe their captured or created in king 
*/

#pragma once
#include "Piece.h"
#include <vector>
#include <utility>
#include <stdexcept>

class Board {
public:
	Board();
	~Board();
	// copy and assignment are deleted; 
	Board(const Board&) = delete;
	Board& operator = (const Board&) = delete;

	// board detup 
	//red on rows 0-2
	// black on 7-5

	void initBoard();

	bool inBounds(int row, int col) const;

	// true if square is empty 
	bool isEmpty(int row, int col) const;
	// Returns the piece at (row, col), or null pointer
	Piece* getPiece(int row, int col)const;

	int getRedCount() const { return m_redCount; }
	int getBlackCount() const { return m_blackCount; }

	// Returns true if player has a move
	bool playerHasMoves(PieceColor Player) const;

	// returns the row col of every piece that can make a jump 
	std::vector<std::pair<int, int>> getPiecesWithJumps(PieceColor player) const;
	
	// moveing 

	// move piece from mv.from rowcol to mv. to rowcol
	// if mv. isjump is true the piece ar mv capture is removed
	//throws an error if out of bounds or illegal move 
	void applyMove(const Move& mv);

	// replace Reg Piece when gets to other side 
	void promoteAt(int row, int col);

	//print board
	void displayBoard() const;

private:
	Piece* m_grid[8][8]; // its all pointers 
	int m_redCount; // red pieces still in poard
	int m_blackCount; // blacks still on board 

};
