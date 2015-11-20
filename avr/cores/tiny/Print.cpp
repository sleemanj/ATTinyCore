/*
 Print.cpp - Base class that provides print() and println()
 Copyright (c) 2008 David A. Mellis.  All right reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 
 Modified 23 November 2006 by David A. Mellis
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "Arduino.h"
#include "Print.h"

// Public Methods //////////////////////////////////////////////////////////////

/* default implementation: may be overridden */
size_t Print::write(const uint8_t *buffer, size_t size)
{
  // Assume that write always returns 1 for a single character
  // we remove some additions and save a few bytes.
  //
  // Note that Hardware, Software and Basic serial all return 1
  // for character write regardless, so this as a valid assumption
  // at least in those cases.
  //
  // J.Sleeman (sparks@gogo.co.nz)
  
  for(size_t n = 0; n < size; n++)
  {
    write(buffer[n]);
  }
  return size;
  
  /*
  size_t n = 0;
  while (size--) {
    n += write(*buffer++);
  }
  return n;
  */
}

size_t Print::print(const __FlashStringHelper *ifsh)
{
  PGM_P p = reinterpret_cast<PGM_P>(ifsh);
  size_t n = 0;
  
  // This way saves us 4 bytes, again we assume that
  // write of a single byte always returns 1 (as it does
  // for all the Serial classes in this core).
  //
  // J.Sleeman (sparks@gogo.co.nz)
  
  do {
    unsigned char c = pgm_read_byte(p++);
    if (c == 0) break;
    write(c);  
  } while(++n);
  
  /*
  while (1) {  
    unsigned char c = pgm_read_byte(p++);
    if (c == 0) break;    
    if (write(c)) n++;
    else break;
  }
  */
 
  return n;
}

size_t Print::print(const String &s)
{
  size_t n = 0;
  for (uint16_t i = 0; i < s.length(); i++) {
    n += write(s[i]);
  }
  return n;
}

size_t Print::print(const char str[])
{
  return write(str);
}

size_t Print::print(char c)
{
  return write(c);
}

size_t Print::print(unsigned char b, uint8_t base)
{
  return print((unsigned long) b, base);
}

size_t Print::print(int n, uint8_t base)
{
  return print((long) n, base);
}

size_t Print::print(unsigned int n, uint8_t base)
{
  return print((unsigned long) n, base);
}

size_t Print::print(long n, uint8_t base)
{
  /* Optimisation - not testing for invalid base
   * saves us some bytes, printNumber will "promote"
   * it to a valid base (2) anyway.
   * 
   * The long-winded negative sign for base 10
   * is simplified to remove the temporary variable
   * sames us some more bytes
   * 
   * I get 46 bytes less flash usage with mine.  On a tiny13
   * that's MASSIVE!
   * 
   * J.Sleeman (sparks@gogo.co.nz)   
   */  
  
  /*
  if (base == 0) {
    return write(n);
  } else if (base == 10) {
    int t = 0;
    if (n < 0) {
      t = print('-');
      n = -n;
    }
    return printNumber(n, 10) + t;
  } else { 
      
  */
  
  if (base == 10 && n < 0) {
    // Interesting, this single liner makes it 16 bytes larger
    // than the 2 lines.    
    //    return write('-') + printNumber(-n,base);    
    write('-');
    return printNumber(-n,base)+1;
  } else {
    return printNumber(n, base);
  }
}

size_t Print::print(unsigned long n, uint8_t base)
{
  /* As above, invalid base gets promoted in printNumber()
   * no need to look for it here.
  if (base == 0) return write(n);
  else return printNumber(n, base);
  */
  return printNumber(n,base);  
}

size_t Print::print(double n, uint8_t digits)
{
  return printFloat(n, digits);
}

size_t Print::println(const __FlashStringHelper *ifsh)
{
  size_t n = print(ifsh);
  n += println();
  return n;
}

size_t Print::print(const Printable& x)
{
  return x.printTo(*this);
}

size_t Print::println(void)
{
  // I'm gonna assume that printing a single character
  // is only every going to return 1, so we don't need 
  // to keep track of this, saves us 10 bytes of flash
  //
  // Note that this is true for Hardware,TinySoftware 
  // and Basic Serial classes, they all blindly return
  // 1 as the result of a ::write(byte) regardless if
  // there is any notion of "success" or not.
  //
  // J.Sleeman (sparks@gogo.co.nz)
  //
  
  /*
  size_t n = print('\r');
  n += print('\n');
  return n;
  */
  
  print('\r'); 
  print('\n');
  return 2;
}

size_t Print::println(const String &s)
{
  size_t n = print(s);
  n += println();
  return n;
}

size_t Print::println(const char c[])
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t Print::println(char c)
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t Print::println(unsigned char b, uint8_t base)
{
  size_t n = print(b, base);
  n += println();
  return n;
}

size_t Print::println(int num, uint8_t base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(unsigned int num, uint8_t base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(long num, uint8_t base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(long long num, uint8_t base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(unsigned long num, uint8_t base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(double num, int digits)
{
  size_t n = print(num, digits);
  n += println();
  return n;
}

size_t Print::println(const Printable& x)
{
  size_t n = print(x);
  n += println();
  return n;
}

// Private Methods /////////////////////////////////////////////////////////////

size_t Print::printNumber(unsigned long n, uint8_t base) {
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 2;

  #ifdef PRINT_BIN_OCT_HEX_BASES_ONLY
    // Generalized division is a very expensive operation in code size
    // but if we limit out scope to only binary divisors
    //   Usefully speaking: 2, 8, 16
    // then we can just divide by shifting right.  Of course if you are
    // already using division somewhere in your code, then this is a moot
    // point, but often times you don't need to divide.  On very small
    // chips (tiny13) this is the only way you're going to have room
    // to do what you need to do!
    uint8_t shifter = 0;
    
    // base    = 8 = 00001000
    // shifter = 0
    //
    //  base >> 1 = 00000100 = TRUE
    //  shifter++
    //
    // base = 00000100
    // shifter = 1
    //
    // base    = 00000010
    // shifter = 2
    //
    // base    = 00000001
    // shifter = 3
    //
    // base    = 00000000 = FALSE
    // shifter = NO CHANGE (loop break)
    if(base == 10) { write('x'); base = 16; }
    while((base = base >> 1)) 
    { 
      shifter++;       
    }
    
    do {
      unsigned long m = n;    
      n = n >> shifter;      
      char c = m - (n<<shifter);
      *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while(n);
  #else
    do {
      unsigned long m = n;    
      n /= base;      
      char c = m - base * n;
      *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while(n);
  #endif

  return write(str);
}

size_t Print::printFloat(double number, uint8_t digits) 
{ 
  size_t n = 0;
  
  // Handle negative numbers
  if (number < 0.0)
  {
     n += print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  n += print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) {
    n += print("."); 
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    n += print(toPrint);
    remainder -= toPrint; 
  } 
  
  return n;
}
