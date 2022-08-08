#ifndef BOARD_CPP
#define BOARD_CPP

#include <iostream>
#include <algorithm>
#include "Board.h"

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

   // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
   Board::Board(std::string FEN)
       : _pieces(std::vector<Piece *>())
   {
      initialize_matrix();
      initialize_FEN(FEN);
   }


   Board::Board(const Board &other)
   {
      for (Piece *p : _pieces)
         if (p)
            delete p;
      _pieces.clear();
      _pieces.reserve(other._pieces.size());
      for (const Piece *p : other._pieces) {
         if (p->type() == KING) {
            if (p->side() == WHITE) {
               _white_king = p->clone();
               _pieces.push_back(_white_king);
            } else {
               _black_king = p->clone();
               _pieces.push_back(_black_king);
            }
         } else
            _pieces.push_back(p->clone());
      }
      _turn = other._turn;
      _castling_permissions = other._castling_permissions;
      _last_pawn_move = other._last_pawn_move;
      _semimosse_50_move_rule = other._semimosse_50_move_rule;
      _mossa = other._mossa;
      for (std::vector<Piece *> positions : _positions) {
         for (Piece *p : positions)
            if (p)
               delete p;
      }
      _positions.clear();
      _positions.reserve(other._positions.size());
      for (const std::vector<Piece *> positions : other._positions) {
         std::vector<Piece *> pieces;
         pieces.reserve(positions.size());
         for (const Piece *piece : positions) {
            pieces.push_back(piece->clone());
         }
         _positions.push_back(pieces);
      }
      initialize_matrix(); // Copia automaticamente _pieces qua dentro
   }
   /*       DISTRUTORI           */

   Board::~Board()
   {
      // Elimino tutti i pezzi
      for (Piece *p : _pieces)
         if (p)
            delete p;
      // Elimino la griglia
      for (int i = 0; i < 8; i++)
      {
         if (_pieces_grid[i])
            delete[] _pieces_grid[i];
      }
      delete[] _pieces_grid;
      _pieces_grid = NULL;
      // Elimino l'elenco delle posizioni
      for (std::vector<Piece *> position : _positions)
      {
         for (Piece *p : position)
            if (p)
               delete p;
      }
   }
   
   Board Board::operator=(const Board &&other) {
      for (Piece *p : _pieces)
         if (p)
            delete p;
      _pieces.clear();
      _pieces.reserve(other._pieces.size());
      for (const Piece *p : other._pieces) {
         if (p->type() == KING) {
            if (p->side() == WHITE) {
               _white_king = p->clone();
               _pieces.push_back(_white_king);
            } else {
               _black_king = p->clone();
               _pieces.push_back(_black_king);
            }
         } else
            _pieces.push_back(p->clone());
      }
      _turn = other._turn;
      _castling_permissions = other._castling_permissions;
      _last_pawn_move = other._last_pawn_move;
      _semimosse_50_move_rule = other._semimosse_50_move_rule;
      _mossa = other._mossa;
      for (std::vector<Piece *> positions : _positions) {
         for (Piece *p : positions)
            if (p)
               delete p;
      }
      _positions.clear();
      _positions.reserve(other._positions.size());
      for (const std::vector<Piece *> positions : other._positions) {
         std::vector<Piece *> pieces;
         pieces.reserve(positions.size());
         for (const Piece *piece : positions) {
            pieces.push_back(piece->clone());
         }
         _positions.push_back(pieces);
      }
      initialize_matrix(); // Copia automaticamente _pieces qua dentro
      return *this;
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

      std::vector<Piece *> pieces;
      pieces.reserve(_pieces.size());
      for (const Piece *p : _pieces) {
         pieces.push_back(p->clone());
      }
      _positions.push_back(pieces);
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

   void Board::initialize_FEN(std::string FEN)
   {
      // Imposto la posizione di tutti i pezzi
      short x = 0, y = 7;
      for (char c : FEN) {
         if (c == ' ') {
            break;
         } else if (c == '/') {
            y--;
            x = 0;
         } else if (isdigit(c)) {
            x += c - '0';
         } else if (isalpha(c)) {
            create_new_piece(static_cast<PieceType>(toupper(c)), {x, y}, isupper(c) ? WHITE : BLACK);
            if (c == 'k') {
               _black_king = find_piece({x, y});
            } else if (c == 'K') {
               _white_king = find_piece({x, y});
            }
            x++;
         } else {
            throw "Invalid FEN format";
         }
      }
      // Gestione turno
      int index = FEN.find(' ') + 1;
      if (FEN[index] == 'w') {
         _turn = WHITE;
      } else if (FEN[index] == 'b') {
         _turn = BLACK;
      } else {
         throw "Invalid FEN format";
      }
      // Gestione arrocco
      index += 2;
      int len = FEN.substr(index).find(' ');
      short castling = 0b0000;
      for (int i = index; i < index + len; i++) {
         switch (FEN[i]) {
            case 'K':
               castling |= 0b0100;
               break;
            case 'Q':
               castling |= 0b1000;
               break;
            case 'k':
               castling |= 0b0001;
               break;
            case 'q':
               castling |= 0b0010;
               break;
            case '-':
               break;
            default:
               throw "Invalid FEN format";
         }
      }
      // Gestione en passant
      index += len + 1;
      if (FEN[index] == '-') {
         _last_pawn_move = -1;
         index += 2;
      } else {
         Position behind_pos{FEN.substr(index, 2)};
         if (!behind_pos.is_valid())
            throw "Invalid FEN format";
         _last_pawn_move = behind_pos.x;
         index += 3;
      }
      // Gestione semimosse (regola delle 50 mosse)
      len = FEN.substr(index).find(' ');
      _semimosse_50_move_rule = stoi(FEN.substr(index, len));
      // Gestione mossa attuale
      index += len + 1;
      _mossa = stoi(FEN.substr(index));
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
         _pieces_grid[position.y][position.x] = nullptr;
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
      output.push_back(checking_piece->position());
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
      output.reserve(positions_to_check.size() + 1);
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
      short filter;
      if (direction < 0)
         filter = 0b1000; // Torre 'A'
      else
         filter = 0b0100; // Torre 'H'
      if (side == BLACK)
         filter >>= 2; // Sposto i due bit a destra
      return (filter & _castling_permissions) != 0;
   }

   void Board::lose_castling(const Side &side, const short direction)
   {
      short filter;
      // Prendo il filtro in base al BIANCO
      if (direction == 0)
         filter = 0b0011; // Entrambe le torri
      else if (direction < 0)
         filter = 0b0111; // Torre 'A'
      else
         filter = 0b1011; // Torre 'H'
      if (side == BLACK)
      {
         filter >>= 2;
         filter |= 0b1100;
      }
      _castling_permissions &= filter;
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

   // TODO Controlla finale
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

   Ending Board::is_checkmate_stalemate(const Side &side) const
   {
      // Controllo se il re ha mosse legali
      King *king = get_king(side);
      if (king->has_legal_moves_ignore_checks(*this))
         return Ending::NONE;
      
      std::vector<Piece *> giving_check;
      whos_giving_check(side, giving_check);
      if (giving_check.size() > 1) { // è scacco doppio, e il re non ha mosse legali (è matto)
         return side == WHITE ? BLACK_CHECKMATE : WHITE_CHECKMATE;
      } else if (giving_check.size() == 1) { // è scacco
         std::vector<Position> cells_to_block;
         cells_to_block_check(giving_check, side, cells_to_block);
         for (const Piece *p : _pieces) {
            if (p->side() != side || p->type() == KING)
               continue;
            if (p->can_block_check(*this, cells_to_block))
               return Ending::NONE;
         }
         return side == WHITE ? BLACK_CHECKMATE : WHITE_CHECKMATE;
      } else { // NON è scacco, potrebbe essere stallo
         for (const Piece *p : _pieces) {
            if (p->side() != side || p->type() == KING)
               continue;
            if (p->has_legal_moves_ignore_checks(*this))
               return Ending::NONE;
         }
         return STALEMATE;
      }
   }

   // TODO Controlla finale
   bool Board::is_repetition(void) const
   {
      for (int i = 0; i < _positions.size(); i++)
      {
         short repetitions = 1;
         const std::vector<Piece *> &curr_pos = _positions[i];
         for (int j = i + 1; j < _positions.size(); j++)
         {
            const std::vector<Piece *> &next_pos = _positions[j];
            if (curr_pos.size() != next_pos.size())
            {
               continue;
            }
            bool found = true;
            for (const Piece *p : curr_pos)
            {
               if (std::find_if(next_pos.begin(), next_pos.end(), [p](const Piece *other)
                                { return *p == *other; }) == next_pos.end())
               {
                  found = false;
                  break;
               }
            }
            if (found)
            {
               repetitions++;
            }
         }
         if (repetitions >= 3)
            return true;
      }
      return false;
   }

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

   King *Board::get_king(const Side side) const
   {
      return (King *)(side == WHITE ? _white_king : _black_king);
   }

   void Board::change_position(const Position &from, const Position &to)
   {
      Piece *p = find_piece(from);
      change_position(p, to);
   }

   void Board::change_position(Piece *p, const Position &to)
   {
      _pieces_grid[p->position().y][p->position().x] = nullptr;
      p->position() = to;
      _pieces_grid[p->position().y][p->position().x] = p;
   }

   bool Board::move(const Position from, const Position to, const PieceType promotion_type)
   {
      Piece *p = find_piece(from);
      if (p->side() != _turn)
         return false;
      bool eaten = find_piece(to) != nullptr;
      if (p->move(to, *this, promotion_type))
      {
         // Gestisco l'en passant per la prossima mossa
         update_last_pawn_move(p, from);
         // Aggiorno la regola delle 50 mosse
         update_50_move_rule(p, eaten);
         // Promuovo se necessario
         promote(p, promotion_type);
         // Cambio turno
         toggle_turn();

         return true;
      }
      return false;
   }

   void Board::update_last_pawn_move(const Piece *p, const Position &from)
   {
      if (p->type() == PAWN && abs((from - p->position()).y) == 2)
         _last_pawn_move = p->position().x; // Ho appena avanzato un pedone di 2 => imposto la sua colonna come possibile en passant
      else
         _last_pawn_move = -1; // Non può essere eseguito en passant la prossima mossa
   }

   void Board::update_50_move_rule(const Piece *p, const bool eaten)
   {
      if (eaten || p->type() == PAWN)
      {
         // Reset della regola delle 50 mosse
         _semimosse_50_move_rule = 0;
      }
      else
      {
         // Incremento periodo 50 mosse
         _semimosse_50_move_rule++;
      }
   }

   void Board::promote(Piece *p, PieceType promotion_type)
   {
      if (p->type() == PAWN)
      {
         const short promotion_row = p->side() == WHITE ? 7 : 0;
         if (p->position().y == promotion_row)
         {
            while (promotion_type == KING || promotion_type == PAWN) // Forzo l'utente a scegliere una promozione
               promotion_type = request_promotion_type();
            // Sto promuovendo
            Position promotion_pos = p->position();
            Side promotion_side = p->side();
            kill_piece(promotion_pos);
            create_new_piece(promotion_type, promotion_pos, promotion_side);
         }
      }
   }

   PieceType Board::request_promotion_type() const
   {
      std::cout << "Inserisci il pezzo a cui vuoi promuovere (N,B,R,Q): ";
      char in;
      std::cin >> in;
      switch (std::toupper(in))
      {
      case 'N':
         return KNIGHT;
      case 'B':
         return BISHOP;
      case 'R':
         return ROOK;
      case 'Q':
         return QUEEN;
      default:
         return KING;
      }
   }

   void Board::create_new_piece(const PieceType type, const Position pos, const Side side)
   {
      Piece *p;
      switch (type)
      {
      case QUEEN:
         p = new Queen(pos, side);
         break;
      case ROOK:
         p = new Rook(pos, side);
         break;
      case BISHOP:
         p = new Bishop(pos, side);
         break;
      case KNIGHT:
         p = new Knight(pos, side);
         break;
      case PAWN:
         p = new Pawn(pos, side);
         break;
      case KING: // Spero che non debba succedere
         p = new King(pos, side);
         break;
      }
      _pieces.push_back(p);
      _pieces_grid[pos.y][pos.x] = p;
   }

   Ending Board::is_game_over(void) const
   {
      if (_semimosse_50_move_rule >= 100)
         return _50_MOVE_RULE;
      if (is_insufficient_material())
         return INSUFFICIENT_MATERIAL;
      if (is_repetition())
         return REPETITION;
      return is_checkmate_stalemate(_turn);
   }
}

#endif