#pragma once
#include "Piece.h"

class RegularPiece : public Piece
{
public:
    // the constructor passes everything to Piece's constructor
    RegularPiece(int playerNum, const std::string& color, int row, int col);
    virtual ~RegularPiece() = default; // virtual destructor

    void move(int toRow, int toCol) override;
    // get valid moves for this should only return forward diagonal squares

    std::vector<std::pair<int,int>> getValidMoves() const override;
    // unique to regular piece
    void promote();
    // creates a king piece for the same player when row 0 for red or row 7 for black

};
