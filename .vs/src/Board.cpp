#include "Board.h"
#include "RegularPiece.h"
#include "KingPiece.h"
#include <iostream>
#include <stdexcept>

// COnstructor / destructor

Board::Board() : m_redCount(0), m_blackCount(0)
{
    // set em all to null pointer first 
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            m_grid[r][c] = nullptr;

    initBoard();
}

Board::~Board() {
    // Every Piece* delete them all to prevent memory leaks
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            delete m_grid[r][c];
            m_grid[r][c] = nullptr;
        }
    }
}

void Board::initBoard()
{
    // first delete whatevers there 
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            delete m_grid[r][c];
            m_grid[r][c] = nullptr;
        }
    }
    // red pieces
    //at top 
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if ((r + c) % 2 == 1) // dark squares
            {
                m_grid[r][c] = new RegularPiece(PieceColor::Red, r, c);
            }
        }
    }

    // Black pieces bottom rows 
    for (int r = 5; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if ((r + c) % 2 == 1) {                              // Dark square
                m_grid[r][c] = new RegularPiece(PieceColor::Black, r, c);
            }
        }
    }

    m_redCount = 12;
    m_blackCount = 12;
}
template <typename T>
bool Board::inBounds(T row, T col) const
{
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool Board::isEmpty(int row, int col) const{
    return inBounds(row, col) && (m_grid[row][col] == nullptr);
}

Piece* Board::getPiece(int row, int col) const
{
    if (!inBounds(row, col)) return nullptr;
    return m_grid[row][col];
}

// returns true if any of the players pieces have getvalid moves 
// if they dont it signals that someone won 
// or that all pieces are blocked which shouldnt happen 
bool Board::playerHasMoves(PieceColor player) const {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece* p = m_grid[r][c];
            if (p && p->getColor() == player) {
                if (!p->getValidMoves(*this).empty())
                    return true;
            }
        }
    }
    return false;
}

// returns the row and col of every piece belonging to player that has a jump is true in one of its valid moves vectors 
// if there is one then jump then makes it a manditory capture
// if its empty the player can move whatever piece they want 
std::vector<std::pair<int, int>> Board::getPiecesWithJumps(PieceColor player) const {
    std::vector<std::pair<int, int>> result;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece* p = m_grid[r][c];
            if (p && p->getColor() == player) {
                // Check if any of this piece's moves is a jump
                for (const Move& mv : p->getValidMoves(*this)) {
                    if (mv.isJump) {
                        result.emplace_back(r, c);
                        break;   // Only add the piece once
                    }
                }
            }
        }
    }

    return result;
}

// actually applying the move fumk
// first it validates that the move isnt illegal using those 
// if its a jump then captured piece is deleted  and count -1
// throws runtime error if anything is wrong 

void Board::applyMove(const Move& mv) {
    //checking 
    if (!inBounds(mv.fromRow, mv.fromCol) || !inBounds(mv.toRow, mv.toCol))
        throw std::runtime_error("Board::applyMove — coordinates out of bounds");

    Piece* mover = m_grid[mv.fromRow][mv.fromCol];
    if (!mover)
        throw std::runtime_error("Board::applyMove — no piece at source square");

    if (!isEmpty(mv.toRow, mv.toCol))
        throw std::runtime_error("Board::applyMove — destination square is occupied");

    // move the piece
    m_grid[mv.toRow][mv.toCol] = mover;
    m_grid[mv.fromRow][mv.fromCol] = nullptr;
    mover->setPosition(mv.toRow, mv.toCol);

    // handling captures 
    if (mv.isJump)
    {
        Piece* captured = m_grid[mv.capRow][mv.capCol];
        if (!captured)
            throw std::runtime_error("board::applyMove no piece found at capture square");

        // shrink capterd piece counter
        if (captured->getColor() == PieceColor::Red)
            --m_redCount;
        else
            --m_blackCount;

        // remove the captured piece from the board 
        delete captured;
        m_grid[mv.capRow][mv.capCol] = nullptr;
    }

}

void Board::promoteAt(int row, int col) {
    Piece* old = m_grid[row][col];
    PieceColor color = old->getColor();
    delete old; // delete reg piece
    m_grid[row][col] = new KingPiece(color, row, col); // make king 
}


// debugger
//   r = red regular piece    R = red king
//   b = black regular piece  B = black king
//   . = empty dark square    (space) = light square (never occupied)

void Board::displayBoard() const {
    std::cout << "\n  0 1 2 3 4 5 6 7\n";
    for (int r = 0; r < 8; r++) {
        std::cout << r;
        for (int c = 0; c < 8; c++) {
            Piece* p = m_grid[r][c];
            if (!p) {
                // Show dots on dark squares, spaces on light squares
                std::cout << ((r + c) % 2 == 1 ? " ." : "  ");
            }
            else {
                bool red = (p->getColor() == PieceColor::Red);
                bool king = p->getIsKing();
                std::cout << ' ' << (red ? (king ? 'R' : 'r')
                    : (king ? 'B' : 'b'));
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}
