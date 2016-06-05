/*
  Blink LED without using delay()
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]
  
  Turns on and off a light emitting diode (LED) connected to a digital
  pin, without using the delay() function.  This means that other code
  can run at the same time without being interrupted by the LED code.

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

const uint8_t ledPin    =  2;  
const uint16_t interval = REAL_MILLIS(1000);  // See Example 01.Basics > RealMillis

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

