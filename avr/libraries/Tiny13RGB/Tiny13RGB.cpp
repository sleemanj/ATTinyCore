#include "Arduino.h"
#include "Tiny13RGB.h"

// This is just nomenclature for this file.
const  uint8_t CHANNELA = 0;
const  uint8_t CHANNELB = 1;
const  uint8_t CHANNELC = 2;

// This array holds the current PWM settings
volatile uint8_t channelPWM[3] = { 0, 0, 0 };

ISR(TIM0_COMPA_vect) 
{  
  static volatile byte tic  = 0;
  tic++;

  if(tic<channelPWM[CHANNELA])  PORTB |= _BV(CHANNELA); 
  else          PORTB &= ~_BV(CHANNELA);

  if(tic<channelPWM[CHANNELB])  PORTB |= _BV(CHANNELB); 
  else          PORTB &= ~_BV(CHANNELB);
  
  if(tic<channelPWM[CHANNELC])  PORTB |= _BV(CHANNELC); 
  else          PORTB &= ~_BV(CHANNELC);
}

// You can use this just as you would analogWrite()
void Tiny13RGB::analogWrite(uint8_t channel, uint8_t val)
{
  channelPWM[channel] = val;
}

// You can use this to throb one of those leds, that means fade it up,
// and then back down.
void Tiny13RGB::analogThrob(uint8_t channel, uint8_t spd)
{
  uint8_t i;
  
  for( i = 0; i < 255; i+=5)
  {        
    analogWrite(channel, i);
    delay(spd);      
  }
  
  for(i = 255;i > 0; i-=5)
  {      
    analogWrite(channel, i);
    delay(spd);
  }   
  analogWrite(channel, 0);
}

void Tiny13RGB::begin()
{
  PORTB &= ~0b00000111; // Set 0,1,2 LOW
  DDRB  |= 0b00000111; // Enable output for pins 0,1,2
  
  // The Tiny13 only has one interrupt, if millis is enabled it almost
  // certainly brings a prescaler along with it, in which case it would
  // make our software pwm run too slowly.  Therefore we disable millis.
  #if MILLIS_TIMER_PRESCALE > 1
    turnOffMillis();  
  #endif
    
  OCR0A   = 255;         // Interrupt fires when counter reaches this number (8 bit timer)
  TIMSK0 |= _BV(OCIE0A); // Enable compare interupt
  TCCR0B |= _BV(CS00);   // Enable timer with no prescaling
  
  // Enable interrupts
  sei();  
}


uint8_t Tiny13RGB::analogWriteTowards(uint8_t channel, uint8_t target)
{      
  if(target == channelPWM[channel])
  {    
    // When we reach the target, return 0 
    return 0;
  }
  else if(target > channelPWM[channel])
  {
    // If the target is higher than the current pwm level, increase 
    // the pwm level.
    channelPWM[channel]++;
  }
  else 
  {
    // Otherwise decrease the pwm level.
    channelPWM[channel]--;
  }  
  
  return 1;
}