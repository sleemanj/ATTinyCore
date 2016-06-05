/*
  Randomly fade 3 leds.
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]
  
  The ATTiny only has 2 PWM channels, so what to do when you want to
  fade an RGB led with 3 channels?  A simple "soft" pwm can get you by.
  
  Handily a library has been included to accomplish this for you.

  Note that your Processor Speed must be at least 2MHz for this to work
  and that millis() will be disabled (because the prescaler will make
  the software PWM too slow).

  This example sets each of 3 leds (or 3 channels of an RGB led, whatever you want)
  to a random PWM brightness, fading between brightnesses.
 
  [ 0 ] -> [Resistor 240R] -> [LED] -> [Ground]
  [ 1 ] -> [Resistor 240R] -> [LED] -> [Ground]
  [ 2 ] -> [Resistor 240R] -> [LED] -> [Ground]
     
  Recommended Settings For This Sketch
  ------------------------------------------------------------------------------
   
  Tools > Board                 : ATTiny13
  Tools > Processor Version     : ATTiny13
  Tools > Use Bootloader        : No (ISP Programmer Upload)
  Tools > Processor Speed       : 9.6MHz Internal Oscillator
  Tools > Millis, Tone Support  : Millis Available, No Tone
  Tools > Millis Accuracy       : 1.666%
  Tools > Print Support         : Bin, Hex, Dec Supported
  Tools > Serial Support        : Half Duplex, Read+Write
     
  Serial Reminder
  ------------------------------------------------------------------------------
  The Baud Rate is IGNORED on the Tiny13 due to using a simplified serial.
  
  The actual Baud Rate used is dependant on the Processor Speed.
  
  9.6MHz will be 57600 Baud
  4.8MHz will be 9600 Baud
  1.2MHz will be 9600 Baud
  
  If you get garbage output:
  
    1. Check baud rate as above
    2. Check if you have anything else connected to TX/RX like an LED
    3. Check OSCCAL (see Examples > 05.Tools > OSCCAL_Helper    
  
  Pinout
  ------------------------------------------------------------------------------
  For ATTiny13 Arduino Pinout: https://goo.gl/ijL0of  
  
  Important: 
    pinMode() must only be used with the "digital pin numbers" 0 .. n
    pins default to INPUT, you do not need to pinMode() to INPUT if you are only
    ever doing an analogRead() from the pin. 
    
    analogRead() must only be used with the "analog pin numbers" A0 .. n
        
  
  Space Saving Tips
  ------------------------------------------------------------------------------
  
  You don't have much flash or ram to work with.  Remember to think about 
  datatype sizes!  Use the options under the Tools menu to reduce capabilities
  hopefully in exchange for more code size (especially Millis and Print).

  Running short on memory, try this tool to help you track down optimisable areas:
    http://sparks.gogo.co.nz/avr-ram-use.html  
  
  Good Luck With Your Itsy Bitsy Teeny Weeny AVR Arduineee
  
*/

/*


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

