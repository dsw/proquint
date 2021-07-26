/* This file is part of proquint: http://github.com/dsw/proquint .
   See License.txt for copyright and terms of use. */

import java.io.*;

/**
   Main driver for library proquint.c.
   Please see the article on proquints: http://arXiv.org/html/0901.4016
   Daniel S. Wilkerson
*/
public class ProquintMain {

  /** My atoi; it's easier to write it than to figure out how to get
      the libraries to parse a string into an unsigned int (Copied the
      C rather than use the Java libraries so the behavior would be
      the same).
  */
  public static int my_atoi(int base, String s) throws IOException {
    long ret = 0;

    for (char c : s.toCharArray()) {
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
        System.err.println("Illegal character in uint-word: '"+ c + "'.");
        System.exit(1);
        break;
      }

      if (value < 0) {
        System.err.println("value < 0: This cannot happen.");
        System.exit(1);
      }
      if (value >= base) {
        System.err.println
          ("Numbers of base "+ base+
           " may not contain the digit '"+ c + "'.");
        System.exit(1);
      }

      ret *= base;
      ret += value;
    }

    if (ret < 0) {
      System.err.println("ret < 0: This cannot happen.");
      System.exit(1);
    }
    if (ret > Integer.MAX_VALUE) {
      System.err.println
        ("Numerical value of String exceeds Integer.MAX_VALUE.");
        System.exit(1);
    }
    return (int) ret;
  }

  public static void main_convertNumber(int base, String s)
    throws IOException {
    //my_atoi could be replaced with:
    //final int n = Integer.parseUnsignedInt(s, base);

    // this seems to be the best place to chop the precision
    final int n = my_atoi(base, s);
    String quint = Proquint.uint2quint(n, "-");

    System.out.printf("uint %s -> quint %s\n", s, quint);
  }

  public static void main_convertQuint(String s) throws IOException {
    long uint0 = Proquint.quint2uint(s);
    System.out.printf("quint %s -> uint %d = x%x\n", s, uint0, uint0);
  }

  /* Convert command-line strings.
   */
  public static void main(String args[]) throws IOException {
    for (String s : args) {
      final int s0 = s.charAt(0);
      if (s0 == '0') {
        System.err.print
          ("Do not lead input strings with a zero.\n"+
           "For decimal, trim leading zeros.\n"+
           "For hex, lead with an 'x'.\n"+
           "We do not process octal.\n");
        System.exit(1);
      } else if (s0 == 'x' || s0 == 'X') {
        /* a hexadecimal number */
        main_convertNumber(16, s.substring(1));
      } else if (Character.isDigit(s0)) {
        /* a decimal number */
        main_convertNumber(10, s);
      } else {
        /* a quint word */
        main_convertQuint(s);
      }
    }

    System.out.println();
  }

}
