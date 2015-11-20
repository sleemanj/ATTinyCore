/* Optimized AVR305 half-duplex serial uart implementation
 * timing for 81N, 115.2kbps @8Mhz = 69.4 cycles/bit
 * and @16Mhz = 138.9 cycles/bit
 * @author: Ralph Doncaster
 * @version: $Id$
 * 
 * http://nerdralph.blogspot.ca/2013/12/writing-avr-assembler-code-with-arduino.html
 * 
 * Modified by J.Sleeman (sparks@gogo.co.nz) to wrap into a very spartan stream
 */
#if (USE_SERIAL_TYPE == SERIAL_TYPE_TX_ONLY)
#ifndef BasicSerial_h
#define BasicSerial_h

#include "Arduino.h"
#include "Print.h"
#include "Stream.h"

#ifndef BAUD_RATE
  #ifndef F_CPU
    #error CPU frequency F_CPU undefined
  #endif
  #if F_CPU > 16000000L
    #define BAUD_RATE  115200  
//    #warning "BAUD_RATE for TX Only Serial set to 115200.  Use "#define BAUD_RATE xxxxxx" in your main sketch to override."
  #elif F_CPU > 9000000L
    #define BAUD_RATE  57600
//    #warning "BAUD_RATE for TX Only Serial set to 57600.   Use "#define BAUD_RATE xxxxxx" in your main sketch to override."
  #elif F_CPU > 5000000L
    #define BAUD_RATE 38400
//    #warning "BAUD_RATE for TX Only Serial set to 38400.   Use "#define BAUD_RATE xxxxxx" in your main sketch to override."
  #else
    #define BAUD_RATE 9600
//    #warning "BAUD_RATE for TX Only Serial set to 9600.    Use "#define BAUD_RATE xxxxxx" in your main sketch to override."
  #endif  
#endif

#ifdef F_CPU
  #define TXDELAY (((F_CPU/BAUD_RATE)-9)/3)
#else
  #error CPU frequency F_CPU undefined
#endif

#if TXDELAY > 255
  #error low baud rates unsupported - use higher BAUD_RATE
#endif

extern "C" {
void TxTimedByte(char, char);
}

#define TxByte(C) TxTimedByte(C , TXDELAY)

class BasicSerial : public Stream
{
  public:
    void begin(long);
    void end();
    int available(void);
    int peek(void); 
    int read(void); 
    void flush(void); 
    virtual size_t write(uint8_t);
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool();
};

extern BasicSerial Serial;

#endif
#endif