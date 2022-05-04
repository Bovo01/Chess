#ifndef PAWN_CPP
#define PAWN_CPP

#include "Pawn.h"
#include "Board.h"

namespace Chess
{
   Pawn::Pawn(const Position position, const Side side) : Piece(position, side) {}

   PieceType Pawn::type(void) const { return PAWN; }

   bool Pawn::can_move(const Position &to, const Board &board, const PieceType promotion_type) const
   {
      if (!Piece::can_move(to, board, promotion_type))
         return false;
   }
}

#endif