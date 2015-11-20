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

#define USE_SOFTWARE_SPI 1
#define USE_WIRING_LITE  1

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            6
#define NUM_ANALOG_INPUTS           4
#define analogInputToDigitalPin(p)  (((p) == 0) ? 5 : (((p) == 1) ? 2 : (((p) == 2) ? 4 :(((p) == 3) ? 3 : -1))))

#define digitalPinHasPWM(p)         ((p) == 0 || (p) == 1)

#define SS   3
#define MOSI 0
#define MISO 1
#define SCK  2

static const uint8_t SDA = 0;
static const uint8_t SCL = 2;

#define ANALOG_PINS_ARE_ADC_NUMBERS 1
static const uint8_t A0 = 0;
static const uint8_t A1 = 1;
static const uint8_t A2 = 2;
static const uint8_t A3 = 3;


//----------------------------------------------------------
//----------------------------------------------------------
//Core Configuration (used to be in core_build_options.h)

//If Software Serial communications doesn't work, run the TinyTuner sketch provided with the core to give you a calibrated OSCCAL value.
//Change the value here with the tuned value. By default this option uses the default value which the compiler will optimise out. 
#define TUNED_OSCCAL_VALUE                        OSCCAL
//e.g
//#define TUNED_OSCCAL_VALUE                        0x57


//Choosing not to initialise saves power and flash. 1 = initialise.
#define INITIALIZE_ANALOG_TO_DIGITAL_CONVERTER    0
#define INITIALIZE_SECONDARY_TIMERS               1

//  Tiny13 only has one timer, we wont have tone() functions then
#define TIMER_TO_USE_FOR_MILLIS                   0
#define NO_TONE                                   1

/*
  Where to put the software serial? (Arduino Digital pin numbers)
*/
//WARNING, if using software, TX is on AIN0, RX is on AIN1. Comparator is favoured to use its interrupt for the RX pin.
#define USE_SERIAL_TYPE              SERIAL_TYPE_TX_ONLY

// The below would be used if we instead had
// #define USE_SERIAL_TYPE SERIAL_TYPE_SOFTWARE
// But the t13 is too small for even TinySoftwareSerial
// I'll leave the definitions here anyway.
#define ANALOG_COMP_DDR               DDRB
#define ANALOG_COMP_PORT              PORTB
#define ANALOG_COMP_PIN               PINB
#define ANALOG_COMP_AIN0_BIT            0
#define ANALOG_COMP_AIN1_BIT            1

/*
  Analog reference bit masks.
*/
#define DEFAULT (0)
#define INTERNAL (1)
#define INTERNAL1V1 INTERNAL

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------


#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 5) ? (&GIMSK) : ((uint8_t *)NULL))
#define digitalPinToPCICRbit(p) 5
#define digitalPinToPCMSK(p)    (((p) >= 0 && (p) <= 5) ? (&PCMSK) : ((uint8_t *)NULL))
#define digitalPinToPCMSKbit(p) (p)



// ATMEL ATTINY13
//
//                    +-\/-+
//  A0 D5   ~RESET~  1|    |8   VCC
//  CLKI      A3 D3  2|    |7   D2 A1     SCK
//            A2 D4  3|    |6   D1    PWM MISO RX* INT0
//              GND  4|    |5   D0    PWM MOSI TX*
//                    +----+
//
// * Software Serial (not a hardware uart)


// The t13 is super small, we only have one port (PB) so we can simplify
// everything to save wasting flash
#undef  PB
#define PB 0

#undef PA
#undef PC
#undef PD

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

#define turnOffPWM(t)  ( ( t==TIMER0A ) ? ( TCCR0A &= ~0B11000000 ) : ( TCCR0A &= ~0B00110000 ) )
#define turnOnPWM(t,v) ( ( t==TIMER0A ) ? ( ( TCCR0A |= 0B11000000 ) && ( OCR0A = v ) ) : ( ( TCCR0A |= 0B00110000 ) && ( OCR0B = v ) ) )

#ifdef ARDUINO_MAIN
/*
 * With the above redefined macros, these are therefore not necessary.
 
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
  NOT_ON_TIMER,
  NOT_ON_TIMER,
};
*/

#endif

#endif
