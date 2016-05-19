 /*
  For ATTiny13 Arduino Pinout: https://goo.gl/ijL0of  

  Turns on and off a light emitting diode(LED) connected to digital
  pin 2, when pressing a pushbutton attached to pin 3.

  [ LED_PIN ]    -> [Resistor 240R]   -> [LED] -> [Ground]
  [ BUTTON_PIN ] -> [ Button ]     -> [Vcc]
  [ BUTTON_PIN ] -> [Resistor 10k] -> [Ground]

 */
 

const uint8_t buttonPin = 3;     // the number of the pushbutton pin
const uint8_t ledPin =  2;       // the number of the LED pin

void setup() 
{  
  pinMode(ledPin, OUTPUT);   
  pinMode(buttonPin, INPUT);
}

void loop() 
{  
  if(digitalRead(buttonPin) == HIGH) 
  {
    digitalWrite(ledPin, HIGH);
  }
  else 
  {   
    digitalWrite(ledPin, LOW);
  }
}
