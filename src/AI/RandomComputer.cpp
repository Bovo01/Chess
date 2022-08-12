#include <stdlib.h>
#include <time.h>
#include "RandomComputer.h"


namespace Chess
{
   RandomComputer::RandomComputer(Board &board, Side side)
       : Computer(board, side)
   {
      srand(time(NULL));
   }

   void RandomComputer::move()
   {
      if (_board.turn() != _side)
         return; // Non è il turno di questo bot
      std::vector<Move> moves;
      _board.get_all_possible_moves(moves);
      int move_index = rand() % moves.size();
      Move move = moves[move_index];
      _board.move_forced(move.from, move.to, QUEEN); // TODO Aggiungi scelta promozione
   }
}