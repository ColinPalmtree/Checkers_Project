#include "RegularPiece.h"
#include "Board.h"

namespace direction {
	const int left = -1;
	const int right = 1;
}

// constructor
RegularPiece::RegularPiece(PieceColor color, int row, int col) : Piece(color, row, col)
{
}

// regular get valid moves
/*

movement is determined by color
red moves down board
black moves up

2 types of moves are generated
step - normal
jump - capture
both diagonals are checked

*/


	std::vector<Move> RegularPiece::getValidMoves(const Board & board) const
	{
		std::vector<Move> moves;
		using namespace direction;

		int dir = (m_color == PieceColor::Red) ? 1 : -1;

		// check the forward diagonals
		for (int dc : { left, right })
		{
			int nextrow = m_row + dir;
			int nextcol = m_col + dc;

			if (!board.inBounds(nextrow, nextcol)) continue;

			Piece* neighbor = board.getPiece(nextrow, nextcol);

			if (neighbor == nullptr)
			{
				// square is empty so this is a normal move
				moves.push_back({ m_row, m_col, nextrow, nextcol, false, -1, -1 });
			}
			else if (neighbor->getColor() != m_color)
			{
				// square has an opponent piece, so check if jump is possible
				int jumprow = nextrow + dir;
				int jumpcol = nextcol + dc;

				if (board.inBounds(jumprow, jumpcol) && board.isEmpty(jumprow, jumpcol))
				{
					moves.push_back({ m_row, m_col, jumprow, jumpcol, true, nextrow, nextcol });
				}
			}
		}
		return moves;
	}
