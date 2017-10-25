/*
 * Copyright (c) 2012 by Thomas Carpenter
 *               2017 by James Sleeman
 * 
 * Software based SPI Master Library for Tiny core.
 * 
 * Modified 2017 by James Sleeman
 *   - Use only the "Hard Coded" (in pins_arduino.h for each variant) MOSI/MISO/SCK
 *   - Do not do anything with SS, leave this up to the user, this is Master only anyway
 *   - Optimise (for space not cycles) byte reversal with assembly
 *   - Remove direct port manipulation as when used with my ATTinyCore fork 
 *       these optimisations are done by the normal functions when given compile-time-constants
 *       (as they will be now since using hard coded MOSI/MISO/SCK)
 *   - Remove "noTransfer", default to transferMode0 to start with, and do not 
 *       change transfer mode on end()
 *   - Rename to VeryTinySoftwareSPI as indication of these changes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 *
 */

#include "VeryTinySoftwareSPI.h"

// We need to be able to handle both MSB First and MSB Last
// which means we need to flip the byte order, this is a space-efficient
// way to do this ( http://omarfrancisco.com/reversing-bits-in-a-byte/ )

inline static uint8_t reverseByte(uint8_t x)
{
  asm(
      "ldi        r25, 0x80\n" // Set r25 to 0b10000000
      "1:\n"                   // Label 1
      "rol        %0\n"        // Rotate x left 1 bit, bit 7 goes into carry and others move up
      "ror        r25\n"       // Rotate output (r25) right 1 bit, carry goes into bit 7, bit 0 goes into carry, others shift down
      "brcc       1b\n"        // If the carry is stil zero, loop to 1: (r25 started as 0b10000000 and those zeros a rippling down)
      "mov        %0, r25\n"   // Put output back into x
      : "=r" (x) /* Out */
      : "0"  (x) /* In  */
      : "r25"    /* Clobber */
   );

   return x;
}

VeryTinySoftSPIClass::VeryTinySoftSPIClass()
{
  _bitOrder = MSBFIRST;
  transferType = &VeryTinySoftSPIClass::transferMode0;
}

void VeryTinySoftSPIClass::begin()
{
		pinMode(MOSI, OUTPUT);
		pinMode(MISO, INPUT);
		pinMode(SCK, OUTPUT);
}

byte VeryTinySoftSPIClass::transferMode0(byte _data)
{
	byte _newData = 0;
	for (byte i = 0;i < 8; i++){
		if(_data & 0x80){
       digitalWrite(MOSI, HIGH);
			// *_MOSI_PORT |= _MOSI_HIGH;
		} else {
      digitalWrite(MOSI, LOW);
			//*_MOSI_PORT &= _MOSI_LOW;
		}
		_data <<= 1;
    digitalWrite(SCK, HIGH);
		//*_SCK_PORT |= _SCK_HIGH;
		_newData <<= 1;
		_newData |= digitalRead(MISO);//((*_MISO_PIN & _MISO_MASK) ? 1 : 0);
    digitalWrite(SCK, LOW);
		//*_SCK_PORT &= _SCK_LOW;
	}
	return _newData;
}

byte VeryTinySoftSPIClass::transferMode1(byte _data){
	byte _newData = 0;
	for (byte i = 0;i < 8; i++){
    digitalWrite(SCK, HIGH);
		// *_SCK_PORT |= _SCK_HIGH;
		if(_data & 0x80){
      digitalWrite(MOSI, HIGH);
			//*_MOSI_PORT |= _MOSI_HIGH;
		} else {
      digitalWrite(MOSI, LOW);
			//*_MOSI_PORT &= _MOSI_LOW;
		}
		_data <<= 1;
    digitalWrite(SCK, LOW);
		//*_SCK_PORT &= _SCK_LOW;
		_newData <<= 1;
		_newData |= digitalRead(MISO);//((*_MISO_PIN & _MISO_MASK) ? 1 : 0);
	}
	return _newData;
}

byte VeryTinySoftSPIClass::transferMode2(byte _data)
{
	byte _newData = 0;
	for (byte i = 0;i < 8; i++){
		if(_data & 0x80){
      digitalWrite(MOSI, HIGH);
			//*_MOSI_PORT |= _MOSI_HIGH;
		} else {
      digitalWrite(MOSI, LOW);
			//*_MOSI_PORT &= _MOSI_LOW;
		}
		_data <<= 1;
    digitalWrite(SCK,LOW);
		//*_SCK_PORT &= _SCK_LOW;
		_newData <<= 1;
		_newData |= digitalRead(MISO);//((*_MISO_PIN & _MISO_MASK) ? 1 : 0);
    digitalWrite(SCK, HIGH);
		//*_SCK_PORT |= _SCK_HIGH;
	}
	return _newData;
}

byte VeryTinySoftSPIClass::transferMode3(byte _data)
{
	byte _newData = 0;
	for (byte i = 0;i < 8; i++){
    digitalWrite(SCK, LOW);
		//*_SCK_PORT &= _SCK_LOW;
		if(_data & 0x80){
      digitalWrite(MOSI, HIGH);
			//*_MOSI_PORT |= _MOSI_HIGH;
		} else {
      digitalWrite(MOSI, LOW);
			//*_MOSI_PORT &= _MOSI_LOW;
		}
		_data <<= 1;
    digitalWrite(SCK, HIGH);
		//*_SCK_PORT |= _SCK_HIGH;
		_newData <<= 1;
		_newData |= digitalRead(MISO);//((*_MISO_PIN & _MISO_MASK) ? 1 : 0);
	}
	return _newData;
}

byte VeryTinySoftSPIClass::transfer(byte _data)
{
	byte _newData = 0;
  byte oldSREG = SREG;
	cli();

  // The transfer functions all do MSB FIRST (most common)
  // so flip in and out if we need LSB FIRST (least common)
  if((_bitOrder != MSBFIRST)) _data = reverseByte(_data);
  _data = (*this.*transferType)(_data);
  SREG = oldSREG;
  if((_bitOrder != MSBFIRST)) _data = reverseByte(_data);
  return _data;
}

void VeryTinySoftSPIClass::setBitOrder(uint8_t bitOrder)
{
	_bitOrder = bitOrder;
}

void VeryTinySoftSPIClass::setDataMode(uint8_t mode)
{	
  switch(mode)
  {
    case SPI_MODE1:
      transferType = &VeryTinySoftSPIClass::transferMode1;
      break;
    case SPI_MODE2:
      transferType = &VeryTinySoftSPIClass::transferMode2;
      break;
    case SPI_MODE3:
      transferType = &VeryTinySoftSPIClass::transferMode3;
      break;      
      
    case SPI_MODE0:
    default:
      transferType = &VeryTinySoftSPIClass::transferMode0;
      break;
  }
  
  if(mode & 0x02){
    digitalWrite(SCK, HIGH);
    //*_SCK_PORT |= _SCK_HIGH;
  } else {
    digitalWrite(SCK, LOW);
    //*_SCK_PORT &= _SCK_LOW;
  }
}

void VeryTinySoftSPIClass::setClockDivider(uint8_t rate)
{
  // Not Implemented, @TODO
  //
  // There is a good chance that the code is already so slow as to make
  // this pointless.
  //
  // Otherwise we would need to insert some delays in the transferModeN 
  // methods on both sides of the clock based on cpu frequency and  
  // expected clock counts for the loops themselves, pretty tricky.
  //
  // It would mean increase flash cost even if it wasn't needed
  // maybe if we had some special methods like ::delayClock8 and 
  // used function pointers like with transferModeN the optimiser
  // would be able to be smart about it and see that they were not
  // used if setClockDivider wasn't used.
}

void VeryTinySoftSPIClass::beginTransaction(SPISettings settings)
{
  _bitOrder = settings._bitOrder;
  setDataMode(settings._dataMode);
}

void VeryTinySoftSPIClass::endTransaction(void)
{
  // NOP
}


VeryTinySoftSPIClass SPI;