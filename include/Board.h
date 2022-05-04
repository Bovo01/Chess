#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <ostream>
#include "Position.h"
#include "Piece.h"

namespace Chess
{
   enum Ending
   {
      // false se non è finita la partita
      NONE = 0,
      // patte (1 - 5)
      STALEMATE = 1,
      _50_MOVE_RULE,
      INSUFFICIENT_MATERIAL,
      REPETITION,
      // Vittorie (10 - 11)
      // Il bianco ha mattato
      WHITE_CHECKMATE = 10,
      // Il nero ha mattato
      BLACK_CHECKMATE
   };
   // Ritorna in stringa il finale
   const char *ending(Chess::Ending e);

   class Board
   {
   private:
      std::vector<Piece *> _pieces;
      Side _turn{WHITE};

      /* VARIABILI UTILI PER MANTENERE LO STATO DELLA PARTITA */
      // controlli arrocco
      short _castling_permissions{0b1111};
      // ultima colonna pedone (solo per en passant)
      short _last_pawn_move{-1};
      // regola delle 50 mosse
      short _50_move_count{0};
      Side _50_move_start;
      // Elenco di tutte le posizione avvenute nella scacchiera (per controllare la ripetizione di mosse)
      std::vector<std::vector<Piece *>> _positions;

      /* OTTIMIZZAZIONI */
      // Elenco dei pezzi sotto forma di matrice (in modo da poter accedere direttamente ad una posizione)
      Piece ***_pieces_grid;
      // Re
      Piece *_white_king;
      Piece *_black_king;

   private:
      // Prepara la posizione iniziale riempiendo il vector _pieces
      void initialize(void);
      // Copia i pezzi nella griglia
      void initialize_matrix(void);
      // Cambia il turno
      void toggle_turn(void);
      // // Ritorna true se la posizione corrente ha generato uno scacco al re dello schieramento side
      // bool is_check(const Side &side, const std::vector<Piece> &pieces) const;
      // // Ritorna true se il pezzo di partenza è ostruito da un altro pezzo cercando di arrivare alla posizione to
      // bool is_obstructed(const Piece &p, const Position &to, const std::vector<Piece> &pieces) const;
      // Ritorna true il pezzo alla posizione from si può muovere nella posizione to, false altrimenti
      // bool can_move(const Piece &p_from, const Position &to) const;
      // Controlla se il pezzo può essere una promozione valida
      bool is_valid_promotion_type(const PieceType &type);

      /* CONTROLLO FINALI */
      // Controlla se la posizione per il lato side è scacco matto o stallo o se è una posizione giocabile
      Ending is_checkmate_stalemate(const Side &side) const;
      // Controlla se la posizione non può essere vinta da nessuno a causa di materiale insufficiente
      bool is_insufficient_material() const;
      // Controlla se la posizione corrente è stata già rivista 3 volte durante la partita
      bool is_repetition() const;

   public:
      // Costruttore che inizializza una partita
      Board();

      // Eccezione per indicare una mossa invalida o illegale
      class IllegalMoveException
      {
      };

      // Cerca il pezzo ad una certa posizione e lo ritorna
      Piece *find_piece(const Position &position) const;
      // Cerca il re di un certo schieramento e lo ritorna
      Piece *get_king(const Side side) const;

      // Getter per il turno attuale
      Side turn(void) const;

      // Getter per i pezzi di nero e bianco, in base al side passato
      // Copia i pezzi nel vector passato come output
      void get_pieces(Side side, std::vector<Piece *> &output) const;

      // Getter per i pezzi di nero e bianco, in base al side passato e ai filtri dei tipi
      // Copia i pezzi nel vector passato come output
      void get_pieces(Side side, std::vector<PieceType> types, std::vector<Piece *> &output) const;

      // Metodo che elimina il pezzo alla posizione indicata dal vettore _pieces
      void kill_piece(const Position &position);

      // Sposta un pezzo dalla posizione 'from' alla posizione 'to'
      // Lancia una eccezione, se per qualche motivo la mossa non è valida
      void move(const Position from, const Position to, const PieceType promotion_type = PieceType::KING);

      // Ritorna 'Ending::NONE = 0' se la partita non è finita, altrimenti ritorna il modo in cui è finita la partita
      // Controlla solo il giocatore del side = _turn
      Ending is_game_over(void) const;

      // Stampa la scacchiera
      friend std::ostream &operator<<(std::ostream &os, const Board &b);
   };
}

#endif