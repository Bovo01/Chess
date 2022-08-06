#include <iostream>
#include <assert.h>
#include "Board.h"

using namespace std;
using namespace Chess;


void turn_test();
void castling_test();
void pawn_test();
void en_passant_test();
void promotion_test();
void pin_test();
void check_test();

int main() {
   // Board b{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
   // Board b{"rnb1k2r/ppp1qpb1/3p1np1/4p2p/4P3/3B1N1P/PPPP1PP1/RNBQ1RK1 b - - 3 12"};
   // Board b{"rnb1kb1r/ppppqppp/5n2/4p3/4P3/5N2/PPPPBPPP/RNBQ1RK1 b kq - 5 4"};
   // Board b{"rnb1kb2/ppppqppr/5n2/4p2p/4P3/3B1N1P/PPPP1PP1/RNBQ1RK1 b q - 2 6"};
   // Board b{"rnbqkbnr/ppp1ppp1/7p/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3"};

   /* TEST */
   // turn_test();
   // castling_test();
   // pawn_test();
   pin_test();


   return 0;
}


void turn_test() {
   Board b;
   assert(b.move("A2", "A3") == true);
   assert(b.move("A3", "A4") == false);
   assert(b.move("C2", "C4") == false);
   assert(b.move("A7", "A5") == true);
   assert(b.move("B8", "C6") == false);
   assert(b.move("B1", "C3") == true);
   assert(b.move("B8", "C6") == true);
   b = Board{"rnb1kb2/ppppqppr/5n2/4p2p/4P3/3B1N1P/PPPP1PP1/RNBQ1RK1 b q - 2 6"};
   assert(b.move("D3", "C4") == false);
   assert(b.move("D7", "D5") == true);
   assert(b.move("E4", "D5") == true);
   assert(b.move("C8", "D7") == true);
   assert(b.move("B8", "C6") == false);
   assert(b.move("B1", "C3") == true);
   assert(b.move("B8", "C6") == true);
   assert(b.move("C3", "B1") == true);
   assert(b.move("E8", "C8") == true);

   cout << "\n--------------------------TURN TEST SUCCESSFUL--------------------------\n";
}

void castling_test() {
   // Arrocco semplice
   Board b{"rnbqk2r/pppp1ppp/3b1n2/4p3/4P3/3B1N2/PPPP1PPP/RNBQK2R w KQkq - 4 4"};
   assert(b.move("E1", "G1") == true);
   assert(b.move("E8", "G8") == true);
   // Perdita di arrocco muovendo la torre
   b = Board{"rnbqk2r/pppp1ppp/3b1n2/4p3/4P3/3B1N2/PPPP1PPP/RNBQK2R w KQkq - 4 4"};
   assert(b.move("H1", "G1") == true);
   assert(b.move("H8", "G8") == true);
   assert(b.move("G1", "H1") == true);
   assert(b.move("G8", "H8") == true);
   assert(b.move("E1", "G1") == false);
   // Perdita di arrocco perdendo la torre
   b = Board{"rnbqk2r/pppp1Np1/3b1n2/4p2p/4P3/3B4/PPPP1PPP/RNBQK2R w KQkq - 0 6"};
   assert(b.move("F7", "H8") == true);
   assert(b.move("E8", "G8") == false);
   assert(b.move("E8", "F8") == true);
   assert(b.move("E1", "G1") == true);
   // Arrocco lungo
   b = Board{"r3k2r/pbppqNp1/1pnb1n2/4p2p/4P3/1PNB1Q2/PBPP1PPP/R3K2R w KQkq - 3 10"};
   assert(b.move("E1", "C1") == true);
   assert(b.move("E8", "C8") == false); // Arrocco impossibile perchè controllato dal cavallo
   assert(b.move("E8", "G8") == true);
   // Arrocco mentre si è sotto scacco
   b = Board{"r3k2r/pbppq1p1/1pnN1n2/4p2p/4P3/1PNB1Q2/PBPP1PPP/R3K2R b KQkq - 0 10"};
   assert(b.move("E8", "G8") == false);
   assert(b.move("E8", "C8") == false);
   assert(b.move("E8", "F7") == false);
   assert(b.move("E8", "F8") == true);

   cout << "\n--------------------------CASTLING TEST SUCCESSFUL--------------------------\n";
}

void pawn_test() {
   en_passant_test();
   promotion_test();

   cout << "\n--------------------------PAWN TEST SUCCESSFUL--------------------------\n";
}
void en_passant_test() {
   // Classico en passant
   Board b{"rnbqkbnr/pppp1ppp/4p3/4P3/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 2"};
   b.move("D7", "D5");
   assert(b.move("E5", "E6") == false);
   assert(b.move("E5", "E7") == false);
   assert(b.move("E5", "F6") == false);
   assert(b.move("E5", "D6") == true);
   // En passant ritardato di una mossa (non si può fare)
   b = Board{"rnbqkbnr/ppp2ppp/4p3/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3"};
   assert(b.move("D1", "E2") == true);
   assert(b.move("F7", "F6") == true); // problema
   assert(b.move("E5", "D6") == false);
   assert(b.move("E5", "F6") == true);
   // Sempre en passant ma dall'altra parte
   b = Board{"rnbqkbnr/ppp2ppp/4p3/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3"};
   assert(b.move("D1", "E2") == true);
   assert(b.move("F7", "F5") == true);
   assert(b.move("E5", "D6") == false);
   assert(b.move("E5", "F6") == true);
   // En passant con pedone pinnato al re
   b = Board{"rnb1kbnr/ppp1qppp/8/3pP3/8/7Q/PPPP1PPP/RNB1KBNR b KQkq - 6 8"};
   b.move("F7", "F5");
   assert(b.move("E5", "F6") == false); // Pedone pinnato dalla regina
   // En passant con pin 'fasullo' (nel senso che provo a rompere il codice, sperando che sia solido)
   b = Board{"rnb1kbnr/ppp3pp/8/3pPp2/1q3P2/7Q/PPPP2PP/RNB1KBNR w KQkq f6 0 10"};
   assert(b.move("E5", "F6") == true);

   cout << "\n--------------------------EN PASSANT TEST SUCCESSFUL--------------------------";
}
void promotion_test() {
   // Problema della promozione : se non inserisco il pezzo a cui promuovere me lo chiede in input finché non lo inserisco

   // Promozione a donna
   Board b{"rnbqkbnr/pppp1ppp/8/8/4P3/5N2/PPPP3p/RNBQKBR1 b Qkq - 1 5"};
   b.move("H2", "H1", QUEEN);
   assert(b.find_piece("H1")->type() == PieceType::QUEEN);
   // Promozione a torre
   b = Board{"rnbqkbnr/pppp1ppp/8/8/4P3/5N2/PPPP3p/RNBQKBR1 b Qkq - 1 5"};
   b.move("H2", "H1", ROOK);
   assert(b.find_piece("H1")->type() == PieceType::ROOK);
   // Promozione a alfiere
   b = Board{"rnbqkbnr/pppp1ppp/8/8/4P3/5N2/PPPP3p/RNBQKBR1 b Qkq - 1 5"};
   b.move("H2", "G1", BISHOP);
   assert(b.find_piece("G1")->type() == PieceType::BISHOP);
   // Promozione a cavallo
   b = Board{"rnbqkbnr/pppp1ppp/8/8/4P3/5N2/PPPP3p/RNBQKBR1 b Qkq - 1 5"};
   b.move("H2", "G1", KNIGHT);
   assert(b.find_piece("G1")->type() == PieceType::KNIGHT);

   cout << "\n--------------------------PROMOTION TEST SUCCESSFUL--------------------------";
}

void pin_test() {
   // Pedone pinnato dalla regina
   Board b = Board{"rnbqkbnr/pppp1ppp/8/4p2Q/4P3/8/PPPP1PPP/RNB1KBNR b KQkq - 1 2"};
   assert(b.move("F7", "F6") == false);
   assert(b.move("F7", "F5") == false);
   assert(b.move("F7", "G6") == false);
   b.move("G7", "G6");
   b.move("H5", "G6");
   assert(b.move("F7", "G6") == true);
   b = Board{"rnb1kbnr/pppp1ppp/8/8/5p1q/4P1P1/PPPP3P/RNBQKBNR w KQkq - 0 4"};
   assert(b.move("G3", "F4") == false);
   assert(b.move("G3", "H4") == true);
   // Cavallo pinnato dalla regina
   b = Board{"rnbqkb1r/ppp2ppp/3p4/8/4n3/5N2/PPPPQPPP/RNB1KB1R b KQkq - 1 5"};
   assert(b.move("E4", "F6") == false);
   // Regina pinnata dalla regina
   b = Board{"rnb1kb1r/ppp1qppp/3p1n2/8/8/3P1N2/PPP1QPPP/RNB1KB1R w KQkq - 1 7"};
   assert(b.move("E2", "D2") == false);
   assert(b.move("E2", "D1") == false);
   assert(b.move("E2", "E3") == true);
   assert(b.move("E7", "D7") == false);
   assert(b.move("E7", "D8") == false);
   assert(b.move("E7", "E2") == false);
   assert(b.move("E7", "E3") == true);
   // Alfiere pinnato dalla regina
   b = Board{"rnb1kbnr/ppp2ppp/3p4/8/8/3PqN2/PPP1BPPP/RNB1K2R w KQkq - 2 9"};
   assert(b.move("E2", "D1") == false);
   assert(b.move("E2", "D3") == false);
   assert(b.move("E2", "F1") == false);
   assert(b.move("E2", "F3") == false);
   // Pedone pinnato non può mangiare
   b = Board{"rnb1k1nr/ppp2ppp/3p4/8/7b/3PqN2/PPP1BPPP/RNB1K2R w KQkq - 6 11"};
   assert(b.move("F2", "E3") == false);
   // Pedone pinnato non può avanzare
   b = Board{"rnb1k1nr/ppp2ppp/8/4p3/7b/3Pq3/PPP1BPPP/RNB1K2R w KQkq - 0 12"};
   assert(b.move("F2", "F3") == false);
   assert(b.move("F2", "F4") == false);
   assert(b.move("F2", "E3") == false);
   // Pedone non pinnato può mangiare
   b = Board{"rnb1k1nr/ppp3pp/5p2/4p3/7b/3Pq1P1/PPP1BP1P/RNB1K2R w KQkq - 0 13"};
   assert(b.move("F2", "E3") == true);
   // Falso pin per vedere se l'alfiere è una torre
   b = Board{"rnbqk2r/ppppbppp/5n2/4P3/8/3P1N2/PPP3PP/RNBQKB1R w KQkq - 1 6"};
   assert(b.move("E5", "F6") == true);
   // Adesso però è una torre per davvero
   b = Board{"rnbqr1k1/pppp1ppp/5n2/4P3/1b6/2PP1N2/PP4PP/RNBQKB1R w KQ - 3 9"};
   assert(b.move("E5", "F6") == false);
   assert(b.move("E5", "E6") == true);
   // Falso pin per vedere se la torre è un alfiere
   b = Board{"rnbq1r1k/ppppbp1p/6pn/4p1N1/1P6/8/P1PPPPPP/RNBQKB1R b KQ - 3 7"};
   assert(b.move("E5", "E4") == true);
   // Adesso però è un alfiere per davvero
   b = Board{"rnbq1r1k/ppppbp1p/6pn/4p3/1P6/5N2/PBPPPPPP/RN1QKB1R b KQ - 7 9"};
   assert(b.move("E5", "E4") == false);
   // Controllo se il cavallo è pinnato da cavallo (situazione assurda ma non si sa mai)
   b = Board{"rnbqk2r/ppppbppp/5n2/4P3/6Q1/3P1N2/PPP3PP/RNB1KB1R b KQkq - 6 8"};
   assert(b.move("F6", "G8") == true);

   cout << "\n--------------------------PIN TEST SUCCESSFUL--------------------------\n";
}

void check_test() {
   // Blocco e mangio la regina che scacca
   Board b{"rnb1kb1r/ppp2ppp/3p1n2/8/8/3PqN2/PPP2PPP/RNB1KB1R w KQkq - 0 8"};
}