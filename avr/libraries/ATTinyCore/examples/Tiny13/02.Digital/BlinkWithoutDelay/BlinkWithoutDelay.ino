/*
  For ATTiny13 Arduino Pinout: https://goo.gl/ijL0of  

 Turns on and off a light emitting diode (LED) connected to a digital
 pin, without using the delay() function.  This means that other code
 can run at the same time without being interrupted by the LED code.

  [ LED_PIN ] -> [Resistor 240R] -> [LED] -> [Ground]
  
 */

const uint8_t ledPin   =  2;  
const uint16_t interval = 1000;  

uint8_t       ledState = LOW;
uint32_t      previousMillis = 0;

void setup() 
{
  pinMode(ledPin, OUTPUT);
}

void loop() 
{ 
  uint32_t currentMillis = millis();

  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;

    if (ledState == LOW) 
    {
      ledState = HIGH;
    } 
    else 
    {
      ledState = LOW;
    }
    
    digitalWrite(ledPin, ledState);
  }
}

