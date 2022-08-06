#ifndef DIRECTION_H
#define DIRECTION_H

#include <ostream>

namespace Chess
{
   struct Direction
   {
      short x, y;

      Direction(short x, short y);

      // Ritorna una nuova direzione normalizzata (vettore con coordinate unitarie, non segue la definizione reale di normalizzazione)
      Direction reduce() const;
      // Ritorna una nuova direzione con coordinate invertite (-x, -y)
      Direction opposite() const;

      // Ritorna true se questa direzione può essere percorsa da un alfiere
      bool is_bishop_direction() const;
      // Ritorna true se questa direzione può essere percorsa da una torre
      bool is_rook_direction() const;
      // Ritorna true se questa direzione può essere percorsa da un cavallo
      bool is_knight_direction() const;

      // Ritorna true se l'altra direzione è equivalente (non considera il verso)
      bool is_same_line(const Direction other) const;

      bool operator==(const Direction &other) const;
      bool operator!=(const Direction &other) const;
   };

   std::ostream &operator<<(std::ostream &os, const Direction &dir);
}

#endif