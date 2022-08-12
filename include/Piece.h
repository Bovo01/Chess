#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include "Position.h"

namespace Chess
{
   /* Dichiaro la classe Board in modo da poterla usare all'interno di Piece */

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
      KNIGHT = 'N',
      BISHOP = 'B',
      ROOK = 'R',
      QUEEN = 'Q',
      KING = 'K'
   };
   // Classe Piece che rappresenta un pezzo in tutte le sue caratteristiche
   class Piece
   {
   protected:
      Position _position;
      const Side _side;

   protected:
      // Controlla se il pezzo attuale è attaccato (pinnato) al suo re da un pezzo avversario
      // La direzione 'dir' è la direzione in cui il pezzo si sta muovendo
      // Ritorna true se il pezzo può muoversi verso dir, false altrimenti
      bool can_move_through_pin(const Board &board, const Direction dir) const;
      // Controlla se il pezzo si può mouvere alla destinazione 'to' in caso di scacco da parte dell'avversario
      bool can_move_through_check(const Board &board, const Position to) const;
      // Controlla se c'è qualcuno che ostruisce il percorso dalla posizione attuale alla posizione 'to' seguendo la direzione 'dir'
      bool is_obstructed(const Board &board, const Position to, const Direction dir) const;
      // Controlla se il passaggio verso 'to' è ostruito
      // Caso speciale per quando c'è il re avversario, in cui è sotto scacco è sto controllando le caso dietro di lui
      bool is_obstructed_controlling(const Board &board, const Position &to, const Direction dir) const;

   public:
      Piece(const Position position, const Side side);

      // Getter di _position
      Position position(void) const;
      // Getter non const di _position
      Position &position(void);
      // Getter di _side
      Side side(void) const;
      // Getter di _type
      virtual PieceType type(void) const = 0;

      // Sposta il pezzo dalla posizione corrente a quella passata per parametro, eseguendo tutti i controlli necessari
      virtual bool move(const Position &to, Board &board, const PieceType promotion_type);
      // Sposta il pezzo senza eseguire alcun controllo
      virtual void move_forced(const Position &to, Board &board, const PieceType promotion_type);

      // Controlla se il pezzo attuale può muoversi alla posizione to
      virtual bool can_move(const Position &to, const Board &board) const;

      // Controlla se il pezzo attuale può bloccare lo scacco, ossia muoversi in almeno una delle posizioni in 'cells_to_block_check'
      virtual bool can_block_check(const Board &board, const std::vector<Position> cells_to_block_check) const = 0;

      // Controlla se questo pezzo sta dando scacco al re avversario
      bool is_giving_check(const Board &board) const;

      // Controlla se il pezzo corrente sta controllando (ossia "vede") la casa 'to'
      virtual bool is_controlling(const Board &board, const Position &to) const = 0;

      // Rimuove i permessi d'arrocco allo schieramento avversario se e quando viene mangiata una torre
      void remove_castling_permissions(Board &board, const Position &to) const;

      // Controlla se questo pezzo ha delle mosse legali senza controllare se il re è sotto scacco
      // Il re non ignora gli scacchi
      virtual bool has_legal_moves_ignore_checks(const Board &board) const = 0;

      // // Ritorna tutte le posizioni possibili in cui il pezzo corrente si potrebbe muovere,
      // //    senza considerare gli altri pezzi nella scacchiera
      // void get_moves(std::vector<Position> &v) const;

      // Copia l'oggetto corrente e ne ritorna la copia
      virtual Piece *clone() const = 0;

      // Controlla se il pezzo corrente si può muovere a destinazione senza controllare se è sotto scacco
      virtual bool can_move_ignore_checks(const Position to, const Board &board) const = 0;
      // Ritorna tutte le mosse del pezzo corrente senza controllare il resto della scacchiera
      virtual void get_moves_unchecked(std::vector<Position> &positions) const = 0;

      bool operator==(const Piece &piece) const;
      bool operator!=(const Piece &piece) const;
   };

   Side operator!(const Side &side);
}

#endif