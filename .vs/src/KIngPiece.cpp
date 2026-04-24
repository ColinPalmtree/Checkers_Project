// promoted piece

#include "KingPiece.h"
#include "Board.h"

// constructor
KingPiece::KingPiece(PieceColor color, int row, int col)
    : Piece(color, row, col)
{
    m_isKing = true; // king sets isKing when it is created
}

// get valid moves
// checks all legal moves a king can make in all 4 diagonal directions
std::vector<Move> KingPiece::getValidMoves(const Board& board) const
{
    std::vector<Move> moves;

    // check all 4 diagonal directions
    for (int dir : { -1, 1 })
    {
        for (int dc : { -1, 1 })
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
    }

    return moves;
}
