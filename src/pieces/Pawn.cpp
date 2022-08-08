#ifndef PAWN_CPP
#define PAWN_CPP

#include "Pawn.h"
#include "Board.h"

namespace Chess
{
   Pawn::Pawn(const Position position, const Side side) : Piece(position, side)
   {
      _pawn_direction = side == WHITE ? 1 : -1;
   }

   PieceType Pawn::type(void) const { return PAWN; }

   bool Pawn::move(const Position &to, Board &board, const PieceType promotion_type)
   {
      if (can_move(to, board))
      {
         remove_castling_permissions(board, to);
         Direction diff = to - _position;
         Piece *p_to = board.find_piece(to);
         if (diff.is_bishop_direction() && p_to == nullptr)
            board.kill_piece({to.x, _position.y}); // mangio in en passant
         else
            board.kill_piece(to); // mangio normalmente
         board.change_position(_position, to);
         return true;
      }
      return false;
   }

   bool Pawn::can_move(const Position &to, const Board &board) const
   {
      if (!Piece::can_move(to, board))
         return false;
      // Controllo se può raggiungere la posizione di destinazione
      if (!can_reach(board, to))
         return false;
      // Controllo se sono pinnato al re
      if (!can_move_through_pin(board, to - _position))
         return false;
      // Controllo se è scacco e se mi posso muovere
      if (!can_move_through_check(board, to))
         return false;
      return true;
   }

   bool Pawn::can_block_check(const Board &board, const std::vector<Position> cells_to_block_check) const
   {
      for (const Position to : cells_to_block_check) {
         Direction diff = to - _position;
         if (can_reach(board, to) && can_move_through_pin(board, diff))
            return true;
      }
      return false;
   }

   bool Pawn::can_reach(const Board &board, const Position &to) const
   {
      Direction diff = to - _position;
      if (diff.y == 0)
         return false; // Il pedone DEVE muoversi in avanti
      if (abs(diff.x) > 1)
         return false; // Il pedone non si può muovere di 2 o più orizzontalmente
      if (diff.y * _pawn_direction < 0)
         return false; // Se la direzione diff.y ha segno discorde alla direzione del pedone => il pedone sta tornando indietro => non possibile
      if (diff.x == 0)
      {
         // Mi sto muovendo in avanti
         if (abs(diff.y) > 1)
         {
            if (abs(diff.y) > 2)
               return false; // Non posso muovere il pedone di 3 o più
            // Controllo se si può muovere di 2 (quindi se è nella posizione iniziale)
            const short initial_position = _side == WHITE ? 1 : 6;
            if (_position.y != initial_position)
               return false; // Non sono nella posizione iniziale => non posso muovermi di 2
         }
         // Controllo se c'è un pezzo nella posizione finale
         const Piece *destination_piece = board.find_piece(to);
         if (destination_piece != nullptr)
            return false; // Se c'è un pezzo nella posizione finale non posso avanzare il pedone
      }
      else
      {
         // Sto mangiando in diagonale
         if (abs(diff.y) > 1)
            return false; // Se mangia si può muovere di 1 in avanti
         const Piece *killed_enemy = board.find_piece(to);
         if (killed_enemy == nullptr && !is_en_passant(board, to))
            return false; // Non sto mangiando nessuno
      }
      return true;
   }

   bool Pawn::is_en_passant(const Board &board, const Position &to) const
   {
      const short en_passant_row = _side == WHITE ? 4 : 3;
      if (_position.y != en_passant_row)
         return false; // Il pedone non è nella riga giusta per eseguire en passant
      const short en_passant_column = board.get_en_passant_column();
      if (to.x != en_passant_column)
         return false; // L'en passant non è possibile nella colonna in cui si sta spostando il pedone
      return true;
   }

   bool Pawn::is_controlling(const Board &board, const Position &to) const
   {
      Direction diff = to - _position;
      if (_pawn_direction != diff.y)
         return false; // Se il pezzo NON si sta muovendo di 1 nella sua direzione non sta controllando 'to'
      if (abs(diff.x) == 1)
         return true; // Se il pezzo si muove anche orizzontalmente di 1 sta controllando la casella
      return false;
   }

   bool Pawn::has_legal_moves_ignore_checks(const Board &board) const
   {
      // Push di 1 (non serve controllare il push di 2 perché se è possibile spostare il pedone di una casa è possibile farlo anche di 2)
      Position to = _position.move(0, _pawn_direction);
      {
         if (Piece::can_move(to, board) && can_reach(board, to) && can_move_through_pin(board, to - _position))
            return true;
      }
      // Mangia a destra (verso 'H')
      to = _position.move(1, _pawn_direction);
      if (to.is_valid()) {
         if (Piece::can_move(to, board) && can_reach(board, to) && can_move_through_pin(board, to - _position))
            return true;
      }
      // Mangia a sinistra (verso 'A')
      to = _position.move(-1, _pawn_direction);
      if (to.is_valid()) {
         if (Piece::can_move(to, board) && can_reach(board, to) && can_move_through_pin(board, to - _position))
            return true;
      }
      return false;
   }

   Piece *Pawn::clone() const
   {
      return new Pawn(_position, _side);
   }
}

#endif