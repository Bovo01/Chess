#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

namespace Chess
{
   class Rook : public Piece
   {
   public:
      Rook(const Position position, const Side side);

      PieceType type(void) const;
      bool can_move(const Position &to, const Board &board, const PieceType promotion_type) const;
   };
}

#endif