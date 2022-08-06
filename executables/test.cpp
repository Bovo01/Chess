#include <iostream>
#include <assert.h>
#include "Board.h"

using namespace std;
using namespace Chess;


void turn_test();

int main() {
   // Board b{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
   // Board b{"rnb1k2r/ppp1qpb1/3p1np1/4p2p/4P3/3B1N1P/PPPP1PP1/RNBQ1RK1 b - - 3 12"};
   // Board b{"rnb1kb1r/ppppqppp/5n2/4p3/4P3/5N2/PPPPBPPP/RNBQ1RK1 b kq - 5 4"};
   // Board b{"rnb1kb2/ppppqppr/5n2/4p2p/4P3/3B1N1P/PPPP1PP1/RNBQ1RK1 b q - 2 6"};
   // Board b{"rnbqkbnr/ppp1ppp1/7p/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3"};
   
   turn_test();



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
   cout << b << endl;
   
   cout << "\n\n--------------------------TURN TEST SUCCESSFUL--------------------------\n\n";
}