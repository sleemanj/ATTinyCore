/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  [ LED_PIN ] -> [Resistor 240R] -> [LED] -> [Ground]

  For ATTiny13 Arduino Pinout: https://goo.gl/ijL0of  
 */

const uint8_t LED_PIN = 2;

void setup() 
{
  pinMode(LED_PIN, OUTPUT);
}


void loop() 
{
  digitalWrite(LED_PIN, HIGH);
  delay(1000);              
  digitalWrite(LED_PIN, LOW);
  delay(1000);              
}
