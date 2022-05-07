#ifndef KING_CPP
#define KING_CPP

#include "King.h"
#include "Board.h"

namespace Chess
{
   King::King(const Position position, const Side side) : Piece(position, side) {}

   PieceType King::type(void) const { return KING; }

   bool King::move(const Position &to, Board &board, const PieceType promotion_type) {
      // TODO king move
   }

   bool King::can_move(const Position &to, const Board &board, const PieceType promotion_type) const
   {
      if (!Piece::can_move(to, board, promotion_type))
         return false;
      // Controllo se il re può raggiungere la posizione 'to', considerando eventuali scacchi e arrocco
      if (!can_reach(board, to))
         return false;
      return true;
   }

   bool King::can_reach(const Board &board, const Position &to) const
   {
      Direction diff = to - _position;
      if (abs(diff.y) > 1)
         return false; // Non posso muovere il re di oltre 1 casa in verticale
      if (abs(diff.x > 2))
         return false; // Non posso muovere il re di oltre 2 case in orizzontale (sono 2 nel caso di arrocco)
      std::vector<Position> positions_to_check;
      positions_to_check.push_back(to);
      if (abs(diff.x == 2))
      {
         if (!board.can_castle(_side, diff.x))
            return false; // Controllo arrocco
         positions_to_check.push_back(_position + diff.reduce());
      }
      std::vector<Position> uncontrolled_positions(positions_to_check.size());
      board.uncontrolled_positions(!_side, positions_to_check, uncontrolled_positions);
      if (uncontrolled_positions.size() == positions_to_check.size())
         return true; // Se le dimensioni sono uguali significa che tutte le posizioni in cui "passa" il re non sono controllate, quindi può arrivare a destinazione
      return false;
   }

   bool King::is_controlling(const Board &board, const Position &to) const
   {
      Direction diff = to - _position;
      if (abs(diff.x) <= 1 && abs(diff.y) <= 1)
         return true;
      return false;
   }

   bool King::is_giving_check(const Board &board) const
   {
      return false; // Il re non può dare scacco visto che non può avvicinarsi ("toccare") al re avversario
   }
}

#endif