#ifndef PIECE_CPP
#define PIECE_CPP

#include <algorithm>
#include "Piece.h"
#include "Board.h"

namespace Chess
{
   Piece::Piece(const Position position, const Side side)
       : _position(position), _side(side) {}

   Position Piece::position(void) const { return _position; }
   Side Piece::side(void) const { return _side; }

   bool Piece::can_move(const Position &to, const Board &board, const PieceType promotion_type) const {
      Piece *p_to = board.find_piece(to);
      if (p_to && p_to->side() == _side)
         return false; // Non posso muovermi dove c'è un mio compagno

      return true;
   }

   bool Piece::move(const Position &to, Board &board, const PieceType promotion_type)
   {
      if (can_move(to, board, promotion_type))
      {
         board.kill_piece(to);
         _position = to;
         return true;
      }
      return false;
   }

   bool Piece::can_move_through_pin(const Board &board, const Direction dir) const
   {
      Piece *my_king = board.get_king(_side);
      Direction king_dir = my_king->position() - _position;
      if (!king_dir.is_bishop_direction() && !king_dir.is_rook_direction())
         return true; // Il percorso tra questo pezzo e il re non può essere percorso nè da un alfiere nè da una torre => nessuno può pinnare il pezzo corrente => il pezzo si muove liberamente
      if (king_dir.is_same_line(dir))
         return true; // La destinazione 'dir' ha stessa direzione rispetto al re => il pezzo si muove liberamente in questa direzione
      // Ottengo tutti i pezzi che potrebbero attaccare questo pezzo al re
      std::vector<PieceType> possible_pinning_types = {QUEEN, BISHOP, ROOK};
      std::vector<Piece *> possible_pinning_pieces;
      board.get_pieces(!_side, possible_pinning_types, possible_pinning_pieces);
      for (Piece *p : possible_pinning_pieces)
      {
         // Controllo se il pezzo 'p' mi attacca al re
         Direction p_dir = my_king->position() - p->position();
         if (!dir.is_same_line(p_dir))
            continue;
         // Se arrivo qua, il pezzo 'p' è allineato a 'this' e al re
         bool am_i_pinned = true;
         p_dir = p_dir.reduce();
         Position curr = p->position() + p_dir;
         while (curr != my_king->position())
         {
            Piece *found = board.find_piece(curr);
            if (found && *found != *this)
               am_i_pinned = false;
         }
         // Se sono pinnato non posso muovermi
         if (am_i_pinned)
            return false;
      }
      return true;
   }

   bool Piece::is_obstructed(const Board &board, const Position to, const Direction dir) const {
      Position curr = _position + dir;
      while (curr != to)
      {
         // Se la vista verso 'to' è ostruita ritorno false
         if (board.find_piece(curr) != nullptr)
            return true;
         curr += dir;
      }
      return false;
   }

   bool Piece::operator==(const Piece &piece) const
   {
      return _position == piece._position && _side == piece._side && type() == piece.type();
   }
   
   bool Piece::operator!=(const Piece &piece) const {
      return !(*this == piece);
   }

   bool is_occupied(const Position &pos, const std::vector<Piece *> &pieces)
   {
      auto it = std::find_if(pieces.begin(), pieces.end(), [&pos](const Piece *p)
                             { return p->position() == pos; });
      return it != pieces.end();
   }

   Side operator!(const Side &side)
   {
      return side == WHITE ? BLACK : WHITE;
   }
}

#endif