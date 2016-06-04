#include "Arduino.h"

/**
 * turnOnPWM(), turnOffPWM() 
 * 
 * In the ATTiny13, the below functions are not used, because pins_arduino.h 
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


/** 
 * millis()/micros() Overflow Timer, and turnOnMillis(), turnOffMillis()
 * 
 * The overflow counter (ovrf, implemented in MillisMicrosDelay.c) 
 * is incremented by our overflow timer, it is up to us to do this 
 * incrementing, as we are the file which knows how to operate the timers
 * on this particular chip!
 * 
 */

#if !(defined(NO_MILLIS) && NO_MILLIS) && defined(USE_NEW_MILLIS) && USE_NEW_MILLIS
extern volatile MillisMicrosTime_t ovrf;
 
ISR(TIM0_OVF_vect)
{
  ovrf++; //Increment counter every 256 clock cycles  
}
#endif

// In order to save bytes for the tiny13 the following function has been rolled
// directly into the turnOnMillis() macro in pins_arduino.h, in other variants
// you might want to have your turnOnMillis() macro call a function instead
// if it's more complicated.
void _turnOnMillis(uint8_t prescale)
{
  // Start timer0 running, setup the millis() interrupt to run
  switch(prescale)
  {
    case 1:
      TCCR0B |= _BV(CS00);              // Timer On, No Prescale
      break;
      
    case 8:
      TCCR0B |= _BV(CS01);              // Timer On, /8 Prescale
      break;
      
    case 64:
      TCCR0B |= _BV(CS01) | _BV(CS00);  // Timer On, /64 Prescale
      break;
  }
  
  TCCR0A |= _BV(WGM00)|_BV(WGM01);  // Fast PWM Mode
  TIMSK0 |= _BV(TOIE0);             // Enable Timer Overfow Interrupt 0
  TCNT0=0;                          // Start From Zero
}

// This is unlikely to be used by much (if anything), it's really just for completeness.
void _turnOffMillis()
{
  // Stop timer0 running, disable the interrupt
  TCCR0B &= ~_BV(CS00);  // Stop Timer  
  TIMSK0 &= ~_BV(TOIE0); // Disable Timer Overflow Interrupt 0  
}