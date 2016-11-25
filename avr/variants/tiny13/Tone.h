/** Tiny13 Tone
 * 
 * This header contains an inline function which stands in for tone()
 * (see pins_ardiuno.h for how exactly that happens), precalculates
 * necessary numbers and pases them into _toneRaw() which is 
 * implemened in Tone.cpp 
 * 
 * @author James Sleeman <james@gogo.co.nz>
 * @license The MIT License (MIT) [See At End Of File]
 *  
 */

// We may need to limit the range in order to save some space
// By default we will have C3 .. B6 available (3rd, 4th, 5th and 6th octaves)
#ifndef TONE_MIN_FREQ
  // 130.81 = C3
  #define TONE_MIN_FREQ 130
#endif

#ifndef TONE_MAX_FREQ
  // 2093.0 = C6
  #define TONE_MAX_FREQ 2093
#endif

static inline void _tone(const uint8_t pin, const uint32_t frequency, const uint32_t length)
{
  uint16_t  prescaleDivider = 1; // The divider increases by powers of 2
  uint8_t   prescaleBitMask = 1; // The bitmask only increments unitarily
  uint32_t  ticksPerMs      = F_CPU/1000;
  
  
  // The numbers in each if statement are the lowest frequency that can be achieved
  // with the prescale inside the if statement
  //
  // eg: the frequency can be viewed as a number of transitions equal to the 
  //     frequency * ([LOW]-transition-[HIGH])
  //     (we ignore the extra transition between cycles, happens anyway, err, I think, maybe, not sure)
  //     the longer the time between transitions the lower the frequency
  //     the maximum length we can get is 255 ticks
  //     the minimum length we can get is 1 tick
  //
  //     therefore minimum is CPU_FREQ / Prescale / 255
  //               maximum is CPU_FREQ / Prescale / 1
  
  // We will add/remove 1 hz to account for rounding here
  
  #define TONE_MIN_FREQ_FOR_PRESCALE(PS) (((F_CPU / PS) / 255) + 1)
  #define TONE_MAX_FREQ_FOR_PRESCALE(PS) (((F_CPU / PS) / 1)   - 1)
  
  #define TONE_RANGE_ACTIVE_FOR_PRESCALE(PS) ( TONE_MAX_FREQ >= TONE_MIN_FREQ_FOR_PRESCALE(PS) && TONE_MIN_FREQ <= TONE_MAX_FREQ_FOR_PRESCALE(PS) )
    
  if(TONE_RANGE_ACTIVE_FOR_PRESCALE(1) && frequency >= TONE_MIN_FREQ_FOR_PRESCALE(1))     // (9600000UL / 1) / 255
  {
    prescaleDivider = 1;
    ticksPerMs      = F_CPU/1000;
    prescaleBitMask = 1;
  }
  else if(TONE_RANGE_ACTIVE_FOR_PRESCALE(8) && frequency >= TONE_MIN_FREQ_FOR_PRESCALE(8))  // (9600000UL / 8) / 255
  {
    prescaleDivider = 8;
    ticksPerMs      = (F_CPU/8)/1000;
    prescaleBitMask = 2;
  }      
  else if(TONE_RANGE_ACTIVE_FOR_PRESCALE(64) && frequency >= TONE_MIN_FREQ_FOR_PRESCALE(64))  // (9600000UL / 64) / 255
  {
    prescaleDivider = 64;
    ticksPerMs      = (F_CPU/64)/1000;
    prescaleBitMask = 3;
  }  
  else if(TONE_RANGE_ACTIVE_FOR_PRESCALE(256) && frequency >= TONE_MIN_FREQ_FOR_PRESCALE(256))  // (9600000UL / 256) / 255
  {
    prescaleDivider = 256;
    ticksPerMs      = (F_CPU/256)/1000;
    prescaleBitMask = 4;
  }      
  else if(TONE_RANGE_ACTIVE_FOR_PRESCALE(1024)  && frequency >= TONE_MIN_FREQ_FOR_PRESCALE(1024))  // (9600000UL / 1024) / 255
  {
    prescaleDivider = 1024;
    ticksPerMs      = (F_CPU/1024)/1000;
    prescaleBitMask = 5;
  }
  
  _toneRaw(pin, ((F_CPU / prescaleDivider) / frequency), length ? length * ticksPerMs : ~(0UL), prescaleBitMask);
}


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