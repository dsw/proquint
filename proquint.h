/* This file is part of proquint: http://github.com/dsw/proquint .
   See License.txt for copyright and terms of use. */

#ifndef PROQUINT_H
#define PROQUINT_H

/*
Convert between proquint, hex, and decimal strings.
Please see the article on proquints: http://arXiv.org/html/0901.4016
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

/* Assume that unsigned int is 32-bits; change this otherwise. */
typedef unsigned int uint32_t;

/* Map a uint to two quints using optional sepCar to separate them. */
void uint2quint(char *quint /*output*/, uint32_t i, int sepChar);

/* Map a quint to a uint, skipping non-coding characters. */
uint32_t quint2uint(char const *quint);

#endif /* PROQUINT_H */
