/*
  SoftWire.h - A Wire compatible wrapper for SoftI2CMaster
  Copyright (c) 2016 Bernhard Nebel.

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
*/

#ifndef _TinySoftWire_h
#define _TinySoftWire_h

#include <Arduino.h>

#define WIRE_BIT_NUM(NUM) \
  (uint8_t) ((NUM & 0b10000000) ? 7 : \
  ((NUM & 0b01000000) ? 6 : \
  ((NUM & 0b00100000) ? 5 : \
  ((NUM & 0b00010000) ? 4 : \
  ((NUM & 0b00001000) ? 3 : \
  ((NUM & 0b00000100) ? 2 : \
  ((NUM & 0b00000010) ? 1 : \
  ((NUM & 0b00000001) ? 0 : 0))))))))
  

#define SDA_PIN  WIRE_BIT_NUM(digitalPinToBitMask(SDA))
#define SCL_PIN  WIRE_BIT_NUM(digitalPinToBitMask(SCL))
#define SDA_PORT *portOutputRegister(digitalPinToPort(SDA))
#define SCL_PORT *portOutputRegister(digitalPinToPort(SCL))

#warning "SDA: " WIRE_BIT_NUM(digitalPinToBitMask(SDA))
#warning "SCL: " WIRE_BIT_NUM(digitalPinToBitMask(SCL))

#include <TinySoftI2CMaster.h>
#include <inttypes.h>
#include "Stream.h"

#ifndef WIRE_RX_BUFFER_LENGTH
  #define WIRE_RX_BUFFER_LENGTH  8
#endif

// WIRE_HAS_END means Wire has end()
#define WIRE_HAS_END 1

class TinySoftWire : public Stream
{
private:
  // If WIRE_RX_BUFFER_LENGTH == 0, no RX is compiled
  #if WIRE_RX_BUFFER_LENGTH > 0
    uint8_t rxBuffer[WIRE_RX_BUFFER_LENGTH];
    uint8_t rxBufferIndex;
    uint8_t rxBufferLength;
  #endif
    
  uint8_t transmitting;
  uint8_t error;
  
public:
  TinySoftWire(void);
  
  void begin(void);  
  void end(void);
  void setClock(uint32_t _);

  void beginTransmission(uint8_t address);  
  void beginTransmission(int address);

  uint8_t endTransmission(uint8_t sendStop);

  //	This provides backwards compatibility with the original
  //	definition, and expected behaviour, of endTransmission
  //
  uint8_t endTransmission(void);

  size_t write(uint8_t data);
  size_t write(const uint8_t *data, size_t quantity);
  inline size_t write(unsigned long n) { return write((uint8_t)n); }
  inline size_t write(long n) { return write((uint8_t)n); }
  inline size_t write(unsigned int n) { return write((uint8_t)n); }
  inline size_t write(int n) { return write((uint8_t)n); }
  using Print::write;
  
  #if WIRE_RX_BUFFER_LENGTH > 0
    uint8_t requestFrom(uint8_t address, uint8_t quantity,
            uint32_t iaddress, uint8_t isize, uint8_t sendStop);
    
    // These should probably be inline'd here, but LTO should take care of it
    // anyway, they just call the full requestFrom() with various defaults
    uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop);
    uint8_t requestFrom(int address, int quantity, int sendStop);
    uint8_t requestFrom(uint8_t address, uint8_t quantity);
    uint8_t requestFrom(int address, int quantity);

    int available(void);

    int read(void);
    int peek(void);
  #endif
  
  void flush(void) {
  }

};


#endif
