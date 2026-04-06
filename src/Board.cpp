// Board class function definitions
#include "Board.h"
#include "RegularPiece.h"
#include "KingPiece.h"
#include <iostream>

// Rough Moc up of what we need
Board::Board() {
    initBoard();
}

void Board::initBoard() {
    // Clear board
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            grid[r][c] = nullptr;
        }
    }

    // Place RED pieces (top of board)
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 8; c++) {
            if ((r + c) % 2 == 1) {
                grid[r][c] = new RegularPiece(RED, r, c);
            }
        }
    }

    // Place BLACK pieces (bottom of board)
    for (int r = 5; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if ((r + c) % 2 == 1) {
                grid[r][c] = new RegularPiece(BLACK, r, c);
            }
        }
    }

    redCount = 12;
    blackCount = 12;
}

bool Board::isValidPosition(int r, int c) const {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

bool Board::isValidMove(const Position& from, const Position& to) const {
    if (!isValidPosition(from.row, from.col) || !isValidPosition(to.row, to.col))
        return false;

    Piece* p = grid[from.row][from.col];
    if (!p)
        return false;

    
    // Real logic will be implemented later
    return grid[to.row][to.col] == nullptr;
}

void Board::applyMove(const Position& from, const Position& to) {
    if (!isValidMove(from, to))
        throw std::runtime_error("Invalid move");

    Piece* p = grid[from.row][from.col];
    grid[from.row][from.col] = nullptr;

    p->move(to.row, to.col);
    grid[to.row][to.col] = p;
}

void Board::displayBoard() const {
    std::cout << "\nCurrent Board:\n";
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (grid[r][c] == nullptr) {
                std::cout << ". ";
            } else {
                std::cout << (grid[r][c]->getColor() == RED ? "R " : "B ");
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

Piece* Board::getPiece(int r, int c) const {
    if (!isValidPosition(r, c))
        return nullptr;
    return grid[r][c];
}

Board::~Board() {
    // Clean up allocated pieces
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            delete grid[r][c];
        }
    }
}
