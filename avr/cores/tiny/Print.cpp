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

// print of a char must always just pass straight to write
// in order to print the actual character rather than it's value
size_t Print::print(char c)
{
  return write(c);
}

#if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_BYTE
size_t Print::print(unsigned char b, uint8_t base)
{
  return print((UNSIGNED_PRINT_INT_TYPE) b, base);
}
#endif

#if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_INT
size_t Print::print(int n, uint8_t base)
{
  return print((PRINT_INT_TYPE) n, base);
}

size_t Print::print(unsigned int n, uint8_t base)
{
  return print((UNSIGNED_PRINT_INT_TYPE) n, base);
}
#endif

#if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_LONG
size_t Print::print(long n, uint8_t base)
{
  return print((PRINT_INT_TYPE) n, base);
}

size_t Print::print(unsigned long n, uint8_t base)
{
  return print((UNSIGNED_PRINT_INT_TYPE) n, base);
}
#endif

size_t Print::print(PRINT_INT_TYPE n, uint8_t base)
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

size_t Print::print(UNSIGNED_PRINT_INT_TYPE n, uint8_t base)
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
#if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_BYTE
size_t Print::println(unsigned char b, uint8_t base)
{
  size_t n = print(b, base);
  n += println();
  return n;
}
#endif
#if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_INT
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
#endif
#if PRINT_MAX_INT_TYPE != PRINT_INT_TYPE_LONG
size_t Print::println(long num, uint8_t base)
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
#endif
size_t Print::println(PRINT_INT_TYPE num, uint8_t base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(UNSIGNED_PRINT_INT_TYPE num, uint8_t base)
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


#ifndef PRINT_USE_BASE_ARBITRARY

// This is a more memory (RAM and FLASH) efficient printNumber implementation
// for very low ram machines, eg tiny13, the original was way too big
// for several reasons...
//
// 1. It uses unsigned long
// 2. It uses division (an expensive operation on attiny)
// 3. It uses a character buffer of effectively 33 bytes
//    in order to build the number before writing it out
//
// When you only have 64 bytes of ram in the first place, that doesn't end well.
//
// This version, which I wrote with the inspiration of especially EEVBlog
// forum user "TassiloH" 
//
//  http://www.eevblog.com/forum/microcontrollers/memory-efficient-int-to-chars-without-division-(bitshift-ok)-for-binary-bases/msg804586/#msg804586
//
// does not use a buffer, it has configurable maximum int type (see Print.h)
// and it doesn't use division.
//
// The only downside is that it can't do arbitrary bases, just 2, 8, 16 and 10
//
// In your pins_arduino.h you will want to set
//
//  #define PRINT_USE_BASE_BIN
//  #define PRINT_USE_BASE_HEX
//  #define PRINT_USE_BASE_OCT
//  #define PRINT_USE_BASE_DEC
//
// if you define PRINT_USE_BASE_ARBITRARY then this function will not be used
// it will suck your ram, but you can use arbitrary bases.

size_t Print::printNumber(UNSIGNED_PRINT_INT_TYPE n, uint8_t base)
{
  UNSIGNED_PRINT_INT_TYPE dValue;

  // dValue needs to be set to the higest, err, exponent, is that
  // the word I'm looking for, of the base that fits in the integer
  // type we are handling.
  //
  // For example, let's say our integer type is "byte"
  // and our base is "10"
  //   10^0 = 1, that fits in a byte
  //   10^1 = 10, that fits in a byte
  //   10^2 = 100, that fits in a byte
  //   10^3 = 1999, that does NOT fit in a byte
  // so we set dValue to 10^2 because that's the highest value
  // that fits in a byte.
  //
  // So we can calculate this quite easily in exactly that fashion
  // just see if we can do the "next" exponent without an overflow
  // and that would be a good general solution.
  //
  // However multiply is not a hardware instruction in attiny
  // so it's more expensive (significantly) to use that general
  // solution, instead we use a switch on the base and select appropriate
  // pre-calculated values.
  //
  // For what it's worth, here's the general solution...
  //
  // dValue = base;
  // while(dValue * base > dValue) dValue *= base;
    
  switch(base)
  {    
    // "Promote" any unsupported base to the highest base that is supported,
    // because we have the bases in this switch high -> low this will fall 
    // through to the highest supported base
    default:
        
        #if defined(PRINT_USE_BASE_HEX)
        write('x');
        base = 16;
        #elif defined(PRINT_USE_BASE_DEC)
        write('d');
        base = 10;
        #elif defined(PRINT_USE_BASE_OCT)
        write('o');
        base = 8;
        #elif defined(PRINT_USE_BASE_BIN)
        write('b');
        base = 2;
        #endif
                    
    #ifdef PRINT_USE_BASE_HEX
    case 16:          
      #if PRINT_MAX_INT_TYPE   == PRINT_INT_TYPE_LONG
        dValue = 268435456;
      #elif PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_INT
        // MAX  = 0xFFFF
        // STRT = 0x1000 = 4096      
        dValue = 4096;
      #elif PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_BYTE
        dValue = 16;
      #else
        #error Unknown PRINT_MAX_INT_TYPE
      #endif
      break;
    #endif
    
    #ifdef PRINT_USE_BASE_DEC
    case 10:
      #if PRINT_MAX_INT_TYPE   == PRINT_INT_TYPE_LONG
        dValue = 1000000000;
      #elif PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_INT
        // MAX  = 65535
        // STRT = 10000 = 10000
        dValue = 10000;        
      #elif PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_BYTE
        dValue = 100;
      #else
        #error Unknown PRINT_MAX_INT_TYPE
      #endif
      break;
    #endif
      
    #ifdef PRINT_USE_BASE_OCT        
    case 8:            
      #if PRINT_MAX_INT_TYPE   == PRINT_INT_TYPE_LONG
        dValue = 1073741824;
      #elif PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_INT
        // MAX  = o177777
        // STRT = o100000 = 32768
        // coincidentally the same as base2
        dValue = 32768;     
      #elif PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_BYTE
        dValue = 64;
      #else
        #error Unknown PRINT_MAX_INT_TYPE
      #endif
      break;
    #endif
      
    #ifdef PRINT_USE_BASE_BIN        
    case 2:
      #if PRINT_MAX_INT_TYPE   == PRINT_INT_TYPE_LONG
        //dValue = 0b10000000000000000000000000000000;
        dValue = 0x80000000;
      #elif PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_INT
        //MAX  = 0b1111111111111111
        //STRT = 0b1000000000000000 = 32768
        dValue = 32768;  
      #elif PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_BYTE
        dValue = 128;
      #else
        #error Unknown PRINT_MAX_INT_TYPE
      #endif
      break;      
    #endif            
  }
     
  uint8_t digit = (1 << 7); // Top bit indicates if have seen the msd or not, 0 = seen, 1 = not
  while(dValue)
  {
    while(n >= dValue)
    {
      n -= dValue;
      digit++;
    }
    
    // If the top bit is zero, or the bottom bits are greater than 0
    // then we have a digit to print (top bit zero means we have seen 
    // the MSD and skipped the leading zeros)
    if(!(digit & (1 << 7)) || (digit & ~(1<<7)) )
    {      
      digit = digit & ~(1<<7); // Clears the top bit for us and gives the actual number
      write(digit < 10 ? digit + '0' : digit + 'A' - 10);      
      digit = 0; // this should not be required, since we clear the 
                 // digit's bits right after this if block as you can see below;
                 // but some strangeness happens with this, the flash usage
                 // actually goes down when we set the digit to zero here
                 // even though we also have to set it below.  Nuts.
    }
    digit &= (1 << 7);      // Clear bottom 7 bits for the next go around
    
    switch(base)
    {
      // The next dValue is the current dValue divided by the base
      // example base 16
      //   0x1000 divide by 16 (0x10) = 0x100
      //   instead of dividing by 16 we shift out 4 bits, because
      //     0b0001000000000000 = 0x1000 
      //  divided by 0x10      
      //     0b000000100000000
      //  you can see that is the same as a 4 bit shift 
      //  of course that's only true for binary bases (2, 4, 8, 16, etc...)
      //
      // For base 10 instead of dividing we use a lookup, because division
      // is too expensive for flash
      //
      // Note that we are guaranteed to have a valid base here, because
      // we forced it to one in the first switch above.
            
      #ifdef PRINT_USE_BASE_BIN
      case 2:  dValue >>= 1; break;
      #endif
      
      #ifdef PRINT_USE_BASE_OCT
      case 8:  dValue >>= 3; break;
      #endif
      
      #ifdef PRINT_USE_BASE_HEX
      case 16: dValue >>= 4; break;
      #endif
      
      #ifdef PRINT_USE_BASE_DEC
      case 10:         
        switch(dValue)
        {
          #if PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_LONG
          case 1000000000: dValue = 100000000; break;
          case 100000000:  dValue = 10000000; break;
          case 10000000:   dValue = 1000000; break;
          case 1000000:    dValue = 100000; break;
          case 100000:     dValue = 10000; break;          
          #endif
          
          #if PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_LONG || PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_INT
          case 10000: dValue = 1000; break;
          case 1000:  dValue = 100;  break;          
          #endif
          
          #if PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_LONG || PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_INT || PRINT_MAX_INT_TYPE == PRINT_INT_TYPE_BYTE
          case 100:   dValue = 10;   break;
          case 10:    dValue = 1;    break;
          #endif
          default:    dValue = 0;    break;          
        }
        break;
     #endif
    }
  }
  
  return 1;
}

#else

// This is the original printNumber from Arduino with just the small change to 
// allow you to set the integer type in use (see Print.h for how to set
// PRINT_MAX_INT_TYPE for your requirements in pins_arduino.h).
//
// This function allows arbitrary bases, but is very VERY heavy especially
// if you are not using division anywhere else in your program.
//
// VERY, VERY HEAVY

size_t Print::printNumber(UNSIGNED_PRINT_INT_TYPE n, uint8_t base) {
  
  // This is super wasteful because it assumes you are using binary in the 
  // worst case and makes a buffer big enough to show it in binary
  // which is 1 byte per bit
  char buf[8 * sizeof(n) + 1]; // Assumes 8-bit chars plus zero byte.
  
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;
    
  do {
    UNSIGNED_PRINT_INT_TYPE m = n;    
    n /= base;      
    char c = m - base * n;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);
     
  return write(str);

}
#endif

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
