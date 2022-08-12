#ifndef COMPUTER_H
#define COMPUTER_H

#include "Board.h"

namespace Chess
{
   // Classe astratta per tutti i tipi di bot
   class Computer
   {
   protected:
      Board &_board;
      Side _side;

   public:
      Computer(Board &board, Side side);

      virtual void move() = 0;
   };
}

#endif // COMPUTER_H