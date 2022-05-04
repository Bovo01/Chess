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
      Direction diff = to - _position;
      if (!diff.is_bishop_direction())
         return false;

      Direction dir = diff.reduce();
      Position curr = _position + dir;
      while (curr != to)
      {
         // Se la vista verso 'to' è ostruita ritorno false
         if (board.find_piece(curr) != nullptr)
            return false;
         curr += dir;
      }
      if (!can_move_through_pin(board, dir))
         return false;
      // TODO Controlla se sono sotto scacco
      return true;
   }
}

#endif