#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

namespace Chess
{
   class Pawn : public Piece
   {
   private:
      short _pawn_direction;

   public:
      Pawn(const Position position, const Side side);

      /* Implementazione funzioni della super classe */

      PieceType type(void) const;
      bool can_move(const Position &to, const Board &board) const;
      bool can_block_check(const Board &board, const std::vector<Position> cells_to_block_check) const;
      bool is_controlling(const Board &board, const Position &to) const;

      bool has_legal_moves_ignore_checks(const Board &board) const;
      Piece *clone() const;
      bool can_move_ignore_checks(const Position to, const Board &board) const;
      void get_moves_unchecked(std::vector<Position> &positions) const;

      /* Modifica move */

      bool move(const Position &to, Board &board, const PieceType promotion_type);
      void move_forced(const Position &to, Board &board, const PieceType promotion_type);

      /* Funzioni aggiuntive per pedone */

      bool can_reach(const Board &board, const Position &to) const;
      bool is_en_passant(const Board &board, const Position &to) const;
   };
}

#endif