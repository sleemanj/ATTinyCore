/*
  BasicSerial.cpp - Stream Wrapper for Raplh Doncaster's 
    BasicSerial function for TX only transmission in very small
    avr.
    
  Wrapper by J.Sleeman (sparks@gogo.co.nz), Derived from TinySoftwareSerial

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

#include "Arduino.h"

#if (USE_SERIAL_TYPE == SERIAL_TYPE_TX_ONLY)
#include "BasicSerial.h"


// Public Methods //////////////////////////////////////////////////////////////

void BasicSerial::begin(long baud)
{
  // We can not change baud on the fly.  
}

void BasicSerial::end()
{
  // We can not end
}

int BasicSerial::available(void)
{
  // There is never anything available, we are TX only
  return 0;
}


int BasicSerial::peek(void)
{
  // TX only
  return -1;
}

int BasicSerial::read(void)
{
  // Yeah, I can't read.
  return -1;  
}

size_t BasicSerial::write(uint8_t ch)
{
  // Wohoo, write is somethng I can do!
  TxByte(ch);
  return 1;
}

void BasicSerial::flush()
{
  // Nothing to do here
}

BasicSerial::operator bool() {
  // ??
	return true;
}

BasicSerial Serial;
#endif // whole file
