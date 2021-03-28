package com.github.dsw.proquint;

import java.io.IOException;
import java.io.Reader;
import java.io.StringReader;
import java.security.SecureRandom;

public class Proquint {
  
  private static final SecureRandom RANDOM = new SecureRandom();
  
  public static String randomProquint() {
    return encode(Math.abs(RANDOM.nextInt()));
  }
  
  public static String encode(long source) {
    try {
      return convertNumber(10, new StringReader(Long.toString(source)));
    } catch (IOException e) {
      throw new RuntimeException(e);
    }
  }

  public static String encode(int source) {
    try {
      return convertNumber(10, new StringReader(Integer.toString(source)));
    } catch (IOException e) {
      throw new RuntimeException(e);
    }
  }

  public static String decode(String proquint) {
    try {
      return convertQuint(new StringReader(proquint));
    } catch (IOException e) {
      throw new RuntimeException(e);
    }
  }
  
  private static String convertNumber(int base, Reader s) throws IOException {
    /*
     * Length of a 32-bit quint word, without trailing NUL: two quints plus a
     * separator.
     */
    final int QUINT_LEN = 5 * 2 + 1;
    /*
     * Double length plus another separator in case we switch to 64-bits.
     */
    final int DOUBLE_QUINT_LEN = 2 * QUINT_LEN + 1;

    // this seems to be the best place to chop the precision
    final int n = my_atoi(base, s);
    StringBuffer quint = new StringBuffer(DOUBLE_QUINT_LEN + 1);
    uint2quint(quint, n, '-');

    /* fprintf(stderr, "uint %s -> quint %s\n", s, quint); */
    return quint.toString();
  }

  private static String convertQuint(Reader s) throws IOException {
    long uint0 = quint2uint(s);
    /* fprintf(stderr, "quint %s -> uint %u = x%x\n", s, uint0, uint0); */
    return "0x"+ Long.toHexString(uint0);
  }
  
  /*
   * My atoi; it's easier to write it than to figure out how to get the
   * libraries to parse a string into an unsigned int (Copied the C rather than
   * use the Java libraries so the behavior would be the same).
   */
  private static int my_atoi(int base, Reader s) throws IOException {
    long ret = 0;

    while (true) {
      final int c = s.read();
      if (c == -1)
        break;
      int value = -1;

      switch (c) {
      /* decimal digits */
        case '0':
          value = 0;
          break;
        case '1':
          value = 1;
          break;
        case '2':
          value = 2;
          break;
        case '3':
          value = 3;
          break;
        case '4':
          value = 4;
          break;

        case '5':
          value = 5;
          break;
        case '6':
          value = 6;
          break;
        case '7':
          value = 7;
          break;
        case '8':
          value = 8;
          break;
        case '9':
          value = 9;
          break;

        case 'a':
        case 'A':
          value = 10;
          break;
        case 'b':
        case 'B':
          value = 11;
          break;
        case 'c':
        case 'C':
          value = 12;
          break;
        case 'd':
        case 'D':
          value = 13;
          break;
        case 'e':
        case 'E':
          value = 14;
          break;
        case 'f':
        case 'F':
          value = 15;
          break;

        /* illegal characters */
        default:
          throw new RuntimeException("Illegal character in uint-word: '" + (char) c + "'.");
      }

      if (value < 0) {
        System.err.println("value < 0: This cannot happen.");
        throw new RuntimeException("value < 0: This cannot happen.");
      }
      if (value >= base) {
        throw new RuntimeException("Numbers of base " + base + " may not contain the digit '" + (char) c + "'.");
      }

      ret *= base;
      ret += value;
    }

    if (ret < 0) {
      System.err.println("ret < 0: This cannot happen.");
      throw new RuntimeException("ret < 0: This cannot happen.");
    }
    if (ret > Integer.MAX_VALUE) {
      throw new RuntimeException("Numerical value of String exceeds Integer.MAX_VALUE.");
    }
    return (int) ret;
  }
  
  /* 
   * Convert an unsigned int to a proquint; the output is appended to
   * quint; sepChar will be omitted if -1.
   */
  private static void uint2quint (StringBuffer quint /*output*/, int i, char sepChar) {
    // http://docs.oracle.com/javase/tutorial/java/nutsandbolts/opsummary.html
    // ">>>" Unsigned right shift
    int j;

    final int MASK_FIRST4 = 0xF0000000;
    final int MASK_FIRST2 = 0xC0000000;

    j = i & MASK_FIRST4; i <<= 4; j >>>= 28; quint.append(uint2consonant[j]);
    j = i & MASK_FIRST2; i <<= 2; j >>>= 30; quint.append(uint2vowel[j]);
    j = i & MASK_FIRST4; i <<= 4; j >>>= 28; quint.append(uint2consonant[j]);
    j = i & MASK_FIRST2; i <<= 2; j >>>= 30; quint.append(uint2vowel[j]);
    j = i & MASK_FIRST4; i <<= 4; j >>>= 28; quint.append(uint2consonant[j]);

    if (sepChar != -1) {
      quint.append(((char) sepChar));
    }

    j = i & MASK_FIRST4; i <<= 4; j >>>= 28; quint.append(uint2consonant[j]);
    j = i & MASK_FIRST2; i <<= 2; j >>>= 30; quint.append(uint2vowel[j]);
    j = i & MASK_FIRST4; i <<= 4; j >>>= 28; quint.append(uint2consonant[j]);
    j = i & MASK_FIRST2; i <<= 2; j >>>= 30; quint.append(uint2vowel[j]);
    j = i & MASK_FIRST4; i <<= 4; j >>>= 28; quint.append(uint2consonant[j]);
  }
  
  /*
   * Convert a proquint to an unsigned int (long).
   */
  static long quint2uint(Reader quint) throws IOException {
    long res = 0;

    while(true) {
      final int c = quint.read();
      if (c == -1) break;

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

  /** Map uints to consonants. */
  private static final char uint2consonant[] = {
    'b', 'd', 'f', 'g',
    'h', 'j', 'k', 'l',
    'm', 'n', 'p', 'r',
    's', 't', 'v', 'z'
  };

  /** Map uints to vowels. */
  private static final char uint2vowel[] = {
    'a', 'i', 'o', 'u'
  };
}
