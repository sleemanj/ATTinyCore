/** Simplified Tone "Library"
 * 
 * The existing Tone.cpp in the core is a complete and utter mess, once again the
 * developers over the years have just used a load of arcane #ifdef's 
 * to try and work out what they should do for a specific chip.
 * 
 * Which pretty much makes it totally completely unreasonably unreadable.
 * 
 * So we simplify by replacing the existing Tone with our own, we are not 
 * composing a grapnd symphony here, let's face it, blurting out Mario Bros
 * is about the most people use tone() for!
 *  
 * @author James Sleeman <james@gogo.co.nz>
 * @license The MIT License (MIT) [See At End Of File]
 *  
 */

#include "Arduino.h"

#if !defined(NO_TONE)

static  volatile uint32_t  CurrentToneDuration = 0;
static  volatile uint8_t   CurrentTonePin      = 255;
static  volatile uint8_t   CurrentToneMidpoint = 0;

void _toneRaw(uint8_t pin, uint8_t midPoint, uint32_t lengthTicks, uint8_t prescaleBitMask) 
{

  // Because the t13 is so limited in flash space, this really is little more than
  // a because-I-can excercise in futility.
  //
  // Because we can't do division (way too heavy) an inline function is used to 
  // do pre-calculation of the midPoint (of the timer, where we toggle the pin)
  // the number of ticks, and the prescale bitmask.  Naturally this is only 
  // going to work when tone is fed a constant frequency and length!
  
  // Because we only have one timer, we can't just use millis() to 
  // do tone duration, since it won't be accurate.
  //
  // So instead we calculate how many ticks the tone should go for and subtract
  // ticks as we hit the mid-point.
    
  CurrentToneDuration = lengthTicks;

  // Are we already playing that tone, if so, just keep doing that
  // otherwise we would make a clicking sound.
  if(pin == CurrentTonePin && midPoint == CurrentToneMidpoint) return;
  
  // The official Arduino tone() sets it as output for you
  //  so we will also.
  if(pin != CurrentTonePin)
  {
    CurrentTonePin      = pin;
    pinMode(pin, OUTPUT);    
  }
    
  // Shut down interrupts while we fiddle about with the timer.
  cli();
  
  TCCR0B &= ~0b111; // Turn off the timer before changing anytning
  TCNT0   = 0;      // Timer counter back to zero
  
  // Set the comparison, we will flip the bit every time this is hit      
  //  (actually, this will set TOP of the timer and we flip on the overflow) 
  OCR0A = midPoint;
  CurrentToneMidpoint = midPoint;
  
  // Enable the overflow interrupt
  TIMSK0 |= _BV(OCIE0A);
  
  // Start playing and record time
  digitalWrite(pin, HIGH);   
  
  // Start the timer    
  TCCR0A = 0b00000011;  
  TCCR0B = 0b00001000 | prescaleBitMask;
  
  sei(); // We **have** to enable interrupts here even if they were disabled coming in,
         //  otherwise it's not going to do anything.  Hence not saving/restoring SREG.
}

void _noTone(uint8_t pin) 
{
  // Disable the interrupt
  //  Note we can leave the rest of the timer setup as is, turnOnPWM() will
  //  fix it for itself next time you analogWrite() if you need to.
  TIMSK0   &= ~_BV(OCIE0A);
  
  // Pin goes back to input state  
  pinMode(pin == 255 ? CurrentTonePin : pin, INPUT);
  
  // And make sure we will reset it to output state next time you call
  // tone() by treating it as a new pin
  CurrentTonePin      = 255;
  
  #ifndef NO_MILLIS
    turnOnMillis(MILLIS_TIMER_PRESCALE);
  #endif
}

ISR(TIMER0_COMPA_vect) 
{  
  // Toggle the pin, most AVR can toggle an output pin by writing a 1 to the input 
  // register bit for that pin.
  *(portInputRegister(digitalPinToPort(CurrentTonePin))) = digitalPinToBitMask(CurrentTonePin);
  
  
  // If we have played this tone for the requested duration, stop playing it.
  if (CurrentToneDuration < CurrentToneMidpoint)
  {
    _noTone(); 
  }
  
  CurrentToneDuration -= CurrentToneMidpoint;
  TCNT0 = 0; // Restart timer
}

#endif

/*
  The MIT License (MIT)

  Copyright (c) 2016 James Sleeman

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/