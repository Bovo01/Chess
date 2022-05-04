#ifndef KING_CPP
#define KING_CPP

#include "King.h"
#include "Board.h"

namespace Chess
{
   King::King(const Position position, const Side side) : Piece(position, side) {}
   
   PieceType King::type(void) const { return KING; }

   bool King::can_move(const Position &to, const Board &board, const PieceType promotion_type) const {}
}

#endif