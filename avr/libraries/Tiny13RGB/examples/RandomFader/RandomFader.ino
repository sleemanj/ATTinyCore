/*
  The ATTiny only has 2 PWM channels, so what to do when you want to
  fade an RGB led with 3 channels?  A simple "soft" pwm can get you by.

  This example sets each of 3 leds (or 3 channels of an RGB led, whatever you want)
  to a random PWM brightness, fading between brightnesses.
 
  [ 0 ] -> [Resistor 240R] -> [LED] -> [Ground]
  [ 1 ] -> [Resistor 240R] -> [LED] -> [Ground]
  [ 2 ] -> [Resistor 240R] -> [LED] -> [Ground]

  For ATTiny13 Arduino Pinout: https://goo.gl/ijL0of  
 */
#include "Tiny13RGB.h"

Tiny13RGB rgb;

// The Tiny13RGB library only supports pins 0, 1 and 2
// so put your colours on those 3 pins...
const uint8_t RED     = 0;
const uint8_t GREEN   = 1;
const uint8_t BLUE    = 2;

const uint8_t SPEED   = 2;
   
void setup() 
{                
  rgb.begin();
  
  // Let's throb each LED once at the start so you can see they work independantly
  rgb.analogThrob(RED,   SPEED*2);
  rgb.analogThrob(GREEN, SPEED*2);
  rgb.analogThrob(BLUE,  SPEED*2);   
}

void loop()
{
  // The target is the pwm level we are heading towards
  //  we use analogWriteTowards to step the channel (R/G/B) towards
  //  their respecitive target, when analogWriteTowards returns 0
  //  then we are at the target and pick a new target at random.
  static byte rTarget = 0, gTarget = 0, bTarget = 0; 
  
  if(rgb.analogWriteTowards(RED,   rTarget) == 0) rTarget = tiny_random();
  if(rgb.analogWriteTowards(GREEN, gTarget) == 0) gTarget = tiny_random();
  if(rgb.analogWriteTowards(BLUE,  bTarget) == 0) bTarget = tiny_random();
    
  delay(SPEED);  
}
