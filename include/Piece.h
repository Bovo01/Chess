#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include "Position.h"

namespace Chess
{
   // Dichiaro la classe Board in modo da poterla usare all'interno di Piece
   class Board;

   // Enum che rappresenta lo schieramento di un pezzo
   enum Side
   {
      WHITE = 0,
      BLACK = 1
   };
   // Enum che rappresenta il pezzo stesso
   enum PieceType
   {
      PAWN = 'P',
      KNIGHT = 'C',
      BISHOP = 'A',
      ROOK = 'T',
      QUEEN = 'D',
      KING = 'R'
   };
   // Classe Piece che rappresenta un pezzo in tutte le sue caratteristiche
   class Piece
   {
   protected:
      Position _position;
      Side _side;

   protected:
      // Controlla se il pezzo attuale è attaccato (pinnato) al suo re da un pezzo avversario
      // La direzione 'dir' è la direzione in cui il pezzo si sta muovendo
      // Ritorna true se il pezzo può muoversi verso dir, false altrimenti
      bool can_move_through_pin(const Board &board, const Direction dir) const;
      // Controlla se il pezzo si può mouvere alla destinazione 'to' in caso di scacco da parte dell'avversario
      bool can_move_through_check(const Board &board, const Position to) const;
      // Controlla se c'è qualcuno che ostruisce il percorso dalla posizione attuale alla posizione 'to' seguendo la direzione 'dir'
      bool is_obstructed(const Board &board, const Position to, const Direction dir) const;

   public:
      Piece(const Position position, const Side side);

      // Getter di _position
      Position position(void) const;
      // Getter di _side
      Side side(void) const;
      // Getter di _type
      virtual PieceType type(void) const = 0;

      // Sposta il pezzo dalla posizione corrente a quella passata per parametro, senza eseguire controlli
      virtual bool move(const Position &to, Board &board, const PieceType promotion_type);

      // Controlla se il pezzo attuale può muoversi alla posizione to
      virtual bool can_move(const Position &to, const Board &board, const PieceType promotion_type) const;

      // Controlla se questo pezzo sta dando scacco al re avversario
      virtual bool is_giving_check(const Board &board) const = 0;

      // Controlla se il pezzo corrente sta controllando (ossia "vede") la casa 'to'
      virtual bool is_controlling(const Board &board, const Position &to) const = 0;

      // // Ritorna tutte le posizioni possibili in cui il pezzo corrente si potrebbe muovere,
      // //    senza considerare gli altri pezzi nella scacchiera
      // void get_moves(std::vector<Position> &v) const;

      bool operator==(const Piece &piece) const;
      bool operator!=(const Piece &piece) const;
   };

   bool is_occupied(const Position &pos, const std::vector<Piece *> &pieces);

   Side operator!(const Side &side);
}

#endif