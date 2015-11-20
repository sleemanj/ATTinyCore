/*
 * This is a "lite" version of wiring, using parts from core13 (especially millis() implementation) and parts from
 * the normal wiring.
 * 
 * The code usage is considerably less than a standard wiring, but I think that accuracy of timing may be
 * somewhat iffy, lucky then that most people don't need accurate timing.
 * 
 * I havn't genericized the timer names etc, this "lite" wiring is probably only really useful
 * for tiny13 users anyway.
 * 
*** Core13 ***
Arduino core designed for Attiny13 and similar devices.
NO WARRANTEE OR GUARANTEES!
Written by John "smeezekitty" 
You are free to use, redistribute and modify at will EXCEPT IF MARKED OTHERWISE IN A PARTICULAR SOURCE FILE!
Version 0.20
*/

#include "Arduino.h"

#ifdef USE_WIRING_LITE
#include "wiring_private.h"

#ifndef NO_MILLIS
#include <avr/interrupt.h>
volatile unsigned long ovrf=0;
ISR(TIM0_OVF_vect){
  ovrf++; //Increment counter every 256 clock cycles
}
unsigned long millis(){
  unsigned long x;
  asm("cli"); 
  /*Scale number of timer overflows to milliseconds*/
  #if F_CPU == 128000
  x = ovrf * 2;
    #elif F_CPU == 600000
  x = ovrf / 2;
  #elif F_CPU == 1000000
  x = ovrf / 4;
  #elif F_CPU == 1200000
  x = ovrf / 5;
  #elif F_CPU == 4000000
  x = ovrf / 16;
  #elif F_CPU == 4800000
  x = ovrf / 19;
  #elif F_CPU == 8000000
  x = ovrf / 31;
  #elif F_CPU == 9600000
  x = ovrf / 37;
    #elif F_CPU == 10000000
  x = ovrf / 39;
  #elif F_CPU == 12000000
  x = ovrf / 47;
  #elif F_CPU == 16000000
  x = ovrf / 63;
  #else
  #error This CPU frequency is not defined
  #endif
  asm("sei");
  return x;
}
/*The following improved micros() code was contributed by a sourceforge user "BBC25185" Thanks!*/
unsigned long micros(){
  unsigned long x;
  asm("cli");
  #if F_CPU == 128000
  x = ovrf * 2000;
  #elif F_CPU == 600000
  x = ovrf * 427;
  #elif F_CPU == 1000000
  x = ovrf * 256;
  #elif F_CPU == 1200000
  x = ovrf * 213;
  #elif F_CPU == 4000000
  x = ovrf * 64;
  #elif F_CPU == 4800000
  x = ovrf * 53;
  #elif F_CPU == 8000000
  x = ovrf * 32;
  #elif F_CPU == 9600000
  x = ovrf * 27;
  #elif F_CPU == 10000000
  x = ovrf * 26;
  #elif F_CPU == 12000000
  x = ovrf * 21;
  #elif F_CPU == 16000000
  x = ovrf * 16;
  #else 
  #error This CPU frequency is not defined
  #endif
  asm("sei");
  return x; 
}
#endif

void delay(unsigned long ms){
  while(ms--){
    delayMicroseconds(1000); 
  }
}

/* Delay for the given number of microseconds.  Assumes a 1, 8, 12, 16, 20 or 24 MHz clock. */
void delayMicroseconds(unsigned int us)
{
  // call = 4 cycles + 2 to 4 cycles to init us(2 for constant delay, 4 for variable)
  
  // calling avrlib's delay_us() function with low values (e.g. 1 or
  // 2 microseconds) gives delays longer than desired.
  //delay_us(us);
#if F_CPU >= 24000000L
  // for the 24 MHz clock for the aventurous ones, trying to overclock

  // zero delay fix
  if (!us) return; //  = 3 cycles, (4 when true)

  // the following loop takes a 1/6 of a microsecond (4 cycles)
  // per iteration, so execute it six times for each microsecond of
  // delay requested.
  us *= 6; // x6 us, = 7 cycles

  // account for the time taken in the preceeding commands.
  // we just burned 22 (24) cycles above, remove 5, (5*4=20)
  // us is at least 6 so we can substract 5
  us -= 5; //=2 cycles

#elif F_CPU >= 20000000L
  // for the 20 MHz clock on rare Arduino boards

  // for a one-microsecond delay, simply return.  the overhead
  // of the function call takes 18 (20) cycles, which is 1us
  __asm__ __volatile__ (
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop"); //just waiting 4 cycles
  if (us <= 1) return; //  = 3 cycles, (4 when true)

  // the following loop takes a 1/5 of a microsecond (4 cycles)
  // per iteration, so execute it five times for each microsecond of
  // delay requested.
  us = (us << 2) + us; // x5 us, = 7 cycles

  // account for the time taken in the preceeding commands.
  // we just burned 26 (28) cycles above, remove 7, (7*4=28)
  // us is at least 10 so we can substract 7
  us -= 7; // 2 cycles

#elif F_CPU >= 16000000L
  // for the 16 MHz clock on most Arduino boards

  // for a one-microsecond delay, simply return.  the overhead
  // of the function call takes 14 (16) cycles, which is 1us
  if (us <= 1) return; //  = 3 cycles, (4 when true)

  // the following loop takes 1/4 of a microsecond (4 cycles)
  // per iteration, so execute it four times for each microsecond of
  // delay requested.
  us <<= 2; // x4 us, = 4 cycles

  // account for the time taken in the preceeding commands.
  // we just burned 19 (21) cycles above, remove 5, (5*4=20)
  // us is at least 8 so we can substract 5
  us -= 5; // = 2 cycles, 

#elif F_CPU >= 12000000L
  // for the 12 MHz clock if somebody is working with USB

  // for a 1 microsecond delay, simply return.  the overhead
  // of the function call takes 14 (16) cycles, which is 1.5us
  if (us <= 1) return; //  = 3 cycles, (4 when true)

  // the following loop takes 1/3 of a microsecond (4 cycles)
  // per iteration, so execute it three times for each microsecond of
  // delay requested.
  us = (us << 1) + us; // x3 us, = 5 cycles

  // account for the time taken in the preceeding commands.
  // we just burned 20 (22) cycles above, remove 5, (5*4=20)
  // us is at least 6 so we can substract 5
  us -= 5; //2 cycles

#elif F_CPU >= 8000000L
  // for the 8 MHz internal clock

  // for a 1 and 2 microsecond delay, simply return.  the overhead
  // of the function call takes 14 (16) cycles, which is 2us
  if (us <= 2) return; //  = 3 cycles, (4 when true)

  // the following loop takes 1/2 of a microsecond (4 cycles)
  // per iteration, so execute it twice for each microsecond of
  // delay requested.
  us <<= 1; //x2 us, = 2 cycles

  // account for the time taken in the preceeding commands.
  // we just burned 17 (19) cycles above, remove 4, (4*4=16)
  // us is at least 6 so we can substract 4
  us -= 4; // = 2 cycles
#elif F_CPU >= 6000000L
  // for that unusual 6mhz clock... 

  // for a 1 and 2 microsecond delay, simply return.  the overhead
  // of the function call takes 14 (16) cycles, which is 2us
  if (us <= 2) return; //  = 3 cycles, (4 when true)

  // the following loop takes 2/3rd microsecond (4 cycles)
  // per iteration, so we want to add it to half of itself
  us +=us>>1;
  us -= 2; // = 2 cycles

#elif F_CPU >= 4000000L
  // for that unusual 4mhz clock... 

  // for a 1 and 2 microsecond delay, simply return.  the overhead
  // of the function call takes 14 (16) cycles, which is 2us
  if (us <= 2) return; //  = 3 cycles, (4 when true)

  // the following loop takes 1 microsecond (4 cycles)
  // per iteration, so nothing to do here! \o/

  us -= 2; // = 2 cycles


#else
  // for the 1 MHz internal clock (default settings for common AVR microcontrollers)

  // the overhead of the function calls is 14 (16) cycles
  if (us <= 16) return; //= 3 cycles, (4 when true)
  if (us <= 25) return; //= 3 cycles, (4 when true), (must be at least 25 if we want to substract 22)

  // compensate for the time taken by the preceeding and next commands (about 22 cycles)
  us -= 22; // = 2 cycles
  // the following loop takes 4 microseconds (4 cycles)
  // per iteration, so execute it us/4 times
  // us is at least 4, divided by 4 gives us 1 (no zero delay bug)
  us >>= 2; // us div 4, = 4 cycles
  

#endif

  // busy wait
  __asm__ __volatile__ (
    "1: sbiw %0,1" "\n\t" // 2 cycles
    "brne 1b" : "=w" (us) : "0" (us) // 2 cycles
  );
  // return = 4 cycles
}


void init(){  
#ifndef NO_MILLIS
  // Start timer0 running, setup the millis() interrupt to run
  TCCR0B |= _BV(CS00);
  TCCR0A |= _BV(WGM00)|_BV(WGM01);
  TIMSK0 |= 2;
  TCNT0=0;   
#else
  #ifndef turnOnPWM
  // Enabled fast PWM on the timer (not connected to the pin, that happens in 
  // analogWrite()
  //
  // if there is a turnOnPWM(t,v) macro, then we don't need to do this, because
  // turnOnPWM() should ensure it has been done when it does that
  TCCR0B |= _BV(CS00);
  TCCR0A |= _BV(WGM00)|_BV(WGM01);
  #endif
#endif
  
  sei();
  
  ADMUX=0;
  //Set up ADC clock depending on F_CPU
  #if F_CPU == 128000
  ADCSRA |= _BV(ADEN);
  #elif F_CPU == 1000000 || F_CPU == 1200000 || F_CPU == 600000
  ADCSRA |= _BV(ADEN) | _BV(ADPS1);
  #else
  ADCSRA |= _BV(ADEN) | _BV(ADPS1) | _BV(ADPS0) | _BV(ADPS2);
  #endif
}
#endif