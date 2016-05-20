 /*
  For ATTiny13 Arduino Pinout: https://goo.gl/ijL0of  

  Turns on and off a light emitting diode(LED) connected to digital
  pin 2, when pressing a pushbutton attached to pin 3 while using the
  internal pullup to save an external resistor.

  [ LED_PIN ]    -> [Resistor 240R]   -> [LED] -> [Ground]
  [ BUTTON_PIN ] -> [ Button ]     -> [Gnd]

 */
 

const uint8_t buttonPin = 3;     // the number of the pushbutton pin
const uint8_t ledPin =  2;       // the number of the LED pin

void setup() 
{  
  pinMode(ledPin, OUTPUT);   
  pinMode(buttonPin, INPUT_PULLUP);
}
void loop() 
{  
  // Note inverted logic so the LED is on when you press button
  if(digitalRead(buttonPin) == LOW) 
  {
    digitalWrite(ledPin, HIGH);
  }
  else 
  {   
    digitalWrite(ledPin, LOW);
  }
}
