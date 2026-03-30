#pragma once

#include "Piece.h"

class KingPiece : public Piece
{
public:
    // the constructor passes everything to pieces constructor

    KingPiece(int playerNum, const std::string& color, int row, int col);
    void move(int toRow, int toCol) override;
    // returns forward AND backward diagonal squares (kings move both directions)

    std::vector<std::pair<int,int>> getValidMoves() const override;
    // unique to King piece

};
