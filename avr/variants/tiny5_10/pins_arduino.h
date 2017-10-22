/*
  pins_arduino.h - Pin definition functions for Arduino
  
    ATtiny5, ATtiny10
    
    6 Pin SOT23-6 
    8 Pin UDFN (with 2 pins NC)
      
  Notes
  --------------------------------------------------------------------
  The Tiny 4 and 9 do not have any ADC.
  
  The Tiny 4 and 5 only have 512 Bytes of Flash, so that's 
    *REALLY* tight to do much interesting, but still we can do blinky
    fading, input, and even serial... well, a byte or two if you don't
    want to do anything else!
  
  All four only have 32 bytes of SRAM.
  
  Licence
  --------------------------------------------------------------------
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

*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

// Initialisation Core Configuration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  USE_WIRING_LITE will cause wiring_lite.c to be used instead of wiring.c
//    this is a simpler, better system, but means more code is needed 
//    for each variant (as it should be IMHO) so the variant can do the 
//    chip specific stuff.
//
//  USE_NEW_MILLIS  will cause MillisMicrosDelay.c/h to be used, this is only
//    possible with USE_WIRING_LITE, it produces better accuracy with less 
//    code, and includes the extra feature of the REAL_MILLIS() macro.
//
//  REDUCED_CORE_TINYAVR is used at this stage by the new millis code to 
//    handle some stuff that has trouble with these new very tiny chips.
//
#define USE_WIRING_LITE  1
#define USE_NEW_MILLIS   1
#define REDUCED_CORE_TINYAVR 1

// TODO: Make this automatic on analogRead()?
#ifndef INITIALIZE_ANALOG_TO_DIGITAL_CONVERTER
  #define INITIALIZE_ANALOG_TO_DIGITAL_CONVERTER    1
#endif

// Print Support
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// For the 4/5 you really don't have a hope in hell of using
// print.  For the 9/10 maybe but probably not realistically.
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

#define PRINT_MAX_INT_TYPE  PRINT_INT_TYPE_INT
//#define PRINT_USE_BASE_BIN
//#define PRINT_USE_BASE_OCT
//#define PRINT_USE_BASE_DEC
//#define PRINT_USE_BASE_HEX
//#define PRINT_USE_BASE_ARBITRARY

// Tone Support
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Tone is not implemented on 4/5/9/10 yet.  With such low RAM (32 bytes)
//  even using a similar method to the Tiny13 tone might be too much.
#define NO_TONE                                   1

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
// #define USE_SERIAL_TYPE           SERIAL_TYPE_SOFTWARE
#define USE_SERIAL_TYPE              SERIAL_TYPE_HALF_DUPLEX
// #define HALF_DUPLEX_SERIAL_DISABLE_WRITE
// #define HALF_DUPLEX_SERIAL_DISABLE_READ


// The below would be used if we instead had
// #define USE_SERIAL_TYPE SERIAL_TYPE_SOFTWARE
// But the 4/5/9/10 is too small for even TinySoftwareSerial
// I'll leave the definitions here anyway.
#define ANALOG_COMP_DDR               DDRB
#define ANALOG_COMP_PORT              PORTB
#define ANALOG_COMP_PIN               PINB
#define ANALOG_COMP_AIN0_BIT            0
#define ANALOG_COMP_AIN1_BIT            1

// Analog reference bit masks.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This series of chips can only use Vcc as ref

#define DEFAULT (0)

// Set the Clock Source/Frequency
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This series of chips sets it's clock source and system clock prescaling
// at runtime as opposed to compile time (or by fuses).
#define setCPUFrequency(f) {                            \
  switch(f)\
  { \
    case 8000000UL: \
      CCP = 0xD8; \
      CLKPSR = 0; \
      break; \
    case 4000000UL: \
      CCP = 0xD8; \
      CLKPSR = 1; \
      break; \
    case 2000000UL: \
      CCP = 0xD8; \
      CLKPSR = 2; \
      break; \
    case 1000000UL: \
      CCP = 0xD8; \
      CLKPSR = 3; \
      break; \
    case  500000UL: \
      CCP = 0xD8; \
      CLKPSR = 4; \
      break; \
    case  250000UL: \
      CCP = 0xD8; \
      CLKPSR = 5; \
      break; \
    case  125000UL: \
      CCP = 0xD8; \
      CLKPSR = 6; \
      break; \
    case   62500UL: \
      CCP = 0xD8; \
      CLKPSR = 7; \
      break; \
    case   31250UL: \
      CCP = 0xD8; \
      CLKPSR = 8; \
      break; \
  } \
}

// PWM On/Off
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  for 4/5/9/10 this is simple since we only have 1 timer anyway
//  so doing it in macros, it uses less code space
//
//  For others, you might want to do something like
//  
//     #define turnOnPWM(t,v)  ( _turnOnPWM(t,v) )
//     void _turnOnPWM(uint8_t t, uint8_t v);
//
//     #define turnOffPWM(t) ( _turnOffPWM(t) )
//     void _turnOffPWM(uint8_t t);
//
//  and then create your _turnOnPWM function in pins_arduino.c


// For clarity our turnOnPWM() macro below uses turnOnPWMTimer() as part of it, to
// ensure that the timer is actually running before the pin is connected to it.
// other variants would probably roll this into their _turnOnPWM() function in 
// pins_arduino.c
#ifdef NO_MILLIS

// Fast PWM 8 Bit Mode, No Prescale
#define turnOnPWMTimer(t) ( \
      ( TCCR0A = ( TCCR0A & 0b11110000) | 0b00000001 ) \
  &&  ( TCCR0B = ( TCCR0B & 0b11100000) | 0b00001001 ) \
)

#else
// If millis() is enabled, then the timer is already running (we only have one)
#define turnOnPWMTimer(t) ( 1 )
#endif

#define ANALOG_WRITE_FLIPPED 1
#define turnOffPWM(t)  ( ( t==TIMER0A ) ? ( TCCR0A &= ~0B11000000 ) : ( TCCR0A &= ~0B00110000 ) )
#define turnOnPWM(t,v) ( turnOnPWMTimer(t) && ( t==TIMER0A ) ? ( ( TCCR0A |= 0B11000000 ) && ( OCR0A = v ) ) : ( ( TCCR0A |= 0B00110000 ) && ( OCR0B = v ) ) )

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
  TCCR0A |= _BV(WGM00); TCCR0B |= _BV(WGM02);  /* Fast PWM 8 Bit Mode */            \
  TIMSK0 |= _BV(TOIE0);             /* Enable Timer Overfow Interrupt 0 */          \
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
// Tiny 5/10: https://goo.gl/SKdLZP
// Tiny 4/9:  https://goo.gl/dL5WWZ
  
#define NUM_DIGITAL_PINS            4
#define NUM_ANALOG_INPUTS           4

// Notice here that p is the ADC reference and returns an Arduino
// pin number
#define analogInputToDigitalPin(p)  (p)

#define digitalPinHasPWM(p)         ((p) == 0 || (p) == 1)

// These are some convenient NAME => Arduino Digital Pin Number mappings
// None of these make sense for 4/5/9/10
// #define SS   ((uint8_t) 3)
// #define MOSI ((uint8_t) 0)
// #define MISO ((uint8_t) 1)
// #define SCK  ((uint8_t) 2)
// #define SDA  ((uint8_t) 0)
// #define SCL  ((uint8_t) 2)

// Analog Pins are set to ( 128 | [ADC] )
// so that they can be identified by pinMode, digitalRead/Write and analogRead/Write
static const uint8_t A0 = 0x80 | 0;
static const uint8_t A1 = 0x80 | 1;
static const uint8_t A2 = 0x80 | 2;
static const uint8_t A3 = 0x80 | 3;

// Pin Change Interrupt (PCI) Setup
#define digitalPinToPCICR(p)    (&PCICR)
#define digitalPinToPCICRbit(p) (0)
#define digitalPinToPCMSK(p)    (&PCMSK)
#define digitalPinToPCMSKbit(p) (p)

// We only have one port (PB) so we can simplify
// everything to save wasting flash
#undef  PB
#define PB 0

// Just to make sure we do not have these defined since these ports 
// do not exist
#undef PA
#undef PC
#undef PD

// Instead of the usual "arduino way" of using lookup tables, on these 
// we are going to use macros to derive information about a pin/port
// we can do this easily on the t13 because we only one port to worry about.
// 
// so we will undefine the standard "arduino way" and redefine our macros
#undef  digitalPinToPort
#define digitalPinToPort(P) ( (uint16_t)&DDRB )

#undef  digitalPinToBitMask
#define digitalPinToBitMask(P) ( (_BV(P)) )

#undef digitalPinToTimer
#define digitalPinToTimer(P) ( ( P == 0 ) ? TIMER0A : ( (P == 1 ) ? TIMER0B : NOT_ON_TIMER ))

#undef portOutputRegister
#define portOutputRegister(P) ( (volatile uint8_t *)(&PORTB))

#undef portInputRegister
#define portInputRegister(P) ( (volatile uint8_t *)(&PINB))

#undef portModeRegister
#define portModeRegister(P) ( (volatile uint8_t *)(&DDRB))

#undef pullupEnableRegister
#define pullupEnableRegister(P) ( (volatile uint8_t *)(&PUEB))

#endif
