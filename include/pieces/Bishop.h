#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

namespace Chess
{
   class Bishop : public Piece
   {
   public:
      Bishop(const Position position, const Side side);

      PieceType type(void) const;
      bool can_move(const Position &to, const Board &board) const;
      bool can_block_check(const Board &board, const std::vector<Position> cells_to_block_check) const;
      bool is_controlling(const Board &board, const Position &to) const;
      
      bool has_legal_moves_ignore_checks(const Board &board) const;
      Piece *clone() const;
      bool can_move_ignore_checks(const Position to, const Board &board) const;
      void get_moves_unchecked(std::vector<Position> &positions) const;
   };
}

#endif