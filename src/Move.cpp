#include "Move.h"

namespace Chess
{
   SimpleMove::SimpleMove(Position from, Position to)
       : SimpleMove(from, to, QUEEN) {}
   SimpleMove::SimpleMove(Position from, Position to, PieceType promotion)
       : from(from), to(to), promotion(promotion) {}

   Move::Move(const Position from, const Position to, const PieceType eaten_piece, const Position eaten_piece_position, const PieceType promotion, BoardStatus previous_status)
       : from(from), to(to), eaten_piece(eaten_piece), eaten_piece_position(eaten_piece_position), promotion(promotion), previous_status(previous_status) {}

   std::ostream &operator<<(std::ostream &os, const Move move)
   {
      return os << "{from: " << move.from << ", to: " << move.to << "}";
   }
}