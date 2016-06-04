/*
  Simple Software Based PWM (analogWrite) for 3 channels (RGB).

  The ATTiny13 especially only has 2 PWM channels, so we can't 
  do a typical mixing of RGB leds on it without using a software
  solution.  This is that.

  Usage
  ------------------------------------------------------------------------------
  Tiny13RGB rgb;
  [...]
  rgb.begin();
  rgb.analogWrite([pin0..2], [val0..255]);
  rgb.analogThrob([pin0..2], [speed0..255]); // Lower number = higher speed
  rgb.analogWriteTowards([pin0..2], [target0..255]); // Does 1 step
  
  
  Caveats!
  ------------------------------------------------------------------------------
  
  Disables millis() if it is running with a prescaler
  Not suitable if running at less than 2MHz 
*/   
   
#ifndef Tiny13RGB_h
#define Tiny13RGB_h
     
  
  class Tiny13RGB
  {
    public:
      /** Set the pins as output, start the interrupt timer, disable millis if necessary. */
      void begin();
      
      /** As for the standard analogWrite().  
       * 
       * @param channel Pin number 0..2 to manipulate.
       * @param val     The pwm value, 0 = 0ff, 255 = full on.
       */
      
      void analogWrite(uint8_t channel, uint8_t val);    
      
      /** Throb an LED from off to on and back to off again.
       * 
       * @param channel Pin number 0..2 to manipulate.
       * @param spd     The inverse speed, that is, 0 = very fast, 255 = very slow
       *                Typically a value from 4 to 20 is fine.
       */
      
      void analogThrob(uint8_t channel, uint8_t spd);        
      
      /** Step an LED's pwm level up or down by one unit towards the target.
       * 
       *  This method returns 0 when the current PWM level is *already* the target.
       * 
       * @param channel Pin number 0..2 to manipulate.
       * @param target  The pwm value, 0 = off, 255 = full on.
       * @return Returns 0 when the target is reached already (no change made to pwm level).
       *         Returns 1 when the pwm level was changed to approach the target.
       */
      
      uint8_t analogWriteTowards(uint8_t channel, uint8_t target);
  };      
  
  #if F_CPU < 2000000UL
    #error Processor speed is too slow for soft rgb pwm, you need at least 2MHz.
  #endif
      
#endif