/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis
  Copyright 2015~2018 Spence Konde

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

#define ATTINYX7 1       //backwards compat
#define __AVR_ATtinyX7__ //recommended

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            16
#define NUM_ANALOG_INPUTS           11
#define analogInputToDigitalPin(p)  (p<8?p:p+5)

#define ADC_TEMPERATURE 11

#define digitalPinHasPWM(p)         ((p) == 2 || (p) == 11 || (p) == 14)

#define SS   6
#define MOSI 4
#define MISO 2
#define SCK  5


#define USI_DDR_PORT DDRB
#define USI_SCK_PORT DDRB
#define USCK_DD_PIN DDA2
#define DO_DD_PIN DDA1
#define DI_DD_PIN DDA0


#  define DDR_USI DDRB
#  define PORT_USI PORTB
#  define PIN_USI PINB
#  define PORT_USI_SDA PORTB0
#  define PORT_USI_SCL PORTB2
#  define PIN_USI_SDA PINB0
#  define PIN_USI_SCL PINB2
#  define USI_START_VECTOR USI_START_vect
#  define USI_OVERFLOW_VECTOR USI_OVF_vect
#  define DDR_USI_CL DDR_USI
#  define PORT_USI_CL PORT_USI
#  define PIN_USI_CL PIN_USI
#ifndef USI_START_COND_INT
#  define USI_START_COND_INT USISIF
#endif

#define SDA 4
#define SCL 5

//Ax constants cannot be used for digitalRead/digitalWrite/analogWrite functions, only analogRead().
static const uint8_t A0 = 0x80 | 0;
static const uint8_t A1 = 0x80 | 1;
static const uint8_t A2 = 0x80 | 2;
static const uint8_t A3 = 0x80 | 3;
static const uint8_t A4 = 0x80 | 4;
static const uint8_t A5 = 0x80 | 5;
static const uint8_t A6 = 0x80 | 6;
static const uint8_t A7 = 0x80 | 7;
static const uint8_t A8 = 0x80 | 8;
static const uint8_t A9 = 0x80 | 9;
static const uint8_t A10 = 0x80 | 10;
#define LED_BUILTIN (3)

#define PIN_A0  ( 0)
#define PIN_A1  ( 1)
#define PIN_A2  ( 2)
#define PIN_A3  ( 3)
#define PIN_A4  ( 4)
#define PIN_A5  ( 5)
#define PIN_A6  ( 6)
#define PIN_A7  ( 7)
#define PIN_B0  ( 8)
#define PIN_B1  ( 9)
#define PIN_B2  ( 10)
#define PIN_B3  ( 11)
#define PIN_B4  ( 12)
#define PIN_B5  ( 13)
#define PIN_B6  ( 14)
#define PIN_B7  ( 15)


//----------------------------------------------------------
//----------------------------------------------------------
//Core Configuration (used to be in core_build_options.h)

//If Software Serial communications doesn't work, run the TinyTuner sketch provided with the core to give you a calibrated OSCCAL value.
//Change the value here with the tuned value.
//e.g
//#define TUNED_OSCCAL_VALUE                        0x57

//Choosing not to initialise saves power and flash. 1 = initialise.
#define INITIALIZE_ANALOG_TO_DIGITAL_CONVERTER    1
#define INITIALIZE_SECONDARY_TIMERS               1

#define TIMER_TO_USE_FOR_MILLIS                   0

#define HAVE_BOOTLOADER                           1

/*
  Where to put the software serial? (Arduino Digital pin numbers)
*/
//WARNING, if using software, TX is on AIN0, RX is on AIN1. Comparator is favoured to use its interrupt for the RX pin.
#define USE_SOFTWARE_SERIAL           0
//Please define the port on which the analog comparator is found.
#define ANALOG_COMP_DDR               DDRA
#define ANALOG_COMP_PORT              PORTA
#define ANALOG_COMP_PIN               PINA
#define ANALOG_COMP_AIN0_BIT          6
#define ANALOG_COMP_AIN1_BIT          7

/*
  Analog reference bit masks.
*/
// VCC used as analog reference, disconnected from PA0 (AREF)
#define DEFAULT (0)
// External voltage reference at PA0 (AREF) pin, internal reference turned off
#define EXTERNAL (1)
// Internal 1.1V voltage reference
#define INTERNAL (2)
#define INTERNAL1V1 INTERNAL
#define INTERNAL2V56 (7)


//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------



#define digitalPinToPCICR(p)    (&PCICR)
#define digitalPinToPCICRbit(p) ((p) >= 8 ? 1 : 0)
#define digitalPinToPCMSK(p)    ((p) >= 8 ?(&PCMSK1) : (&PCMSK0))
#define digitalPinToPCMSKbit(p) (p&15)

#define digitalPinToInterrupt(p)  ((p) == 14 ? 0 : ((p)==3?1: NOT_AN_INTERRUPT))
#ifdef ARDUINO_MAIN

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATTINY167
//
//                   +-\/-+
// RX   (D  0) PA0  1|    |20  PB0 (D  8)
// TX   (D  1) PA1  2|    |19  PB1 (D  9)
//     *(D  2) PA2  3|    |18  PB2 (D 10)
// INT1 (D  3) PA3  4|    |17  PB3 (D 11)*
//            AVCC  5|    |16  GND
//            AGND  6|    |15  VCC
//      (D  4) PA4  7|    |14  PB4 (D 12)
//      (D  5) PA5  8|    |13  PB5 (D 13)
//      (D  6) PA6  9|    |12  PB6 (D 14)* INT0
//      (D  7) PA7 10|    |11  PB7 (D 15)
//                   +----+
//
// * indicates PWM pin.

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] =
{
  NOT_A_PORT,
  (uint16_t)&DDRA,
  (uint16_t)&DDRB,
};

const uint16_t PROGMEM port_to_output_PGM[] =
{
  NOT_A_PORT,
  (uint16_t)&PORTA,
  (uint16_t)&PORTB,
};

const uint16_t PROGMEM port_to_input_PGM[] =
{
  NOT_A_PORT,
  (uint16_t)&PINA,
  (uint16_t)&PINB,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
  PA, /* 0 */
  PA,
  PA, /* 2 */
  PA, /* 3 */
  PA, /* 4 */
  PA,
  PA,
  PA,
  PB,
  PB,
  PB, /* 10 */
  PB,
  PB,
  PB,
  PB,
  PB, /* 15 */
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
  _BV(0), /* 0 */
  _BV(1),
  _BV(2), /* 2 */
  _BV(3), /* 3 */
  _BV(4), /* 4 */
  _BV(5),
  _BV(6),
  _BV(7),
  _BV(0),
  _BV(1),
  _BV(2), /* 10 */
  _BV(3),
  _BV(4),
  _BV(5),
  _BV(6),
  _BV(7), /* 15 */
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  TIMER0A,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  TIMER1B,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  TIMER1A,
  NOT_ON_TIMER,
};

#endif

#endif




//Old code, just here for temporary backup until I decide it is not needed.
//WARNING, if using software, RX must be on a pin which has a Pin change interrupt <= 7 (e.g. PCINT6, or PCINT1, but not PCINT8)
/*#define USE_SOFTWARE_SERIAL             1
//These are set to match Optiboot pins.

#define SOFTWARE_SERIAL_PORT            PORTB
#define SOFTWARE_SERIAL_TX              0
#define SOFTWARE_SERIAL_PIN             PINB
#define SOFTWARE_SERIAL_RX              1*/
