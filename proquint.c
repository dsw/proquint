/*
This file is part of proquint.  See License.txt for copyright and
terms of use.

Convert between proquint, hex, and decimal strings.
Daniel S. Wilkerson

Four-bits as a consonant:
    0 1 2 3 4 5 6 7 8 9 A B C D E F
    b d f g h j k l m n p r s t v z

Two-bits as a vowel:
    0 1 2 3
    a i o u

Whole 16-bit word, where "con" = consonant, "vo" = vowel.
     0 1 2 3 4 5 6 7 8 9 A B C D E F
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |con0   |vo1|con2   |vo3|con4   |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |-Hex0--|-Hex1--|-Hex2--|-Hex3--|
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

#include "proquint.h"

#include <stdio.h>              /* printf, fprintf */
#include <stdlib.h>             /* exit */

/* Map uints to consonants.
 */
static char const uint2consonant[] = {
  'b', 'd', 'f', 'g',
  'h', 'j', 'k', 'l',
  'm', 'n', 'p', 'r',
  's', 't', 'v', 'z'
};

/* Map uints to vowels.
 */
static char const uint2vowel[] = {
  'a', 'i', 'o', 'u'
};

/* Map a quint to a uint.
 */
uint32_t quint2uint(char const *quint) {
  LOC;
  char const *const quint_orig = quint; /* for error messages */
  uint32_t ret = 0;
  char c;

  for(; (c=*quint); ++quint) {
    switch(c) {

    /* consonants */
    case 'b': ret <<= 4; ret +=  0; break;
    case 'd': ret <<= 4; ret +=  1; break;
    case 'f': ret <<= 4; ret +=  2; break;
    case 'g': ret <<= 4; ret +=  3; break;

    case 'h': ret <<= 4; ret +=  4; break;
    case 'j': ret <<= 4; ret +=  5; break;
    case 'k': ret <<= 4; ret +=  6; break;
    case 'l': ret <<= 4; ret +=  7; break;

    case 'm': ret <<= 4; ret +=  8; break;
    case 'n': ret <<= 4; ret +=  9; break;
    case 'p': ret <<= 4; ret += 10; break;
    case 'r': ret <<= 4; ret += 11; break;

    case 's': ret <<= 4; ret += 12; break;
    case 't': ret <<= 4; ret += 13; break;
    case 'v': ret <<= 4; ret += 14; break;
    case 'z': ret <<= 4; ret += 15; break;

    /* vowels */
    case 'a': ret <<= 2; ret +=  0; break;
    case 'i': ret <<= 2; ret +=  1; break;
    case 'o': ret <<= 2; ret +=  2; break;
    case 'u': ret <<= 2; ret +=  3; break;

    /* separators */
    case '-': break;

    /* illegal characters */
    default:
      fprintf(stderr, "Illegal character in quint-word '%s': '%c'.\n",
              quint_orig, c);
      exit(1);
      break;
    }
  }

  return ret;
}

/* Map a uint to a quint.
 */
void uint2quint(char *quint /* output */, uint32_t i) {
  LOC;
  /* Note K&R section 2.9: "Right shifting an unsigned quantity always
     fills vacated it with zero."
  */
  uint32_t j;

# define APPEND(X) *quint = (X); ++quint

# define HANDLE_CONSONANT  \
  j = i & MASK_FIRST4;     \
  i <<= 4; j >>= 28;       \
  APPEND(uint2consonant[j])

# define HANDLE_VOWEL      \
  j = i & MASK_FIRST2;     \
  i <<= 2; j >>= 30;       \
  APPEND(uint2vowel[j])

  HANDLE_CONSONANT;
  HANDLE_VOWEL;
  HANDLE_CONSONANT;
  HANDLE_VOWEL;
  HANDLE_CONSONANT;

  APPEND('-');

  HANDLE_CONSONANT;
  HANDLE_VOWEL;
  HANDLE_CONSONANT;
  HANDLE_VOWEL;
  HANDLE_CONSONANT;

# undef APPEND
# undef HANDLE_CONSONANT
# undef HANDLE_VOWEL
}
