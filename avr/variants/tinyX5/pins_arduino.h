/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

// We will use the lite version of wiring.c
//  - this is better for the tiny13
#define USE_WIRING_LITE  1
#define USE_NEW_MILLIS   1

// The tiny13 is very ram limited, so restrict the Print class
//  (and things so derived, like Serial) to only be able to 
//  print at most an int, rather than a long
//
// Also we have no chance to use arbitrary bases, while it 
// will happily fit in the flash, the SRAM will be exhausted
// the second you try and do anything with it, leading
// to a crash, or reset, or other randomness.
//
// Since Print.h defines BIN, OCT, DEC and HEX as the defaults
// we will leave all these commented out here, just including
// them in case you want to force-override
//
// You might use for example 
//    build.extra_flags=-DPRINT_USE_BASE_BIN -DPRINT_USE_BASE_DEC
// in your boards.txt
//

#define PRINT_MAX_INT_TYPE  PRINT_INT_TYPE_LONG
//#define PRINT_USE_BASE_BIN
//#define PRINT_USE_BASE_OCT
//#define PRINT_USE_BASE_DEC
//#define PRINT_USE_BASE_HEX
//#define PRINT_USE_BASE_ARBITRARY

// TODO: Make this automatic on analogRead()?
#ifndef INITIALIZE_ANALOG_TO_DIGITAL_CONVERTER
  #define INITIALIZE_ANALOG_TO_DIGITAL_CONVERTER    1
#endif

// Tone Support
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This will cause the core's default tone() [and anything supporting it]
// to go away and we will implement our own _tone() 
#if defined( __cplusplus ) && !defined( NO_TONE )
  // Note because we use default values for length and pin (as does the official
  // Arduino implementation) this only work in __cplusplus
  #define tone(...)   _tone(__VA_ARGS__)
  #define noTone(...) _noTone(__VA_ARGS__)
  void _tone(uint8_t pin, uint32_t frequency, uint32_t length = 0);
  void _noTone(uint8_t pin = 0);
#endif

// Serial Port Configuration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  we have 3 options for serial, hardware, software and half_duplex 
//  hardware and software are self explanitory, half_duplex is 
//  a very small memory/flash usage routine without any buffering
//  or interrupts really intended for sending or receiving minimal stuff
//  for helping with debugging.  
//
//  For half_duplex we also have the option to disable the *standard*
//  read() and/or write() functions in order to save space, however
//  even if disabled, you can use some alternative functions such
//  as read_byte() and write_byte() which can be optimized out if 
//  not used.
//

// #define USE_SERIAL_TYPE           SERIAL_TYPE_HARDWARE
#define USE_SERIAL_TYPE           SERIAL_TYPE_SOFTWARE
// #define USE_SERIAL_TYPE              SERIAL_TYPE_HALF_DUPLEX
// #define HALF_DUPLEX_SERIAL_DISABLE_WRITE
// #define HALF_DUPLEX_SERIAL_DISABLE_READ


// The below are used for SERIAL_TYPE_SOFTWARE
// #define USE_SERIAL_TYPE SERIAL_TYPE_SOFTWARE
// TX is on AIN0, RX is on AIN1
#define ANALOG_COMP_DDR               DDRB
#define ANALOG_COMP_PORT              PORTB
#define ANALOG_COMP_PIN               PINB
#define ANALOG_COMP_AIN0_BIT            0
#define ANALOG_COMP_AIN1_BIT            1

// Analog reference bit masks.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define DEFAULT     (0)           // VCC
#define EXTERNAL    (1)           // External on AREF (PB0)
#define INTERNAL    (2)           // Internal (1v1)
#define INTERNAL1V1 (2)
#define INTERNAL2V6 (6)           // Internal (2v6)
#define BYPASSED2V6 (7)           // Internal (2v6) but connected to PB0 
                                  //   which must have a bypass capacitor on it.

// PWM On/Off
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define turnOnPWM(t,v)  ( _turnOnPWM(t,v) )
void _turnOnPWM(uint8_t t, uint8_t v);

#define turnOffPWM(t) ( _turnOffPWM(t) )
void _turnOffPWM(uint8_t t);


// millis()/micros() Timer On/Off
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
// If the macros are not defined, a default implementation may be implemented
// if the macros are defined, then they will be used.
//
// Your macro may call a function which you can implement in pins_arduino.c
// or as I have done here for the t13, just do it directly in the macro to
// save some bytes of flash.  This "function" is only called once normally and
// not by users.

//#define turnOnMillis(prescale)  ( _turnOnMillis(prescale)  )
#define turnOffMillis()         ( _turnOffMillis() )
void _turnOnMillis(uint8_t prescale);
void _turnOffMillis();

#define turnOnMillis(PRESCALE)                                                      \
{                                                                                   \
  TCCR0A |= _BV(WGM00)|_BV(WGM01);  /* Fast PWM Mode */                             \
  TIMSK  |= _BV(TOIE0);             /* Enable Timer Overfow Interrupt 0 */          \
  switch(PRESCALE)                                                                  \
  {                                                                                 \
    case 1: TCCR0B |= _BV(CS00); break;               /* Timer On, No Prescale  */  \
    case 8: TCCR0B |= _BV(CS01); break;               /* Timer On, /8 Prescale  */  \
    case 64: TCCR0B |= _BV(CS01) | _BV(CS00); break;  /* Timer On, /64 Prescale */  \
  }                                                                                 \
  TCNT0=0;                          /* Start From Zero */                           \
}

// Arduino Pin Numbering to Chip's PORT.PIN and ADC Numbers
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// See PinMapping.jpg
  
#define NUM_DIGITAL_PINS            6
#define NUM_ANALOG_INPUTS           4

#define analogInputToDigitalPin(p)  (\
  ((p) == 3) ? 3 : (\
  ((p) == 1) ? 2 : (\
  ((p) == 2) ? 4 : (\
  ((p) == 0) ? 5 :  \
-1 ))))

#define digitalPinHasPWM(p)         ((p) == 0 || (p) == 1 || (p) == 4)

// These are some convenient NAME => Arduino Digital Pin Number mappings
#define SS   3
#define MOSI 0
#define MISO 1
#define SCK  2
#define SDA ((uint8_t) 0)
#define SCL ((uint8_t) 2)

// Analog Pin => ADC number, note that if  ANALOG_PINS_ARE_ADC_NUMBERS is not set
// then you need to add NUM_DIGITAL_PINS to the ADC number and it will be 
// subtracted when yo try to do analogRead() in order to get the ADC.
#define ANALOG_PINS_ARE_ADC_NUMBERS 1
#define A0 ((uint8_t) 0)
#define A1 ((uint8_t) 1)
#define A2 ((uint8_t) 2)
#define A3 ((uint8_t) 3)

// Pin Change Interrupt (PCI) Setup
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 5) ? (&GIMSK) : ((uint8_t *)NULL))
#define digitalPinToPCICRbit(p) 5
#define digitalPinToPCMSK(p)    (((p) >= 0 && (p) <= 5) ? (&PCMSK) : ((uint8_t *)NULL))
#define digitalPinToPCMSKbit(p) (p)


// The x5 is super small, we only have one port (PB) so we can simplify
// everything to save wasting flash
#undef  PB
#define PB 0

// Just to make sure we do not have these defined since these ports 
// do not exist on the x5
#undef PA
#undef PC
#undef PD

// Instead of the usual "arduino way" of using lookup tables, on the x5 
// we are going to use macros to derive information about a pin/port
// we can do this easily on the x5 because we only one port to worry about.
// 
// so we will undefine the standard "arduino way" and redefine our macros
#undef  digitalPinToPort
#define digitalPinToPort(P) ( (uint16_t)&DDRB )

#undef  digitalPinToBitMask
#define digitalPinToBitMask(P) ( (_BV(P)) )

#undef digitalPinToTimer
#define digitalPinToTimer(P) ( ( P == 0 ) ? TIMER0A : ( (P == 1 ) ? TIMER0B : ( (P == 4 ) ? TIMER1B : NOT_ON_TIMER ) ))

#undef portOutputRegister
#define portOutputRegister(P) ( (volatile uint8_t *)(&PORTB))

#undef portInputRegister
#define portInputRegister(P) ( (volatile uint8_t *)(&PINB))

#undef portModeRegister
#define portModeRegister(P) ( (volatile uint8_t *)(&DDRB))


#ifdef ARDUINO_MAIN
/*
 * With the above redefined macros, these are therefore not necessary.

 #include <avr/pgmspace.h>

const uint16_t PROGMEM port_to_mode_PGM[] = 
{
  NOT_A_PORT,
  NOT_A_PORT,
  (uint16_t)&DDRB,
};

const uint16_t PROGMEM port_to_output_PGM[] = 
{
  NOT_A_PORT,
  NOT_A_PORT,
  (uint16_t)&PORTB,
};

const uint16_t PROGMEM port_to_input_PGM[] = 
{
  NOT_A_PIN,
  NOT_A_PIN,
  (uint16_t)&PINB,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = 
{
  PB, 
  PB,
  PB,
  PB,
  PB, 
  PB, 

};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = 
{
  _BV(0), 
  _BV(1),
  _BV(2),
  _BV(3),
  _BV(4),
  _BV(5),

};


const uint8_t PROGMEM digital_pin_to_timer_PGM[] = 
{
  TIMER0A, // OC0A 
  TIMER0B, // OC0B 
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  TIMER1B,
  NOT_ON_TIMER,
};
*/

#endif

#endif
