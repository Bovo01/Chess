#ifndef KING_H
#define KING_H

#include "Piece.h"

namespace Chess
{
   class King : public Piece
   {
   public:
      King(const Position position, const Side side);

      PieceType type(void) const;
      bool can_move(const Position &to, const Board &board, const PieceType promotion_type) const;
   };
}

#endif