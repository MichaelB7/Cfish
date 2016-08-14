/*
  Stockfish, a UCI chess playing engine derived from Glaurung 2.1
  Copyright (C) 2004-2008 Tord Romstad (Glaurung author)
  Copyright (C) 2008-2015 Marco Costalba, Joona Kiiski, Tord Romstad
  Copyright (C) 2015-2016 Marco Costalba, Joona Kiiski, Gary Linscott, Tord Romstad

  Stockfish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Stockfish is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PAWNS_H
#define PAWNS_H

#include "misc.h"
#include "position.h"
#include "types.h"

// PawnEntry contains various information about a pawn structure. A lookup
// to the pawn hash table (performed by calling the probe function) returns
// a pointer to an Entry object.

struct PawnEntry {
  Key key;
  Score score;
  Bitboard passedPawns[2];
  Bitboard pawnAttacks[2];
  Bitboard pawnAttacksSpan[2];
  Square kingSquares[2];
  Score kingSafety[2];
  int castlingRights[2];
  int semiopenFiles[2];
  int pawnSpan[2];
  int pawnsOnSquares[2][2]; // [color][light/dark squares]
  int asymmetry;
};

typedef struct PawnEntry PawnEntry;
typedef PawnEntry PawnTable[16384];

Score do_king_safety_white(PawnEntry *pe, Pos *pos, Square ksq);
Score do_king_safety_black(PawnEntry *pe, Pos *pos, Square ksq);

Value shelter_storm_white(Pos *pos, Square ksq);
Value shelter_storm_black(Pos *pos, Square ksq);


INLINE int semiopen_file(PawnEntry *pe, int c, int f)
{
  return pe->semiopenFiles[c] & (1 << f);
}

INLINE int semiopen_side(PawnEntry *pe, int c, int f, int left)
{
  return pe->semiopenFiles[c] & (left ? (1 << f) - 1 : ~((1 << (f + 1)) - 1));
}

INLINE int pawns_on_same_color_squares(PawnEntry *pe, int c, Square s)
{
  return pe->pawnsOnSquares[c][!!(DarkSquares & sq_bb(s))];
}

INLINE Score king_safety_white(PawnEntry *pe, Pos *pos, Square ksq)
{
  if (  pe->kingSquares[WHITE] == ksq
      && pe->castlingRights[WHITE] == can_castle_cr(WHITE_OO | WHITE_OOO))
    return pe->kingSafety[WHITE];
  else
    return pe->kingSafety[WHITE] = do_king_safety_white(pe, pos, ksq);
}

INLINE Score king_safety_black(PawnEntry *pe, Pos *pos, Square ksq)
{
  if (  pe->kingSquares[BLACK] == ksq
      && pe->castlingRights[BLACK] == can_castle_cr(BLACK_OO | BLACK_OOO))
    return pe->kingSafety[BLACK];
  else
    return pe->kingSafety[BLACK] = do_king_safety_black(pe, pos, ksq);
}

void pawn_init();
PawnEntry *pawn_probe(Pos *pos);

#endif

