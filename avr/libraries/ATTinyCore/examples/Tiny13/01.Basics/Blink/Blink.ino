/*
  Blink
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]
  
  Turns on an LED on for one second, then off for one second, repeatedly.

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
