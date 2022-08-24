#include "Move.h"

namespace Chess
{
   SimpleMove::SimpleMove(Position from, Position to)
       : SimpleMove(from, to, QUEEN) {}
   SimpleMove::SimpleMove(Position from, Position to, PieceType promotion)
       : from(from), to(to), promotion(promotion) {}

   Move::Move(const Position from, const Position to, const PieceType eaten_piece, const Position eaten_piece_position, const PieceType promotion, BoardStatus previous_status)
       : from(from), to(to), eaten_piece(eaten_piece), eaten_piece_position(eaten_piece_position), promotion(promotion), previous_status(previous_status) {}

   std::ostream &operator<<(std::ostream &os, const SimpleMove move)
   {
      return os << "{from: " << move.from << ", to: " << move.to << "}";
   }

   std::ostream &operator<<(std::ostream &os, const Move move)
   {
      return os << "{from: " << move.from << ", to: " << move.to
                << ", tipo di pezzo mangiato: " << (char) move.eaten_piece
                << ", posizione pezzo mangiato: " << move.eaten_piece_position
                << ", tipo di promozione: " << (char) move.promotion
                << ", stato precedente della scacchiera: " << move.previous_status
                << "}";
   }

   std::ostream &operator<<(std::ostream &os, const BoardStatus b_status)
   {
      return os << "{stato arrocco: " << b_status.castling_status
                << ", colonna per en passant: " << b_status.en_passant_column
                << ", semimosse per regola delle 50 mosse: " << b_status.semimosse_50_move_rule
                << "}";
   }
}