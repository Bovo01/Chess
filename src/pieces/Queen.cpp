#ifndef QUEEN_CPP
#define QUEEN_CPP

#include "Queen.h"
#include "Board.h"

namespace Chess
{
   Queen::Queen(const Position position, const Side side) : Piece(position, side) {}

   PieceType Queen::type(void) const { return QUEEN; }

   bool Queen::can_move(const Position &to, const Board &board, const PieceType promotion_type) const
   {
      if (!Piece::can_move(to, board, promotion_type))
         return false;
   }
}

#endif