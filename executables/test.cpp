#include <iostream>
using namespace std;

class Parent {
protected:
   int data;
public:
   Parent() {
      data = 9;
   }
   void print() const {
      if (is_valid()) {
         cout << data << endl;
      }
   }
   virtual bool is_valid() const = 0;
};

class Child : public Parent {
public:
   bool is_valid() const {
      return data > 5;
   }
};

int main() {
   Child c;
   c.print();
   return 0;
}