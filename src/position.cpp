#ifndef POSITION_CPP
#define POSITION_CPP

#include <ostream>
#include "Position.h"

namespace Chess
{

   bool Position::is_valid(void) const
   {
      if (!((x < 0) || (x > 7)) && !((y < 0) || ((y > 7))))
         return true;

      return false;
   }

   void Position::decode_arithmetic_notation(const std::string &arithmetic_notation)
   {
      x = std::tolower(arithmetic_notation[0]) - 97;
      y = arithmetic_notation[1] - 49;
   }

   Position::Position() : Position(0, 0) {}

   Position::Position(const short x, const short y) : x(x), y(y) {}

   Position::Position(const std::string arithmetic_notation)
   {
      decode_arithmetic_notation(arithmetic_notation);
   }

   Position::Position(const char *arithmetic_notation)
       : Position(std::string(arithmetic_notation))
   {
   }

   Position Position::move(const short x_increment, const short y_increment) const
   {
      short newX = x + x_increment;
      short newY = y + y_increment;

      return Position(newX, newY);
   }

   Direction Position::operator-(const Position &other) const
   {
      return {(short)(x - other.x), (short)(y - other.y)};
   }

   Position Position::operator+(const Direction &other) const
   {
      return {(short)(x + other.x), (short)(y + other.y)};
   }

   Position &Position::operator+=(const Direction &other)
   {
      x += other.x;
      y += other.y;
      return *this;
   }

   bool Position::operator==(const Position &other) const
   {
      if ((x == other.x) && (y == other.y))
         return true;
      return false;
   }

   bool Position::operator!=(const Position &other) const
   {
      return !(*this == other);
   }

   std::string Position::to_string() const
   {
      std::string s;
      s.push_back((char)('A' + x));
      s.push_back((char)('1' + y));
      return s;
   }

   std::string Position::to_string_lower() const
   {
      std::string s;
      s.push_back((char)('a' + x));
      s.push_back((char)('1' + y));
      return s;
   }

   std::ostream &operator<<(std::ostream &os, const Position &pos)
   {
      return os << pos.to_string();
   }
}

#endif