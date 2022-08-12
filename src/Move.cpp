#include "Move.h"

namespace Chess
{
   Move::Move(const Position from, const Position to)
       : Move(from, to, false, KING) {}
   Move::Move(const Position from, const Position to, bool eaten)
       : Move(from, to, eaten, KING) {}
   Move::Move(const Position from, const Position to, bool eaten, PieceType promotion)
       : from(from), to(to), eaten(eaten), promotion(promotion) {}

   std::ostream& operator<<(std::ostream& os, const Move move) {
      return os << "{from: " << move.from << ", to: " << move.to << "}";
   }
}