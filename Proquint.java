/* This file is part of proquint: http://github.com/dsw/proquint .
   See License.txt for copyright and terms of use. */

import java.io.*;

/**
  Convert between proquint, hex, and decimal strings.
  Please see the article on proquints: http://arXiv.org/html/0901.4016
  Daniel S. Wilkerson
*/
public class Proquint {

  /** Map uints to consonants. */
  static final char uint2cons[] = {
    'b', 'd', 'f', 'g',
    'h', 'j', 'k', 'l',
    'm', 'n', 'p', 'r',
    's', 't', 'v', 'z'
  };

  /** Map uints to vowels. */
  static final char uint2vowel[] = {
    'a', 'i', 'o', 'u'
  };

  /** Convert an unsigned int (actually a long int) to a proquint; the
   * output is appended to quint; sepChar will be omitted if -1. */
  static void uint2quint
    (StringBuffer quint /*output*/, long i, char sepChar)
  {
    // http://docs.oracle.com/javase/tutorial/java/nutsandbolts/opsummary.html
    // ">>>" Unsigned right shift
    long j;

    j = (i >>> 28) & 0XF; i <<= 4; quint.append(uint2cons [(int)j]);
    j = (i >>> 30) & 0X3; i <<= 2; quint.append(uint2vowel[(int)j]);
    j = (i >>> 28) & 0XF; i <<= 4; quint.append(uint2cons [(int)j]);
    j = (i >>> 30) & 0X3; i <<= 2; quint.append(uint2vowel[(int)j]);
    j = (i >>> 28) & 0XF; i <<= 4; quint.append(uint2cons [(int)j]);

    if (sepChar != -1) {
      quint.append(((char) sepChar));
    }

    j = (i >>> 28) & 0XF; i <<= 4; quint.append(uint2cons [(int)j]);
    j = (i >>> 30) & 0X3; i <<= 2; quint.append(uint2vowel[(int)j]);
    j = (i >>> 28) & 0XF; i <<= 4; quint.append(uint2cons [(int)j]);
    j = (i >>> 30) & 0X3; i <<= 2; quint.append(uint2vowel[(int)j]);
    j = (i >>> 28) & 0XF; i <<= 4; quint.append(uint2cons [(int)j]);
  }

  /** Convert a proquint to an unsigned int (actually a long int).
   */
  static long quint2uint(Reader quint) throws IOException {
    long res = 0;

    while(true) {
      final long c = quint.read();
      if (c == -1) break;

      switch((int)c) {

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

}
