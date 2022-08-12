#ifndef RANDOM_COMPUTER_H
#define RANDOM_COMPUTER_H

#include "Board.h"
#include "Computer.h"

namespace Chess
{
   class RandomComputer : public Computer
   {
   public:
      RandomComputer(Board &board, Side side);

      void move();
   };
}

#endif // RANDOM_COMPUTER_H