#include "DepthComputer.h"
#include <stdlib.h>
#include <time.h>
#include <algorithm>

namespace Chess
{
   EvaluatedMove::EvaluatedMove(const int evaluation)
       : EvaluatedMove(evaluation, {{-1, -1}, {-1, -1}}, false, false) {}
   EvaluatedMove::EvaluatedMove(const int evaluation, const SimpleMove move)
       : EvaluatedMove(evaluation, move, false, false) {}
   EvaluatedMove::EvaluatedMove(const int evaluation, const bool isGameOver)
       : EvaluatedMove(evaluation, {{-1, -1}, {-1, -1}}, isGameOver, false) {}
   EvaluatedMove::EvaluatedMove(const int evaluation, const SimpleMove move, const bool isGameOver)
       : EvaluatedMove(evaluation, move, isGameOver, false) {}
   EvaluatedMove::EvaluatedMove(const int evaluation, const SimpleMove move, const bool isGameOver, const bool isForcing)
       : evaluation(evaluation), move(move), isGameOver(isGameOver), isForcing(isForcing) {}

   DepthComputer::DepthComputer(Board &board, Side side, int depth)
       : Computer(board, side), _depth(depth)
   {
      if (_depth < 1)
         throw "Depth must be positive and greater than zero";
      if (_depth > 20)
         throw "Depth must be a reasonable value (below 20)";
      srand(time(NULL));
   }

   int DepthComputer::evaluate_piece(const Piece *p)
   {
      return evaluate_piece(p->type(), p->side());
   }

   int DepthComputer::evaluate_piece(const PieceType type, const Side side)
   {
      int multiplier = side == WHITE ? 1 : -1;
      switch (type)
      {
      case PAWN:
         return 1 * multiplier;
      case KNIGHT:
      case BISHOP:
         return 3 * multiplier;
      case ROOK:
         return 5 * multiplier;
      case QUEEN:
         return 9 * multiplier;
      case KING:
         return 0;
      default:
         throw "Unexpected value";
      }
   }

   EvaluatedMove DepthComputer::evaluate_position() const
   {
      return DepthComputer::evaluate_position(_board);
   }

   EvaluatedMove DepthComputer::evaluate_position(const Ending game_over) const
   {
      return DepthComputer::evaluate_position(_board, game_over);
   }

   EvaluatedMove DepthComputer::evaluate_position(const Board &board)
   {
      return evaluate_position(board, board.is_game_over());
   }

   EvaluatedMove DepthComputer::evaluate_position(const Board &board, const Ending game_over)
   {
      if (game_over != Ending::NONE)
         return DepthComputer::evaluate_ending(game_over);
      std::vector<Piece *> pieces;
      board.get_pieces(pieces);
      return evaluate_position(pieces);
   }

   EvaluatedMove DepthComputer::evaluate_position(const std::vector<Piece *> &pieces)
   {
      int evaluation = 0;
      for (const Piece *p : pieces)
         evaluation += evaluate_piece(p);
      return evaluation;
   }

   int DepthComputer::evaluate_ending(const Ending game_over)
   {
      if (game_over < 10) // Patta o partita in corso
         return 0;
      else if (game_over == WHITE_CHECKMATE)
         return CHECKMATE_VALUE; // Matto del bianco
      else
         return -CHECKMATE_VALUE; // Matto del nero
   }

   void DepthComputer::move()
   {
      if (_board.turn() != _side)
         return;
      EvaluatedMove best_move = bestMove(_depth * 2);
      _board.move_forced(best_move.move);
   }

   EvaluatedMove DepthComputer::bestMove(int depth)
   {
      if (depth == 0)
         return evaluate_position();

      const Side side = _board.turn();
      std::vector<EvaluatedMove> best_moves = {side == WHITE ? INT_MIN : INT_MAX}; // Valore default da sovrascrivere
      std::vector<SimpleMove> moves;
      _board.get_all_possible_moves(moves);
      for (const SimpleMove &move : moves)
      {
         _board.move_forced(move);
         Ending e = _board.is_game_over();
         EvaluatedMove curr(0);
         if (e != Ending::NONE)
            curr = {evaluate_ending(e), move, true};
         else
            curr = bestMove(depth - 1);
         const EvaluatedMove best = best_moves[0];
         switch (side)
         {
         case WHITE:
            if (curr.evaluation > best.evaluation)
               best_moves = {{curr.evaluation, move, curr.isGameOver, curr.isForcing}};
            break;
         case BLACK:
            if (curr.evaluation < best.evaluation)
               best_moves = {{curr.evaluation, move, curr.isGameOver, curr.isForcing}};
         }
         if (curr.evaluation == best.evaluation)
            best_moves.push_back({curr.evaluation, move, curr.isGameOver, curr.isForcing});
         // if (best_moves[0].isGameOver)
         //    best = {curr.evaluation, move, curr.isGameOver, curr.isForcing}; // Evito situazioni di patta se possibile
         // else if (curr.isForcing && !best.isForcing)                         // Preferisco una mossa forzata ad una non forzata
         //    best = {curr.evaluation, move, curr.isGameOver, curr.isForcing};
         // else if (curr.isForcing == best.isForcing && abs(rand() % 2) == 0)  // 50% di possibilità
         //    best = {curr.evaluation, move, curr.isGameOver, curr.isForcing}; // Cambio casualmente la mossa (in modo che non venga mosso il primo pezzo disponibile)
         _board.undo_move();
      }
      if (moves.size() == 0)
         throw "Unexpected, moves is empty"; // E' stallo se non ci sono mosse
      else if (moves.size() == 1)            // Mossa forzata
         return {best_moves[0].evaluation, best_moves[0].move, best_moves[0].isGameOver, true};
      else
      {
         std::vector<EvaluatedMove> forcing_moves; // Mosse forzate
         std::copy_if(best_moves.begin(), best_moves.end(), std::back_inserter(forcing_moves), [](const EvaluatedMove move)
                      { return move.isForcing; });
         if (forcing_moves.size() == 0)
         { // Se non ci sono mosse forzate le ignoro
            if (best_moves[0].evaluation == 0)
            {                                             // Posizione equilibrata, potrebbero esserci posizioni patte
               std::vector<EvaluatedMove> no_drawn_moves; // Mosse non patte (posizione equa ma non partita finita)
               std::copy_if(best_moves.begin(), best_moves.end(), std::back_inserter(no_drawn_moves), [](const EvaluatedMove move)
                            { return !move.isGameOver; });
               if (!no_drawn_moves.empty())
                  return random_from_vector(no_drawn_moves);
            }
            return random_from_vector(best_moves);
         }
         else
         { // Guardo le mosse forzate e soprattutto quelle che non pattano
            if (best_moves[0].evaluation == 0)
            {
               std::vector<EvaluatedMove> forcing_no_drawn_moves;
               std::copy_if(best_moves.begin(), best_moves.end(), std::back_inserter(forcing_no_drawn_moves), [](const EvaluatedMove move)
                            { return move.isForcing && !move.isGameOver; });
               if (!forcing_no_drawn_moves.empty()) // Se ci sono mosse che non pattano le gioco, altrimenti mi tocca pattare
                  return random_from_vector(forcing_no_drawn_moves);
            }
            return random_from_vector(forcing_moves);
         }
      }
   }

   template <typename T>
   T DepthComputer::random_from_vector(const std::vector<T> &v)
   {
      int index = rand() % v.size();
      return v[index];
   }
}