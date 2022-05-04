#ifndef KNIGHT_CPP
#define KNIGHT_CPP

#include "Knight.h"
#include "Board.h"

namespace Chess
{
   Knight::Knight(const Position position, const Side side) : Piece(position, side) {}

   PieceType Knight::type(void) const { return KNIGHT; }

   bool Knight::can_move(const Position &to, const Board &board, const PieceType promotion_type) const {}
}

#endif