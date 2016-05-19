/*
   Fade
 
  [ LED_PIN ] -> [Resistor 240R] -> [LED] -> [Ground]

  For ATTiny13 Arduino Pinout: https://goo.gl/ijL0of  
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
