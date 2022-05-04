#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

namespace Chess
{
   class Pawn : public Piece
   {
   public:
      Pawn(const Position position, const Side side);

      PieceType type(void) const;
      bool can_move(const Position &to, const Board &board, const PieceType promotion_type) const;
   };
}

#endif