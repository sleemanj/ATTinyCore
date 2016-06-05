/*
  Press button, light LED.
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]
  
  Turns on and off a light emitting diode(LED) connected to digital
  pin 2, when pressing a pushbutton attached to pin 3.

  [ LED_PIN ]    -> [Resistor 240R]   -> [LED] -> [Ground]
  [ BUTTON_PIN ] -> [ Button ]     -> [Vcc]
  [ BUTTON_PIN ] -> [Resistor 10k] -> [Ground]
     
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
