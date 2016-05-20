#include "Arduino.h"

/** In the ATTiny13, the below functions are not used, because pins_arduino.h 
 *  defines macros to perform the action.
 * 
 * If you use the tiny13 variant to base a new variant on, note that in 
 * pins_arduino.h you willl want to define the macros and function declarations
 * as...
 
     #define turnOnPWM(t,v)  ( _turnOnPWM(t,v) )
     void _turnOnPWM(uint8_t t, uint8_t v);
    
     #define turnOffPWM(t) ( _turnOffPWM(t) )
     void _turnOffPWM(uint8_t t);
  
 *  t = timer to use, v = value to write... the pin if the timer can 
 *    be connected to more than one, is up to you to decide, whatever you 
 *    specified in your pinout diagram :-)
 *
 *  If your variant doesn't specify these macros (turnOnPWM and turnOffPWM) 
 *   the core may or may not be able to do pwm, it will make an attempt anyway.
 */

void _turnOnPWM(uint8_t t, uint8_t v)
{
   // Ensure the given timer is switched on and set to fast pwm mode
   ( (  TCCR0B |= _BV(CS00) ) && ( TCCR0A |= _BV(WGM00)|_BV(WGM01) ) );
   
   // Ensure that the appropriate pin is connected to the given timer
   ( ( t==TIMER0A ) ? ( ( TCCR0A |= 0B10000000 ) && ( OCR0A = v ) ) : ( ( TCCR0A |= 0B00100000 ) && ( OCR0B = v ) ) );
}

void _turnOffPWM(uint8_t t)
{
  // Disconnect the pin from the timer
  ( ( t==TIMER0A ) ? ( TCCR0A &= ~0B11000000 ) : ( TCCR0A &= ~0B00110000 ) );  
}