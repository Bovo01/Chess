#ifndef MOVE_H
#define MOVE_H

#include "Position.h"
#include "Piece.h"

namespace Chess
{
   struct SimpleMove {
      Position from, to;
      PieceType promotion;

      SimpleMove(Position from, Position to);
      SimpleMove(Position from, Position to, PieceType promotion);
   };

   // Rappresenta lo stato della scacchiera in un istante
   struct BoardStatus {
      short en_passant_column; // valore della colonna in cui si è mosso l'ultimo pedone di 2
      short castling_status; // stato dell'arrocco (salvato con 4 bit binari)
      short semimosse_50_move_rule; // stato delle semimosse per la regola delle 50 mosse
   };

   struct Move
   {
      Position from, to;
      PieceType eaten_piece; // pezzo che è stato mangiato (KING se non è stato mangiato nessuno)
      Position eaten_piece_position; // Posizione del pezzo mangiato (aiuta per en passant)
      PieceType promotion; // Contiene il pezzo a cui ho promosso o King in caso di non promozione
      BoardStatus previous_status; // Contiene lo stato della scacchiera prima di questa mossa

      Move(const Position from, const Position to, const PieceType eaten_piece, const Position eaten_piece_position, const PieceType promotion, BoardStatus previous_status);
   };

   std::ostream& operator<<(std::ostream& os, const Move move);
}

#endif // MOVE_H