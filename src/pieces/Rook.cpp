#ifndef ROOK_CPP
#define ROOK_CPP

#include "Rook.h"
#include "Board.h"

namespace Chess
{
   Rook::Rook(const Position position, const Side side) : Piece(position, side) {}

   PieceType Rook::type(void) const { return ROOK; }

   bool Rook::can_move(const Position &to, const Board &board, const PieceType promotion_type) const {}
}

#endif