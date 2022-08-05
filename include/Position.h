#ifndef POSITION_H
#define POSITION_H

#include <string>
#include "Direction.h"

namespace Chess
{
   struct Position
   {
   public:
      short x, y;

   private:
      // Converte la notazione aritmetica (A1, B7, H6, ...) e inizializza le variabili private
      //    alle coordinate corrette ({0, 0}, {6, 1}, {5, }, ...)
      void decode_arithmetic_notation(const std::string &arithmetic_notation);

   public:
      // Costruttore vuoto
      Position();
      // Inizializza l'oggetto controllando la validità delle coordinate
      Position(const short x, const short y);
      // Inizializza l'oggetto tramite il metodo privato 'decode_arithmetic_notation'
      Position(const std::string arithmetic_notation);
      // Inizializza l'oggetto tramite il metodo privato 'decode_arithmetic_notation'
      // Passo un 'const char *' per avere una conversione implicita che con una stringa non avrei
      Position(const char *arithmetic_notation);

      // Controlla se l'oggetto è valido (x e y compresi tra 0 e 7)
      bool is_valid(void) const;

      // Ritorna una nuova posizione data dall'incremento di quella corrente
      //    con le coordinate passate per parametro
      Position move(const short x_increment, const short y_increment) const;

      // Operatore di sottrazione
      Direction operator-(const Position &other) const;
      // Operatore di somma
      Position operator+(const Direction &other) const;
      // Operatore di somma e assegnazione
      Position &operator+=(const Direction &other);
      // Operatore di uguaglianza
      bool operator==(const Position &other) const;
      // Operatore di disuguaglianza
      bool operator!=(const Position &other) const;

      // Converte l'oggetto in stringa
      std::string to_string() const;
   };

   std::ostream &operator<<(std::ostream &os, const Position &pos);
}

#endif