#ifndef KING_H
#define KING_H

#include "Piece.h"

namespace Chess
{
   class King : public Piece
   {
   public:
      King(const Position position, const Side side);

      // Implementazione funzioni della super classe
      PieceType type(void) const;
      bool can_move(const Position &to, const Board &board, const PieceType promotion_type) const;
      bool is_controlling(const Board &board, const Position &to) const;
      bool is_giving_check(const Board &board) const;
      // Modifica move TODO
      bool move(const Position &to, Board &board, const PieceType promotion_type);

      // Funzioni aggiuntive per pedone
      bool can_reach(const Board &board, const Position &to) const;
   };
}

#endif