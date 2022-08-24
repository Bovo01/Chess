#ifndef KNIGHT_CPP
#define KNIGHT_CPP

#include "Knight.h"
#include "Board.h"

namespace Chess
{
   Knight::Knight(const Position position, const Side side) : Piece(position, side) {}

   PieceType Knight::type(void) const { return KNIGHT; }

   bool Knight::can_move(const Position &to, const Board &board) const
   {
      if (!Piece::can_move(to, board))
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

   bool Knight::can_block_check(const Board &board, const std::vector<Position> cells_to_block_check) const
   {
      for (const Position to : cells_to_block_check) {
         Direction diff = to - _position;
         if (is_controlling(board, to) && can_move_through_pin(board, diff))
            return true;
      }
      return false;
   }

   bool Knight::is_controlling(const Board &board, const Position &to) const
   {
      Direction diff = to - _position;
      // Controllo se ci posso arrivare
      if (!diff.is_knight_direction())
         return false;
      return true;
   }

   bool Knight::has_legal_moves_ignore_checks(const Board &board) const
   {
      for (short i = -2; i <= 2; i += 4) {
         for (short j = -1; j <= 1; j += 2) {
            // Direzione i, j
            Direction dir = {i, j};
            Position to = _position + dir;
            if (to.is_valid() && Piece::can_move(to, board) && is_controlling(board, to) && can_move_through_pin(board, dir))
               return true;
            // Direzione j, i
            dir = {j, i};
            to = _position + dir;
            if (to.is_valid() && Piece::can_move(to, board) && is_controlling(board, to) && can_move_through_pin(board, dir))
               return true;
         }
      }
      return false;
   }

   Piece *Knight::clone() const
   {
      return new Knight(_position, _side);
   }

   bool Knight::can_move_ignore_checks(const Position to, const Board &board) const
   {
      if (!Piece::can_move(to, board))
         return false;
      // Controllo se il pezzo vede la posizione di destinazione
      if (!is_controlling(board, to))
         return false;
      Direction diff = to - _position;
      // Controllo se sono pinnato al re
      if (!can_move_through_pin(board, diff))
         return false;
      return true;
   }

   void Knight::get_moves_unchecked(std::vector<Position> &positions) const
   {
      for (short i = -1; i <= 1; i += 2) {
         for (short j = -2; j <= 2; j+= 4) {
            Direction dir1 = {i, j};
            Direction dir2 = {j, i};
            Position pos1 = _position + dir1;
            Position pos2 = _position + dir2;
            if (pos1.is_valid())
               positions.push_back(pos1);
            if (pos2.is_valid())
               positions.push_back(pos2);
         }
      }
   }
}

#endif