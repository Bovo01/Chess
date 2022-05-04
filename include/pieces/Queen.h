#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

namespace Chess
{
   class Queen : public Piece
   {
   public:
      Queen(const Position position, const Side side);

      PieceType type(void) const;
      bool can_move(const Position &to, const Board &board, const PieceType promotion_type) const;
   };
}

#endif