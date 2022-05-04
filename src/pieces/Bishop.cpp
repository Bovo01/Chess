#ifndef BISHOP_CPP
#define BISHOP_CPP

#include "Bishop.h"
#include "Board.h"

namespace Chess
{
   Bishop::Bishop(const Position position, const Side side) : Piece(position, side) {}

   PieceType Bishop::type(void) const { return BISHOP; }

   bool Bishop::can_move(const Position &to, const Board &board, const PieceType promotion_type) const
   {
      if (!Piece::can_move(to, board, promotion_type))
         return false;
      Direction diff = to - _position;
      // Controllo se ci posso arrivare
      if (!diff.is_bishop_direction())
         return false;
      Direction dir = diff.reduce();
      // TODO Controlla se sono sotto scacco

      // Controllo se qualcuno ostruisce il passaggio
      if (is_obstructed(board, to, dir))
         return false;
      // Controllo se sono pinnato al re
      if (!can_move_through_pin(board, dir))
         return false;
      return true;
   }
}

#endif