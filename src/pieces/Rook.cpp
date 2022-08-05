#ifndef ROOK_CPP
#define ROOK_CPP

#include "Rook.h"
#include "Board.h"

namespace Chess
{
   Rook::Rook(const Position position, const Side side) : Piece(position, side) {}

   PieceType Rook::type(void) const { return ROOK; }

   bool Rook::move(const Position &to, Board &board, const PieceType promotion_type)
   {
      const Position from = _position;
      if (Piece::move(to, board, promotion_type))
      {
         remove_castling_permissions(board, from); // Se la torre si è mossa, rimuovo i permessi di arrocco
         return true;
      }
      return false;
   }

   bool Rook::can_move(const Position &to, const Board &board) const
   {
      if (!Piece::can_move(to, board))
         return false;
      // Controllo se il pezzo vede la posizione di destinazione
      if (!is_controlling(board, to))
         return false;
      Direction dir = (to - _position).reduce();
      // Controllo se sono pinnato al re
      if (!can_move_through_pin(board, dir))
         return false;
      // Controllo se è scacco e se mi posso muovere
      if (!can_move_through_check(board, to))
         return false;
      return true;
   }

   bool Rook::can_block_check(const Board &board, const std::vector<Position> cells_to_block_check) const
   {
      for (const Position to : cells_to_block_check) {
         Direction dir = (to - _position).reduce();
         if (is_controlling(board, to) && can_move_through_pin(board, dir))
            return true;
      }
      return false;
   }

   void Rook::remove_castling_permissions(Board &board, const Position &from) const
   {
      const short initial_row = _side == WHITE ? 0 : 7;
      if (from.y != initial_row)
         return; // La torre non è nella riga iniziale
      if (from.x != 0 && from.x != 7)
         return;                                        // La torre non è nella colonna iniziale
      board.lose_castling(_side, from.x == 0 ? -1 : 1); // Potrei fare 'from.x - 1' (come secondo parametro) ma sarebbe parecchio losco
   }

   bool Rook::is_controlling(const Board &board, const Position &to) const
   {
      Direction diff = to - _position;
      // Controllo se ci posso arrivare
      if (!diff.is_rook_direction())
         return false;
      Direction dir = diff.reduce();
      // Controllo se qualcuno ostruisce il passaggio
      if (is_obstructed(board, to, dir))
         return false;
      return true;
   }

   bool Rook::has_legal_moves_ignore_checks(const Board &board) const
   {
      for (short i = -1; i <= 1; i += 2) {
         // Direzione orizzontale
         Direction dir{i, 0};
         Position to = _position + dir;
         while (to.is_valid()) {
            if (!is_obstructed(board, to, dir)) {
               if (Piece::can_move(to, board) && can_move_through_pin(board, dir))
                  return true;
            }
            else
               break;
            to += dir;
         }
         // Direzione verticale
         dir = {0, i};
         to = _position + dir;
         while (to.is_valid()) {
            if (!is_obstructed(board, to, dir)) {
               if (Piece::can_move(to, board) && can_move_through_pin(board, dir))
                  return true;
            }
            else
               break;
            to += dir;
         }
      }
      return false;
   }
}

#endif