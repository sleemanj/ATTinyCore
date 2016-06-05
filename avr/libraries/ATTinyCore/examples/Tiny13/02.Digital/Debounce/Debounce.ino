/*
  Press button, toggle LED.
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]
  
  Toggles a light emitting diode(LED) connected to digital
  pin 1, each time you press a pushbutton attached to pin 2 while using the
  internal pullup to save an external resistor and implementing de-bouncing
  to combat switch bounce.

  LED changes state when you release the button (rising edge).

  NB: Since the T13 is very constrained in flash/ram this is probably
      impractical, and you might just slap a capacitor on your button pin
      to ground.
      
      Alternatively, see StateChangeDetection example for simple delay()
      based debounce then you can choose the Smallest Core Size without
      millis()

  [ LED_PIN ]    -> [Resistor 240R]   -> [LED] -> [Ground]
  [ BUTTON_PIN ] -> [ Button ]     -> [Gnd]
     
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

const uint8_t buttonPin = 2;        // the number of the pushbutton pin
const uint8_t ledPin = 1;           // the number of the LED pin
const uint8_t debounceDelay = 100;  // milliseconds the button must be in a stable state   

uint8_t ledState = LOW;             // the current state of the output pin
uint8_t buttonState     = HIGH;     // the current reading from the input pin,  initial value = default (pullup, so high)
uint8_t lastButtonState = HIGH;     // the previous reading from the input pin, initial value = default (pullup, so high)

uint32_t lastDebounceTime = 0;      // the last time the output pin was toggled
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

