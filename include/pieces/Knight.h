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
      bool can_move(const Position &to, const Board &board) const;
      bool can_block_check(const Board &board, const std::vector<Position> cells_to_block_check) const;
      bool is_controlling(const Board &board, const Position &to) const;
      
      bool has_legal_moves_ignore_checks(const Board &board) const;
   };
}

#endif