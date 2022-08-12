#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <ostream>
#include "Position.h"
#include "Move.h"
#include "Piece.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"

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
      // Primi due bit rappresentano il bianco, terzo e quarto bit sono per il nero
      // Primo bit di entrambi i blocchi rappresenta arrocco verso la torre 'A' (verso sinistra per il bianco)
      // Secondo bit di entrambi i blocchi rappresenta arrocco verso la torre 'H' (verso destra per il bianco)
      short _castling_permissions{0b1111};
      // ultima colonna pedone (solo per en passant)
      short _last_pawn_move{-1};
      // regola delle 50 mosse
      short _semimosse_50_move_rule{0};
      // Contatore delle mosse
      short _mossa{1};
      // Elenco di tutte le posizione avvenute nella scacchiera (per controllare la ripetizione di mosse)
      std::vector<std::vector<Piece *>> _positions;

      /* OTTIMIZZAZIONI */
      // Elenco dei pezzi sotto forma di matrice (in modo da poter accedere direttamente ad una posizione)
      Piece ***_pieces_grid;
      // Re
      Piece *_white_king;
      Piece *_black_king;
      std::vector<Move> _moves;
      // // Scacco
      // // TODO Gestisci variabile scacco
      // short _is_check{0b00};
      // // TODO Ottimizza whos_giving_check (aggiungi vector) e cells_to_block_check

   private:
      // Prepara la posizione iniziale riempiendo il vector _pieces
      void initialize(void);
      // Copia i pezzi nella griglia
      void initialize_matrix(void);
      // Cambia il turno
      void toggle_turn(void);
      // Inizializza la partita a partire da FEN (https://it.wikipedia.org/wiki/Notazione_Forsyth-Edwards)
      void initialize_FEN(std::string FEN);
   // // Ritorna true se il pezzo di partenza è ostruito da un altro pezzo cercando di arrivare alla posizione to
   // bool is_obstructed(const Piece &p, const Position &to, const std::vector<Piece> &pieces) const;
   // // Ritorna true il pezzo alla posizione from si può muovere nella posizione to, false altrimenti
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

      static std::string get_pgn_name_type(PieceType type);

   public:
      // Costruttore che inizializza una partita
      Board();
      // Costruttore che inizializza una partita tramite FEN (https://it.wikipedia.org/wiki/Notazione_Forsyth-Edwards)
      Board(std::string FEN);
      // Costruttore di copia
      Board(const Board &other);
      // Distruttore che libera la memoria dai puntatori
      ~Board();

      Board operator=(const Board &&other);

      // Cerca il pezzo ad una certa posizione e lo ritorna
      Piece *find_piece(const Position &position) const;
      // Cerca il re di un certo schieramento e lo ritorna
      King *get_king(const Side side) const;

      // Getter per il turno attuale
      Side turn(void) const;

      // Sposta in modo forzato un pezzo
      void change_position(const Position &from, const Position &to);
      // Sposta in modo forzato un pezzo
      void change_position(Piece *p, const Position &to);

      // Ritorna tutti i pezzi della scacchiera all'interno del vector 'output'
      void get_pieces(std::vector<Piece *> &output) const;

      // Getter per i pezzi di nero e bianco, in base al side passato
      // Copia i pezzi nel vector passato come output
      void get_pieces(Side side, std::vector<Piece *> &output) const;

      // Getter per i pezzi di nero e bianco, in base al side passato e ai filtri dei tipi
      // Copia i pezzi nel vector passato come output
      void get_pieces(Side side, std::vector<PieceType> types, std::vector<Piece *> &output) const;

      // Getter per ottenere la possibilità di fare en passant
      // Ritorna la colonna in cui si è mosso l'ultimo pedone di 2. Se la mossa precedente non si è mosso nessun pedone di 2, ritorna -1
      short get_en_passant_column() const;

      BoardStatus get_board_current_status() const;

      // Metodo che elimina il pezzo alla posizione indicata dal vettore _pieces
      void kill_piece(const Position &position);

      // Sposta un pezzo dalla posizione 'from' alla posizione 'to'
      // Lancia una eccezione, se per qualche motivo la mossa non è valida
      bool move(const Position from, const Position to, const PieceType promotion_type = PieceType::KING);
      bool move(const SimpleMove &move);

      void register_move(const Position from, const Position to, const PieceType from_type, const PieceType to_type, const bool eaten, const PieceType promotion_type);

      /* FUNZIONI PER IL MOVE */
      void update_last_pawn_move(const Piece *p, const Position &from);
      void update_50_move_rule(const Piece *p, const bool eaten);
      void promote(Piece *p, PieceType promotion_type);
      void add_position(std::vector<Piece *> pieces);
      PieceType request_promotion_type() const;
      void create_new_piece(const PieceType type, const Position pos, const Side side);
      /* FINE FUNZIONI PER IL MOVE */

      // Ritorna 'Ending::NONE = 0' se la partita non è finita, altrimenti ritorna il modo in cui è finita la partita
      // Controlla solo il giocatore del side = _turn
      Ending is_game_over(void) const;

      // Stampa la scacchiera
      friend std::ostream &operator<<(std::ostream &os, const Board &b);

   public: // Funzioni riguardanti lo scacco
      // Ritorna true se la posizione corrente ha generato uno scacco al re dello schieramento side
      bool is_check(const Side &side) const;
      // Ritorna true se la posizione corrente ha generato uno scacco al re dello schieramento side
      bool is_check(const Side &side, const std::vector<Piece *> &pieces) const;
      // Ritorna, nell'array output, tutti i pezzi che stanno dando scacco al re dello schieramento side
      std::vector<Piece *> &whos_giving_check(const Side &side, std::vector<Piece *> &output) const;
      // Ritorna, nell'array output, tutti i pezzi che stanno dando scacco al re dello schieramento side
      std::vector<Piece *> &whos_giving_check(const Side &side, const std::vector<Piece *> &pieces, std::vector<Piece *> &output) const;
      // Ritorna, nell'array output, tutte le posizioni che permettono di bloccare uno scacco o di mangiare chi sta dando lo scacco
      std::vector<Position> &cells_to_block_check(const Side &side, std::vector<Position> &output) const;
      // Ritorna, nell'array output, tutte le posizioni che permettono di bloccare uno scacco o di mangiare chi sta dando lo scacco
      std::vector<Position> &cells_to_block_check(const Side &side, const std::vector<Piece *> &pieces, std::vector<Position> &output) const;
      // Ritorna, nell'array output, tutte le posizioni che permettono di bloccare uno scacco, avendo i pezzi che danno scacco
      std::vector<Position> &cells_to_block_check(const Side &side, const std::vector<Piece *> &pieces, const std::vector<Piece *> &pieces_that_give_check, std::vector<Position> &output) const;
      // Ritorna, nell'array output, tutte le posizioni che permettono di bloccare uno scacco, avendo i pezzi che danno scacco
      std::vector<Position> &cells_to_block_check(const std::vector<Piece *> &pieces_that_give_check, const Side &side, std::vector<Position> &output) const;
      // Ritorna, nell'array output, tutte le posizioni che NON sono controllate da side, basandosi su quelle in input
      std::vector<Position> &uncontrolled_positions(const Side &side, const std::vector<Position> &positions_to_check, std::vector<Position> &output) const;

      // Ritorna il numero di mosse giocate fino ad ora
      int mossa() const;
      /* Ritorna una stringa contenente il pgn (https://en.wikipedia.org/wiki/Portable_Game_Notation)
         della partita giocata, a partire dalla posizione iniziale. Utile per esportare una partita */
      std::string get_pgn() const;

      // Ritorna tutte le mosse possibili nella posizione corrente e le ritorna nel vector output_moves
      void get_all_possible_moves(std::vector<SimpleMove>& output_moves) const;

      /*    ARROCCO    */
      // Controlla se il re dello schieramento side può arroccare (ossia non ha perso il diritto di farlo) verso la direzione 'direction' (viene considerato il segno di direction)
      bool can_castle(const Side &side, const short direction) const;
      // Rimuove il diritto di arrocco per un certo schieramento 'side' in una certa direzione 'direction'
      // La direzione può essere positiva (per la torre 'H'), negativa (per la torre 'A') o zero (per entrambe le torri)
      void lose_castling(const Side &side, const short direction);
      
      // Muove forzatamente (senza eseguire controlli) un pezzo !PERICOLOSO!
      void move_forced(const Position from, const Position to, const PieceType promotion_type);
      void move_forced(SimpleMove &move);

      // Annulla l'ultima mossa effettuata
      void undo_move();
   };
}

#endif