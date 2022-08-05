#ifndef QUEEN_CPP
#define QUEEN_CPP

#include "Queen.h"
#include "Board.h"

namespace Chess
{
   Queen::Queen(const Position position, const Side side) : Piece(position, side) {}

   PieceType Queen::type(void) const { return QUEEN; }

   bool Queen::can_move(const Position &to, const Board &board) const
   {
      if (!Piece::can_move(to, board))
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

   bool Queen::can_counter_check(const Board &board, const std::vector<Position> cells_to_block_check) const
   {
      for (const Position to : cells_to_block_check)
      {
         Direction dir = (to - _position).reduce();
         if (is_controlling(board, to) && can_move_through_pin(board, dir))
            return true;
      }
      return false;
   }

   bool Queen::is_controlling(const Board &board, const Position &to) const
   {
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

   bool Queen::has_legal_moves_ignore_checks(const Board &board) const
   {
      for (short i = -1; i <= 1; i++) {
         for (short j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)
               continue;
            Direction dir = {i, j};
            Position to = _position + dir;
            while (to.is_valid()) {
               if (!is_obstructed(board, to, dir)) {
                  if (Piece::can_move(to, board) && can_move_through_pin(board, dir))
                     return true;
               }
               else
                  break;
               to += dir;
            }
         }
      }
      return false;
   }
}

#endif