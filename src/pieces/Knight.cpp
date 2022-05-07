#ifndef KNIGHT_CPP
#define KNIGHT_CPP

#include "Knight.h"
#include "Board.h"

namespace Chess
{
   Knight::Knight(const Position position, const Side side) : Piece(position, side) {}

   PieceType Knight::type(void) const { return KNIGHT; }

   bool Knight::can_move(const Position &to, const Board &board, const PieceType promotion_type) const
   {
      if (!Piece::can_move(to, board, promotion_type))
         return false;
      // Controllo se il pezzo vede la posizione di destinazione
      if (!is_controlling(board, to))
         return false;
      Direction diff = to - _position;
      // Controllo se sono pinnato al re
      if (!can_move_through_pin(board, diff))
         return false;
      // Controllo se è scacco e se mi posso muovere
      if (!can_move_through_check(board, to))
         return false;
      return true;
   }
   bool Knight::is_controlling(const Board &board, const Position &to) const
   {
      Direction diff = to - _position;
      // Controllo se ci posso arrivare
      if (!diff.is_knight_direction())
         return false;
      return true;
   }

   bool Knight::is_giving_check(const Board &board) const
   {
      Piece *enemy_king = board.get_king(!_side);
      Direction king_dir = enemy_king->position() - _position;
      if (!king_dir.is_knight_direction())
         return false;
      return true;
   }
}

#endif