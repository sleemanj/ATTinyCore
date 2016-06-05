/*
  Fade
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]
  
  Fades an LED back and forth (throb).

  [ LED_PIN ] -> [Resistor 240R] -> [LED] -> [Ground]
     
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
  
  Pinout
  ------------------------------------------------------------------------------
  For ATTiny13 Arduino Pinout: https://goo.gl/ijL0of  

  You don't have much flash or ram to work with.  Remember to think about data type sizes!

  Running short on memory, try this tool to help you track down optimisable areas:
    http://sparks.gogo.co.nz/avr-ram-use.html
  
  Good Luck With Your Itsy Bitsy Teeny Weeny AVR Arduineee
  
*/

const uint8_t led  = 1;    // the PWM pin the LED is attached to
uint8_t brightness = 10;   // how bright the LED is
int8_t  fadeAmount = 5;    // how many points to fade the LED by

void setup() 
{  
  pinMode(led, OUTPUT);
}

void loop() 
{
  // set the brightness of pin 9:
  analogWrite(led, brightness);

  // change the brightness for next time through the loop:
  brightness += fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 10)
  {
    brightness = 10;
    fadeAmount = -fadeAmount ;
  }
  else if(brightness >= 250) 
  {
    brightness = 250;
    fadeAmount = -fadeAmount ;
  }
  
  // wait for 30 milliseconds to see the dimming effect
  delay(30);
}
