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
   Position &Piece::position(void) { return _position; }
   Side Piece::side(void) const { return _side; }


   bool Piece::can_move(const Position &to, const Board &board) const
   {
      if (to == _position)
         return false; // Non sto muovendo il pezzo
      Piece *p_to = board.find_piece(to);
      if (p_to && p_to->side() == _side)
         return false; // Non posso muovermi dove c'è un mio compagno

      return true; // Mancano altri controlli che vengono fatti per ogni singolo pezzo separatamente
   }

   bool Piece::move(const Position &to, Board &board, const PieceType promotion_type)
   {
      if (can_move(to, board))
      {
         Piece::move_forced(to, board, promotion_type);
         return true;
      }
      return false;
   }

   void Piece::move_forced(const Position &to, Board &board, const PieceType promotion_type)
   {
      remove_castling_permissions(board, to);
      board.kill_piece(to);
      board.change_position(this, to);
   }

   void Piece::remove_castling_permissions(Board &board, const Position &to) const
   {
      // Controllo se 'to' è in un angolo della scacchiera
      if ((to.x != 0 && to.x != 7) || (to.y != 0 && to.y != 7))
         return;
      // Controllo se sto mangiando sulla riga iniziale dell'avversario
      const short initial_enemy_row = !_side == WHITE ? 0 : 7;
      if (to.y != initial_enemy_row)
         return;
      // Se sto mangiando una torre rimuovo l'arrocco in quella direzione
      Piece *p = board.find_piece(to);
      if (p && p->type() == ROOK)
         board.lose_castling(!_side, to.x - 1); // to.x-1 è positivo (=6) per la torre 'H', negativo (=-1) per la torre 'A'
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
      std::vector<PieceType> possible_pinning_types{QUEEN, BISHOP, ROOK};
      std::vector<Piece *> possible_pinning_pieces;
      board.get_pieces(!_side, possible_pinning_types, possible_pinning_pieces);
      for (Piece *p : possible_pinning_pieces)
      {
         // Controllo se il pezzo 'p' mi attacca al re
         Direction p_dir = my_king->position() - p->position();
         if (dir.is_same_line(p_dir))
            continue; // Se dir e p_dir sono nella stessa retta, mi sto muovendo nella stessa direzione del pin
         if (!king_dir.is_same_line(p_dir))
            continue; // Se questo pezzo e 'p' non sono allineati al re (dalla stessa direzione) non può pinnarmi
         // Se arrivo qua, il pezzo 'p' è allineato a 'this' e al re e non mi sto muovendo nella direzione del pin (basta controllare che non ci siano altri pezzi in mezzo)
         bool am_i_pinned = true;
         p_dir = p_dir.reduce();
         if (p->type() == ROOK && !p_dir.is_rook_direction())
            continue;
         if (p->type() == BISHOP && !p_dir.is_bishop_direction())
            continue;
         Position curr = p->position() + p_dir;
         while (curr != my_king->position())
         {
            Piece *found = board.find_piece(curr);
            if (found && found != this) {
               am_i_pinned = false;
               break;
            }
            curr += p_dir;
         }
         // Se sono pinnato non posso muovermi
         if (am_i_pinned)
            return false;
      }
      return true;
   }

   bool Piece::can_move_through_check(const Board &board, const Position to) const
   {
      if (!board.is_check(_side))
         return true; // Non è scacco
      std::vector<Piece *> pieces_that_give_check;
      board.whos_giving_check(_side, pieces_that_give_check);
      if (pieces_that_give_check.size() != 1 && type() != KING)
         return false; // Doppio scacco, non può essere bloccato
      if (pieces_that_give_check[0]->position() == to)
         return true; // Mangio chi mi dà scacco
      std::vector<Position> blocking_positions;
      board.cells_to_block_check(pieces_that_give_check, _side, blocking_positions);
      for (const Position &pos : blocking_positions)
      {
         if (pos == to)
            return true; // La posizione in cui mi voglio muovere è una di quelle che blocca lo scacco
      }
      return false; // Non posso bloccare lo scacco
   }

   bool Piece::is_obstructed(const Board &board, const Position to, const Direction dir) const
   {
      Position curr = _position + dir;
      while (curr != to)
      {
         // Se la vista verso 'to' è ostruita ritorno true
         if (board.find_piece(curr) != nullptr)
            return true;
         curr += dir;
      }
      return false;
   }

   bool Piece::is_obstructed_controlling(const Board &board, const Position &to, const Direction dir) const
   {
      Position curr = _position + dir;
      while (curr != to)
      {
         Piece *p = board.find_piece(curr);
         // Se la vista verso 'to' è ostruita ritorno true
         if (p != nullptr && (p->type() != KING || p->side() == _side))
            return true;
         curr += dir;
      }
      return false;
   }

   bool Piece::is_giving_check(const Board &board) const
   {
      Piece *enemy_king = board.get_king(!_side);
      if (is_controlling(board, enemy_king->position()))
         return true;
      return false;
   }

   bool Piece::operator==(const Piece &piece) const
   {
      return _position == piece._position && _side == piece._side && type() == piece.type();
   }

   bool Piece::operator!=(const Piece &piece) const
   {
      return !(*this == piece);
   }

   Side operator!(const Side &side)
   {
      return side == WHITE ? BLACK : WHITE;
   }
}

#endif