#ifndef DEPTH_COMPUTER_H
#define DEPTH_COMPUTER_H

#include "Computer.h"
#include "Board.h"

namespace Chess
{
   struct EvaluatedMove {
      EvaluatedMove(const int evaluation);
      EvaluatedMove(const int evaluation, const SimpleMove move);
      EvaluatedMove(const int evaluation, const bool isGameOver);
      EvaluatedMove(const int evaluation, const SimpleMove move, const bool isGameOver);
      EvaluatedMove(const int evaluation, const SimpleMove move, const bool isGameOver, const bool isForcing);

      int evaluation;
      SimpleMove move;
      bool isGameOver; // Default value is false
      bool isForcing;  // Default value is false
   };

   class DepthComputer : public Computer
   {
   private:
      int _depth;

      static constexpr int CHECKMATE_VALUE = 200; // Il totale di materiale possibile è 103

   private:
      static int evaluate_piece(const Piece *p);
      static int evaluate_piece(const PieceType type, const Side side);
      EvaluatedMove evaluate_position() const;
      EvaluatedMove evaluate_position(const Ending game_over) const;

      EvaluatedMove bestMove(int depth);

      template <typename T>
      static T random_from_vector(const std::vector<T> &v);

   public:
      DepthComputer(Board &board, Side side, int depth);

      void move();

      static int evaluate_ending(const Ending game_over);
      static EvaluatedMove evaluate_position(const Board &board);
      static EvaluatedMove evaluate_position(const Board &board, const Ending game_over);
      static EvaluatedMove evaluate_position(const std::vector<Piece *> &pieces);
   };
}

#endif