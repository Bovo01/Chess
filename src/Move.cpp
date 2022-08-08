#include "Move.h"

namespace Chess
{
   Move::Move(const Position from, const Position to)
       : Move(from, to, false) {}
   Move::Move(const Position from, const Position to, bool eaten)
       : from(from), to(to), eaten(eaten) {}
}