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
      // Controllo se il pezzo vede la posizione di destinazione
      if (!is_controlling(board, to))
         return false;
      Direction dir = (to - _position).reduce();
      // Controllo se sono pinnato al re
      if (!can_move_through_pin(board, dir))
         return false;
      // Controllo se è scacco e se mi posso muovere
      if (!can_move_through_check(board, to))
         return false;
      return true;
   }

   bool Queen::is_controlling(const Board &board, const Position &to) const {
      Direction diff = to - _position;
      // Controllo se ci posso arrivare
      if (!diff.is_bishop_direction() && !diff.is_rook_direction())
         return false;
      Direction dir = diff.reduce();
      // Controllo se qualcuno ostruisce il passaggio
      if (is_obstructed(board, to, dir))
         return false;
      return true;
   }

   bool Queen::is_giving_check(const Board &board) const
   {
      Piece *enemy_king = board.get_king(!_side);
      Direction king_dir = enemy_king->position() - _position;
      if (!king_dir.is_bishop_direction() && !king_dir.is_rook_direction())
         return false;
      if (is_obstructed(board, enemy_king->position(), king_dir.reduce()))
         return false;
      return true;
   }
}

#endif