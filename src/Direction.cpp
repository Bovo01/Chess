#ifndef DIRECTION_CPP
#define DIRECTION_CPP

#include <math.h>
#include "Direction.h"

namespace Chess
{
   Direction::Direction(short x, short y)
       : x(x), y(y) {}

   Direction Direction::reduce() const
   {
      if (x == 0 && y == 0)
         return {0, 0};
      else if (x == 0)
         return {0, (short)(y / abs(y))};
      else if (y == 0)
         return {(short)(x / abs(x)), 0};
      else
         return {(short)(x / abs(x)), (short)(y / abs(y))};
   }

   Direction Direction::opposite() const { return {(short)(-x), (short)(-y)}; }

   bool Direction::is_bishop_direction() const { return abs(x) == abs(y); }
   bool Direction::is_rook_direction() const { return x == 0 || y == 0; }
   bool Direction::is_knight_direction() const
   {
      return (abs(x) == 1 && abs(y) == 2) || (abs(x) == 2 && abs(y) == 1);
   }

   bool Direction::is_same_line(const Direction other) const
   {
      return x * other.y - y * other.x == 0; // Controllo la stessa direzione tramite la coordinata z del prodotto vettoriale
   }

   bool Direction::operator==(const Direction &other) const { return x == other.x && y == other.y; }
   bool Direction::operator!=(const Direction &other) const { return !(*this == other); }
}

#endif