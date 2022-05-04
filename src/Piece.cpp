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

   bool Piece::move(const Position &to, const Board &board, const PieceType promotion_type)
   {
      if (can_move(to, board, promotion_type))
      {
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
         Direction p_dir = p->position() - _position;
         if (king_dir.is_same_line(p_dir))
            return false;
      }
      return true;
   }

   bool Piece::operator==(const Piece &piece) const
   {
      return _position == piece._position && _side == piece._side && type() == piece.type();
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