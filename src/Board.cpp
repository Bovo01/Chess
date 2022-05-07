#ifndef BOARD_CPP
#define BOARD_CPP

#include <algorithm>
#include "Board.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"

namespace Chess
{
   const char *ending(Ending e)
   {
      switch (e)
      {
      case STALEMATE:
         return "Patta per stallo";
      case _50_MOVE_RULE:
         return "Patta per regola delle 50 mosse";
      case INSUFFICIENT_MATERIAL:
         return "Patta per materiale insufficiente";
      case REPETITION:
         return "Patta per ripetizione";
      case WHITE_CHECKMATE:
         return "SCACCOMATTO! Vince il bianco";
      case BLACK_CHECKMATE:
         return "SCACCOMATTO! Vince il nero";
      default:
         return "La partita non e' finita";
      }
   }

   /*       COSTRUTTORI       */

   Board::Board()
       : _pieces(std::vector<Piece *>())
   {
      _pieces.reserve(32);
      initialize();
      initialize_matrix();
   }

   Board::Board(const Board &other)
   {
      // TODO Costruttore di copia, copia profonda di _positions, _pieces, _pieces_grid
   }
   /*       DISTRUTORI           */

   Board::~Board()
   {
      // Elimino tutti i pezzi
      for (Piece *p : _pieces)
      {
         delete p;
      }
      // Elimino la griglia
      for (Piece ***row = _pieces_grid; row != _pieces_grid + 8; ++row)
      {
         delete[] row[0];
      }
      delete[] _pieces_grid;
      // Elimino l'elenco delle posizioni
      for (std::vector<Piece *> position : _positions)
      {
         for (Piece *p : position)
         {
            delete p;
         }
      }
   }
   /*       METODI PRIVATI       */

   void Board::initialize(void)
   {
      // Pezzi del bianco
      _pieces.push_back(new Rook{"A1", WHITE});
      _pieces.push_back(new Knight{"B1", WHITE});
      _pieces.push_back(new Bishop{"C1", WHITE});
      _pieces.push_back(new Queen{"D1", WHITE});
      _white_king = new King{"E1", WHITE};
      _pieces.push_back(_white_king);
      _pieces.push_back(new Bishop{"F1", WHITE});
      _pieces.push_back(new Knight{"G1", WHITE});
      _pieces.push_back(new Rook{"H1", WHITE});
      // Pedoni
      for (short i = 0; i < 8; i++)
      {
         _pieces.push_back(new Pawn{{i, 1}, WHITE});
         _pieces.push_back(new Pawn{{i, 6}, BLACK});
      }
      // Pezzi del nero
      _pieces.push_back(new Rook{"A8", BLACK});
      _pieces.push_back(new Knight{"B8", BLACK});
      _pieces.push_back(new Bishop{"C8", BLACK});
      _pieces.push_back(new Queen{"D8", BLACK});
      _black_king = new King{"E8", BLACK};
      _pieces.push_back(_black_king);
      _pieces.push_back(new Bishop{"F8", BLACK});
      _pieces.push_back(new Knight{"G8", BLACK});
      _pieces.push_back(new Rook{"H8", BLACK});

      _positions.push_back(_pieces);
   }

   void Board::initialize_matrix(void)
   {
      _pieces_grid = new Piece **[8];
      for (int i = 0; i < 8; i++)
      {
         _pieces_grid[i] = new Piece *[8];
         for (int j = 0; j < 8; j++)
            _pieces_grid[i][j] = nullptr;
      }
      for (Piece *p : _pieces)
      {
         Position pos = p->position();
         _pieces_grid[pos.y][pos.x] = p;
      }
   }

   void Board::toggle_turn(void)
   {
      _turn = !_turn;
   }

   bool Board::is_valid_promotion_type(const PieceType &type)
   {
      switch (type)
      {
      case KING:
      case PAWN:
         return false;
      case KNIGHT:
      case BISHOP:
      case ROOK:
      case QUEEN:
         return true;
      default:
         return false;
      }
   }

   void Board::kill_piece(const Position &position)
   {
      Piece *p = find_piece(position);
      if (p)
      {
         _pieces.erase(std::find_if(_pieces.begin(), _pieces.end(), [&position](Piece *p)
                                    { return p->position() == position; }));
         delete p;
      }
   }

   bool Board::is_check(const Side &side) const
   {
      return is_check(side, _pieces);
   }

   bool Board::is_check(const Side &side, const std::vector<Piece *> &pieces) const
   {
      for (Piece *p : pieces)
      {
         if (p->side() != side && p->is_giving_check(*this))
            return true;
      }
      return false;
   }

   std::vector<Piece *> &Board::whos_giving_check(const Side &side, std::vector<Piece *> &output) const
   {
      return whos_giving_check(side, _pieces, output);
   }

   std::vector<Piece *> &Board::whos_giving_check(const Side &side, const std::vector<Piece *> &pieces, std::vector<Piece *> &output) const
   {
      // Svuoto l'output in caso ci sia qualche disturbo
      output.clear();
      for (Piece *p : pieces)
      {
         if (p->side() != side && p->is_giving_check(*this))
            output.push_back(p);
      }
      return output;
   }

   std::vector<Position> &Board::cells_to_block_check(const Side &side, std::vector<Position> &output) const
   {
      return cells_to_block_check(side, _pieces, output);
   }

   std::vector<Position> &Board::cells_to_block_check(const Side &side, const std::vector<Piece *> &pieces, std::vector<Position> &output) const
   {
      // Svuoto l'output in caso ci sia qualche disturbo
      output.clear();
      std::vector<Piece *> pieces_that_give_check;
      whos_giving_check(side, pieces, pieces_that_give_check);
      return cells_to_block_check(side, pieces, pieces_that_give_check, output);
   }

   std::vector<Position> &Board::cells_to_block_check(const Side &side, const std::vector<Piece *> &pieces, const std::vector<Piece *> &pieces_that_give_check, std::vector<Position> &output) const
   {
      if (pieces_that_give_check.size() != 1)
         return output; // Se ci sono più pezzi che fanno scacco o non è scacco => lo scacco non si può bloccare
      Piece *checking_piece = pieces_that_give_check[0];
      switch (checking_piece->type())
      {
      case PAWN:
      case KNIGHT:
         return output; // Lo scacco non può essere bloccato
      case BISHOP:
      case ROOK:
      case QUEEN:
      {
         Piece *king = get_king(side);
         Direction king_dir = (king->position() - checking_piece->position()).reduce();
         Position curr = checking_piece->position() + king_dir;
         while (curr != king->position())
         {
            output.push_back(curr);
            curr += king_dir;
         }
         return output;
      }
      case KING:
         throw "Si è rotto qualcosa";
      default:
         throw "Si è rotto qualcosa";
      }
   }

   std::vector<Position> &Board::cells_to_block_check(const std::vector<Piece *> &pieces_that_give_check, const Side &side, std::vector<Position> &output) const
   {
      return cells_to_block_check(side, _pieces, pieces_that_give_check, output);
   }

   std::vector<Position> &Board::uncontrolled_positions(const Side &side, const std::vector<Position> &positions_to_check, std::vector<Position> &output) const
   {
      // Svuoto l'output in caso ci sia qualche disturbo
      output.clear();
      output.reserve(positions_to_check.size());
      // Copio tutte le posizioni da controllare in output
      for (const Position pos : positions_to_check)
         output.push_back(pos);
      for (Piece *p : _pieces)
      {
         if (p->side() != side)
            continue;
         // Quando le posizioni sono tutte controllate da side le ritorno
         if (output.size() == 0)
            return output;
         // Controllo tutte le posizioni ancora possibili
         for (int i = output.size() - 1; i >= 0; i--)
         {
            if (p->is_controlling(*this, output[i]))
               output.erase(output.begin() + i); // Se la posizione corrente è controllata la elimino dal vector output
         }
      }
      return output;
   }

   bool Board::can_castle(const Side &side, const short direction) const
   {
      short filter = 0b0;
      if (direction < 0)
         filter = 0b1000; // Torre 'A'
      else
         filter = 0b0100; // Torre 'H'
      if (side == BLACK)
         filter = filter >> 2; // Sposto i due bit a destra
      return (filter & _castling_permissions) != 0;
   }

   /*       GETTERS       */

   Side Board::turn(void) const
   {
      return _turn;
   }

   void Board::get_pieces(Side side, std::vector<Piece *> &output) const
   {
      // Svuota vector output
      output.clear();
      for (Piece *p : _pieces)
      {
         if (p->side() == side)
            output.push_back(p);
      }
   }

   void Board::get_pieces(Side side, std::vector<PieceType> types, std::vector<Piece *> &output) const
   {
      // Svuota vector output
      output.clear();
      for (Piece *p : _pieces)
      {
         if (p->side() == side && std::find_if(types.begin(), types.end(), [&p](const PieceType pt)
                                               { return pt == p->type(); }) != types.end())
            output.push_back(p);
      }
   }

   short Board::get_en_passant_column() const
   {
      return _last_pawn_move;
   }

   /* CONTROLLO FINALI */

   bool Board::is_insufficient_material() const
   {
      short white_bishop_color = -1,
            black_bishop_color = -1;
      bool has_white_knight = false,
           has_black_knight = false;

      for (Piece *p : _pieces)
      {
         switch (p->type())
         {
         case KING:
            break;
         case PAWN:
         case ROOK:
         case QUEEN:
            // C'è ancora abbastanza materiale
            return false;
         case BISHOP:
         {
            short bishop_color = (p->position().x + p->position().y) % 2;
            if (p->side() == WHITE)
            {
               // Alfiere e cavallo bastano per mattare
               if (has_white_knight)
                  return false;

               // Se non è stato impostato, imposto il colore dell'alfiere
               if (white_bishop_color == -1)
                  white_bishop_color = bishop_color;
               else if (white_bishop_color != bishop_color)
                  /* se c'è un alfiere del colore opposto a questo, la partita non è ancora patta */
                  return false;
            }
            else
            {
               // Alfiere e cavallo bastano per mattare
               if (has_black_knight)
                  return false;

               // Se non è stato impostato, imposto il colore dell'alfiere
               if (black_bishop_color == -1)
                  black_bishop_color = bishop_color;
               else if (black_bishop_color != bishop_color)
                  /* se c'è un alfiere del colore opposto a questo, la partita non è ancora patta */
                  return false;
            }
         }
         break;
         case KNIGHT:
            if (p->side() == WHITE)
            {
               // Due cavalli / Un cavallo e un alfiere  bastano per mattare
               if (has_white_knight || white_bishop_color != -1)
                  return false;

               has_white_knight = true;
            }
            else
            {
               // Due cavalli / Un cavallo e un alfiere  bastano per mattare
               if (has_black_knight || black_bishop_color != -1)
                  return false;

               has_black_knight = true;
            }
         }
      }
      return true;
   }
   // TODO Fixa codice finali
   // Ending Board::is_checkmate_stalemate(const Side &side) const
   // {
   //    // Controllo se il re ha mosse legali
   //    Piece* king = *std::find_if(_pieces.begin(), _pieces.end(), [&side](Piece *p)
   //                                     { return p->type() == KING && p->side() == side; });
   //    std::vector<Position> moves;
   //    king.get_moves(moves);
   //    // Controllo le mosse del re all'inizio per una questione di ottimizzazione
   //    for (const Position &pos : moves)
   //    {
   //       // Se il re ha mosse legali la partita non è finita
   //       if (can_move(king, pos))
   //          return NONE;
   //    }
   //    // Controllo se ci sono mosse legali tra tutti pezzi di side
   //    for (const Piece &p : _pieces)
   //    {
   //       if (p.type() == KING || p.side() != side)
   //          continue;

   //       std::vector<Position> moves;
   //       p.get_moves(moves);
   //       for (const Position &pos : moves)
   //       {
   //          // Se il pezzo ha mosse legali la partita non è finita
   //          if (can_move(p, pos))
   //          {
   //             can_move(p, pos);
   //             return NONE;
   //          }
   //       }
   //    }

   //    // Se non ci sono mosse legali è scacco matto o stallo

   //    if (is_check(side, _pieces)) /* Scacco matto */
   //    {
   //       if (side == WHITE)
   //          return BLACK_CHECKMATE; // Il nero ha mattato il bianco
   //       return WHITE_CHECKMATE;    // Il bianco ha mattato il nero
   //    }
   //    // Altrimenti stallo
   //    return STALEMATE;
   // }

   // bool Board::is_repetition(void) const
   // {
   //    for (int i = 0; i < _positions.size(); i++)
   //    {
   //       short repetitions = 1;
   //       const std::vector<Piece> &curr_pos = _positions[i];
   //       for (int j = i + 1; j < _positions.size(); j++)
   //       {
   //          const std::vector<Piece> &next_pos = _positions[j];
   //          if (curr_pos.size() != next_pos.size())
   //          {
   //             continue;
   //          }
   //          bool found = true;
   //          for (const Piece &p : curr_pos)
   //          {
   //             if (std::find_if(next_pos.begin(), next_pos.end(), [&p](const Piece &other)
   //                              { return p == other; }) == next_pos.end())
   //             {
   //                found = false;
   //                break;
   //             }
   //          }
   //          if (found)
   //          {
   //             repetitions++;
   //          }
   //       }
   //       if (repetitions >= 3)
   //          return true;
   //    }
   //    return false;
   // }

   /*       OVERLOAD OPERATORI       */

   std::ostream &operator<<(std::ostream &os, const Board &b)
   {
      for (short i = 0; i < 8; i++)
      {
         os << 8 - i << ' ';
         for (short j = 0; j < 8; j++)
         {
            Piece *p = b._pieces_grid[7 - i][j];
            if (p)
               os << (p->side() == WHITE ? (char)p->type() : (char)(p->type() + 32));
            else
               os << ' ';
         }
         os << std::endl;
      }
      os << std::endl;
      os << "  ";
      for (char c = 'A'; c <= 'H'; c++)
         os << c;
      return os;
   }
   /*       FUNZIONALITA' DI GIOCO       */

   Piece *Board::find_piece(const Position &position) const
   {
      return _pieces_grid[position.y][position.x];
   }

   Piece *Board::get_king(const Side side) const
   {
      return side == WHITE ? _white_king : _black_king;
   }

   void Board::move(const Position from, const Position to, const PieceType promotion_type)
   {
      Piece *p_from = find_piece(from);
      if (p_from->move(to, *this, promotion_type))
      {
         // TODO Gestisci cose (en passant, arrocco, aggiornamento _last_pawn_move, ...)
         toggle_turn();
         // TODO Sposta in _pieces_grid
      }
   }

   // Ending Board::is_game_over(void) const
   // {
   //    if (_50_move_count >= 50)
   //       return _50_MOVE_RULE;
   //    if (is_insufficient_material())
   //       return INSUFFICIENT_MATERIAL;
   //    if (is_repetition())
   //       return REPETITION;
   //    return is_checkmate_stalemate(_turn);
   // }
}

#endif