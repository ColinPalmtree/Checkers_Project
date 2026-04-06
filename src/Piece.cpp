// Class Piece function definitions
#include <string>
#include "Piece.h"

Piece::Piece(int player_num, std::string color, int row, int col) :
	m_player_num{ player_num }, m_color{ color }, m_row{ row }, m_col{ col } 
    { }

void Piece::setNum(player_num) { m_player_num = player_num; }
int Piece::getNum() { return m_player_num; }

void Piece::setColor(color) { m_color = color; }
std::string Piece::getColor() { return m_color; }

void Piece::setRow(row) { m_row = row; }
int Piece::getRow() { return row; }

void Piece::setCol(col) { m_col = col; }
int Piece::getCol() { return m_col; }