#ifndef KING_CPP
#define KING_CPP

#include "King.h"
#include "Board.h"

namespace Chess
{
   King::King(const Position position, const Side side) : Piece(position, side) {}

   PieceType King::type(void) const { return KING; }

   bool King::move(const Position &to, Board &board, const PieceType promotion_type)
   {
      const Position from = _position;
      if (Piece::move(to, board, promotion_type))
      {
         board.lose_castling(_side, 0); // Se muovo il re, gli annullo anche l'arrocco da entrambe le direzioni
         // In caso di arrocco sposto anche la torre
         if (abs(from.x - to.x) == 2)
         {
            const short initial_row = _side == WHITE ? 0 : 7;
            const Direction king_dir = to - from;
            const short rook_col = king_dir.x > 0 ? 7 : 0;
            Piece *rook = board.find_piece({rook_col, initial_row});
            board.change_position(rook, to + king_dir.reduce().opposite());
         }
         return true;
      }
      return false;
   }

   bool King::can_move(const Position &to, const Board &board) const
   {
      if (!Piece::can_move(to, board))
         return false;
      // Controllo se il re può raggiungere la posizione 'to', considerando eventuali scacchi e arrocco
      if (!can_reach(board, to))
         return false;
      return true;
   }

   bool King::can_block_check(const Board &board, const std::vector<Position> cells_to_block_check) const
   {
      return false;
   }

   bool King::can_reach(const Board &board, const Position &to) const
   {
      Direction diff = to - _position;
      if (abs(diff.y) > 1)
         return false; // Non posso muovere il re di oltre 1 casa in verticale
      if (abs(diff.x > 2))
         return false; // Non posso muovere il re di oltre 2 case in orizzontale (sono 2 nel caso di arrocco)
      std::vector<Position> positions_to_check;
      positions_to_check.push_back(to);
      if (abs(diff.x) == 2)
      {
         if (diff.y != 0)
            return false;
         if (!board.can_castle(_side, diff.x))
            return false; // Controllo arrocco
         Position mid_position = _position + diff.reduce();
         if (board.find_piece(mid_position) != nullptr)
            return false; // Lo spazio tra il re e la destinazione deve essere libero
         positions_to_check.push_back(mid_position);
         positions_to_check.push_back(_position);
      }
      std::vector<Position> uncontrolled_positions;
      board.uncontrolled_positions(!_side, positions_to_check, uncontrolled_positions);
      if (uncontrolled_positions.size() == positions_to_check.size())
         return true; // Se le dimensioni sono uguali significa che tutte le posizioni in cui "passa" il re non sono controllate, quindi può arrivare a destinazione
      return false;
   }

   bool King::is_controlling(const Board &board, const Position &to) const
   {
      Direction diff = to - _position;
      if (abs(diff.x) <= 1 && abs(diff.y) <= 1)
         return true;
      return false;
   }

   bool King::is_giving_check(const Board &board) const
   {
      return false; // Il re non può dare scacco visto che non può avvicinarsi ("toccare") al re avversario
   }

   void King::get_moves(const Board &board, std::vector<Position> &output) const
   {
      output.clear();
      for (short i = -1; i <= 1; i++) {
         for (short j = -1; j <= 1; j++) {
            const Position new_pos = _position.move(i, j);
            if (new_pos.is_valid())
               output.push_back(new_pos);
         }
      }
      if (board.can_castle(_side, 1))
         output.push_back(_position.move(2, 0));
      if (board.can_castle(_side, -1))
         output.push_back(_position.move(-2, 0));
   }

   bool King::has_legal_moves_ignore_checks(const Board &board) const {
      std::vector<Piece *> enemy_pieces;
      board.get_pieces(!_side, enemy_pieces);
      for (short i = -1; i <= 1; i++) {
         for (short j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)
               continue; // Ignoro la posizione del re
            const Position to = _position.move(i, j);
            if (to.is_valid()) {
               Piece *p = board.find_piece(to);
               if (p && p->side() == _side)
                  continue; // La posizione di arrivo è occupata da un pezzo alleato
               bool controlled = false;
               for (const Piece *enemy_piece : enemy_pieces) {
                  if (enemy_piece->is_controlling(board, to)) {
                     controlled = true;
                     break;
                  }
               }
               if (!controlled)
                  return true; // Se la casa non è controllata da nessun pezzo avversario, il re ha mosse legali
            }
         }
      }
      return false;
   }

   Piece *King::clone() const
   {
      return new King(_position, _side);
   }
}

#endif