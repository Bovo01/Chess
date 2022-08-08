#ifndef MOVE_H
#define MOVE_H

#include "Position.h"

namespace Chess
{
   struct Move
   {
      Position from, to;
      bool eaten; // true se con questa mossa è stato mangiato un pezzo

      Move(const Position from, const Position to);
      Move(const Position from, const Position to, bool eaten);
   };
}

#endif // MOVE_H