#include "Arduino.h"

/**
 * turnOnPWM(), turnOffPWM() 
 * 
 * In this variant, the below functions are not used, because pins_arduino.h 
 *  defines macros to perform the action.
 * 
 * If you use the this variant to base a new variant on, note that in 
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

void _turnOnPWM(uint8_t t __attribute__((unused)), uint8_t v __attribute__((unused)))
{
  // Not Used, see pins_arduino.h for Macro Definition turnOnPWM()
}

void _turnOffPWM(uint8_t t __attribute__((unused)))
{
  // Not Used, see pins_arduino.h for Macro Definition turnOffPWM()
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

// In order to save bytes for this variant the following function has been rolled
// directly into the turnOnMillis() macro in pins_arduino.h, in other variants
// you might want to have your turnOnMillis() macro call a function instead
// if it's more complicated.
void _turnOnMillis(uint8_t prescale __attribute__((unused)))
{
  // Not used see pins_arduino.h for Macro Definition turnOnMillis()
}

// This is unlikely to be used by much (if anything), it's really just for completeness.
void _turnOffMillis()
{
  // Stop timer0 running, disable the interrupt
  TCCR0B &= ~(_BV(CS00)|_BV(CS01));  // Stop Timer  
  TIMSK0 &= ~_BV(TOIE0); // Disable Timer Overflow Interrupt 0  
}