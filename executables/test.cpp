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

int main() {
   // Board b{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
   // Board b{"rnb1k2r/ppp1qpb1/3p1np1/4p2p/4P3/3B1N1P/PPPP1PP1/RNBQ1RK1 b - - 3 12"};
   // Board b{"rnb1kb1r/ppppqppp/5n2/4p3/4P3/5N2/PPPPBPPP/RNBQ1RK1 b kq - 5 4"};
   // Board b{"rnb1kb2/ppppqppr/5n2/4p2p/4P3/3B1N1P/PPPP1PP1/RNBQ1RK1 b q - 2 6"};
   // Board b{"rnbqkbnr/ppp1ppp1/7p/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3"};

   // turn_test();
   // castling_test();
   pawn_test();


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

   cout << "\n--------------------------EN PASSANT TEST SUCCESSFUL--------------------------";
}
void promotion_test() {

   cout << "\n--------------------------PROMOTION TEST SUCCESSFUL--------------------------";
}
