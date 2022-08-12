#include "Computer.h"

namespace Chess
{
   Computer::Computer(Board &board, Side side)
       : _board(board), _side(side) {}
}