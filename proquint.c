/*
This file is part of proquint.  See License.txt for copyright and
terms of use.
*/

#include "proquint.h"

/* Mask off the first four bits.
 */
#define MASK_FIRST4 0xF0000000
/* Mask off the first two bits.
 */
#define MASK_FIRST2 0xC0000000

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

/* Map a quint to a uint, skipping non-coding characters.
 */
uint32_t quint2uint(char const *quint) {
  uint32_t res = 0;
  char c;

  for(; (c=*quint); ++quint) {
    switch(c) {

    /* consonants */
    case 'b': res <<= 4; res +=  0; break;
    case 'd': res <<= 4; res +=  1; break;
    case 'f': res <<= 4; res +=  2; break;
    case 'g': res <<= 4; res +=  3; break;

    case 'h': res <<= 4; res +=  4; break;
    case 'j': res <<= 4; res +=  5; break;
    case 'k': res <<= 4; res +=  6; break;
    case 'l': res <<= 4; res +=  7; break;

    case 'm': res <<= 4; res +=  8; break;
    case 'n': res <<= 4; res +=  9; break;
    case 'p': res <<= 4; res += 10; break;
    case 'r': res <<= 4; res += 11; break;

    case 's': res <<= 4; res += 12; break;
    case 't': res <<= 4; res += 13; break;
    case 'v': res <<= 4; res += 14; break;
    case 'z': res <<= 4; res += 15; break;

    /* vowels */
    case 'a': res <<= 2; res +=  0; break;
    case 'i': res <<= 2; res +=  1; break;
    case 'o': res <<= 2; res +=  2; break;
    case 'u': res <<= 2; res +=  3; break;

    /* separators */
    default: break;
    }
  }

  return res;
}

/* Map a uint to two quints using optional sepCar to separate them.
 */
void uint2quint(char *quint /*output*/, uint32_t i, int sepChar) {
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

  if (sepChar) {
    APPEND(((char) sepChar));
  }

  HANDLE_CONSONANT;
  HANDLE_VOWEL;
  HANDLE_CONSONANT;
  HANDLE_VOWEL;
  HANDLE_CONSONANT;

# undef HANDLE_VOWEL
# undef HANDLE_CONSONANT
# undef APPEND
}
