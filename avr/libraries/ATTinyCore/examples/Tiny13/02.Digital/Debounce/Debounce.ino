 /*
  For ATTiny13 Arduino Pinout: https://goo.gl/ijL0of  

  Toggles a light emitting diode(LED) connected to digital
  pin 2, each time you press a pushbutton attached to pin 3 while using the
  internal pullup to save an external resistor and implementing de-bouncing
  to combat switch bounce.

  LED changes state when you release the button (rising edge).

  NB Since the T13 is very constrained in flash/ram this is probably
     overkill, and you might just slap a capacitor on your button pin
     to ground.
     
     Alternatively, see StateChangeDetection example for simple delay()
     based debounce then you can choose the Smallest Core Size without
     millis()

  [ LED_PIN ]    -> [Resistor 240R]   -> [LED] -> [Ground]
  [ BUTTON_PIN ] -> [ Button ]     -> [Gnd]

 */


const uint8_t buttonPin = 3;       // the number of the pushbutton pin
const uint8_t ledPin = 2;         // the number of the LED pin
const uint8_t debounceDelay = 50;  // milliseconds the button must be in a stable state   

uint8_t ledState = LOW;             // the current state of the output pin
uint8_t buttonState     = HIGH;     // the current reading from the input pin,  initial value = default (pullup, so high)
uint8_t lastButtonState = HIGH;     // the previous reading from the input pin, initial value = default (pullup, so high)

uint32_t lastDebounceTime = 0;     // the last time the output pin was toggled
uint8_t count = 0;

void setup() 
{
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, ledState);  
}

void loop() 
{
  if(digitalRead(buttonPin) != lastButtonState)
  {
    lastButtonState = !lastButtonState; // Switched states, HIGH becomes LOW, or LOW becomes HIGH
    lastDebounceTime = millis();
  }
  else if((millis() - lastDebounceTime) > debounceDelay && buttonState != lastButtonState)
  {
    buttonState = lastButtonState;        
        
    // On the rising edge, that is when you release the button, toggle the LED
    if(buttonState == HIGH)
    {
      ledState    = !ledState;   
    }
  }

   digitalWrite(ledPin, ledState);
}

