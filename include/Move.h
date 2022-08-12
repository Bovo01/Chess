#ifndef MOVE_H
#define MOVE_H

#include "Position.h"
#include "Piece.h"

namespace Chess
{
   struct Move
   {
      Position from, to;
      bool eaten; // true se con questa mossa è stato mangiato un pezzo
      PieceType promotion; // Contiene il pezzo a cui ho promosso o King in caso di non promozione

      Move(const Position from, const Position to);
      Move(const Position from, const Position to, bool eaten);
      Move(const Position from, const Position to, bool eaten, PieceType promotion);
   };

   std::ostream& operator<<(std::ostream& os, const Move move);
}

#endif // MOVE_H