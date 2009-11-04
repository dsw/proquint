#ifndef PROQUINT_H
#define PROQUINT_H

/* Assume that unsigned int is a 32-bit int; change this if it is not.
 */
typedef unsigned int uint32_t;

/* Length of a 32-bit quint word, without trailing NUL
 */
#define QUINT_LEN (5*4 + 1*3)

/* Map a quint to a uint.
 */
uint32_t quint2uint(char const *quint);

/* Map a uint to a quint.
 */
void uint2quint(char *quint /* output */, uint32_t i);

#endif /* PROQUINT_H */
