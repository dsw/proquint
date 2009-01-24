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

#include <stdio.h>              /* printf, fprintf */
#include <ctype.h>              /* isdigit */
#include <stdlib.h>             /* exit */

#ifdef DEBUG
#  define LOC \
   printf("%s:%d:%s\n", __FILE__, __LINE__, __FUNCTION__); fflush(stdout)
#else
#  define LOC
#endif

/* Assume that unsigned int is a 32-bit int; change this if it is not.
 */
typedef unsigned int uint32_t;

/* Mask off the first four bits.
 */
#define MASK_FIRST4 0xF0000000
/* Mask off the first two bits.
 */
#define MASK_FIRST2 0xC0000000

/* Length of a 32-bit quint word, without trailing NUL
 */
#define QUINT_LEN (5*4 + 1*3)

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

/* My atoi; it's easier to write it than to figure out how to get the
   libraries to parse a string into an unsigned int.
*/
static uint32_t my_atoi(int base, char const *s) {
  LOC;
  char const *const s_orig = s; /* for error messages */
  uint32_t ret = 0;
  int c;

  for(; (c=*s); ++s) {
    LOC;
    int value = -1;

    switch(c) {
    /* decimal digits */
    case '0': value = 0; break;
    case '1': value = 1; break;
    case '2': value = 2; break;
    case '3': value = 3; break;
    case '4': value = 4; break;

    case '5': value = 5; break;
    case '6': value = 6; break;
    case '7': value = 7; break;
    case '8': value = 8; break;
    case '9': value = 9; break;

    case 'a': case 'A': value = 10; break;
    case 'b': case 'B': value = 11; break;
    case 'c': case 'C': value = 12; break;
    case 'd': case 'D': value = 13; break;
    case 'e': case 'E': value = 14; break;
    case 'f': case 'F': value = 15; break;

    /* illegal characters */
    default:
      fprintf(stderr, "Illegal character in uint-word '%s': '%c'.\n",
              s_orig, c);
      exit(1);
      break;
    }

    if (value < 0) {
      fprintf(stderr, "This cannot happen.");
      exit(1);
    }
    if (value >= base) {
      fprintf(stderr,
              "Numbers of base %d may not contain the digit '%c': '%s'.\n",
              base, c, s_orig);
      exit(1);
    }

    ret *= base;
    ret += value;
  }

  return ret;
}

static void main_convertNumber(int base, char const *s) {
  LOC;
  char quint[QUINT_LEN+1];
  int i;

  for(i=0; i<QUINT_LEN+1; ++i) quint[i] = '\0';
  uint2quint(quint, my_atoi(base, s));

  /* fprintf(stderr, "uint %s -> quint %s\n", s, quint); */
  printf("%s ", quint);
}

static void main_convertQuint(char const *s) {
  LOC;

  uint32_t const uint0 = quint2uint(s);

  /* fprintf(stderr, "quint %s -> uint %u = x%x\n", s, uint0, uint0); */
  printf("x%x ", uint0);
}

/* Convert command-line strings.
 */
int main(int argc, char const **argv) {
  LOC;

  --argc; ++argv;
  for(; *argv; --argc, ++argv) {
    char const *s = *argv;
    if (!s[0]) continue;        /* this would be very strange */
    if (s[0] == '0') {
      fprintf(stderr,
              "Do not lead input strings with a zero.\n"
              "For decimal, trim leading zeros.\n"
              "For hex, lead with an 'x'.\n"
              "We do not process octal.\n");
      exit(1);
    } else if (s[0] == 'x' || s[0] == 'X') {
      /* a hexadecimal number */
      main_convertNumber(16, s+1);
    } else if (isdigit(s[0])) {
      /* a decimal number */
      main_convertNumber(10, s);
    } else {
      /* a quint word */
      main_convertQuint(s);
    }
  }

  printf("\n");
  return 0;
}
