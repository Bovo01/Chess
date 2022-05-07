#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

namespace Chess
{
   class Knight : public Piece
   {
   public:
      Knight(const Position position, const Side side);

      PieceType type(void) const;
      bool can_move(const Position &to, const Board &board, const PieceType promotion_type) const;
      bool is_controlling(const Board &board, const Position &to) const;
      bool is_giving_check(const Board &board) const;
   };
}

#endif