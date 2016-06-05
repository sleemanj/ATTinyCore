/*
  Blink an LED using two diffent timing methods.
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]
  
  Turns on and off a light emitting diode (LED) connected to a digital
  pin, without using the delay() function, with and without 
  the use of REAL_MILLIS so you can see the difference.

  First it will blink 5 times using the normal interval of 1000ms
  Then it will blink 5 times using the REAL_MILLIS interval of 1000ms

  If you adjust the Tools > Millis Accuracy to a very low accuracy (50%)
  upload the code and observe you will see that the first 5 blinks are 
  much longer than 1 second (1000ms), but you will see that the second
  5 blinks, the REAL_MILLIS(1000) ones, are very close to 1 second.

  This shows you that you can set a very low accuracy millis to save 
  flash space, but using REAL_MILLIS() get a very high accuracy 
  waiting period.
  
  [ LED_PIN ] -> [Resistor 240R] -> [LED] -> [Ground]
     
  Recommended Settings For This Sketch
  ------------------------------------------------------------------------------
  (* indicates non default)
  
  Tools > Board                 : ATTiny13
  Tools > Processor Version     : ATTiny13
  Tools > Use Bootloader        : No (ISP Programmer Upload)
  Tools > Processor Speed       : 9.6MHz Internal Oscillator
  Tools > Millis, Tone Support  : Millis Available, No Tone
* Tools > Millis Accuracy       : 50%
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

const uint8_t ledPin    =  2;  

void setup() 
{
  pinMode(ledPin, OUTPUT);
}


void loop() 
{ 
  uint32_t lastMillis;

  // Note here that this implementation is no better than using delay()
  // I'm just doing it this way for sake of example, see BlinkWithoutDelay
  // for the proper way :-)
    
  // For the standard interval
  for(uint8_t i = 0; i < 5; i++)
  {
    digitalWrite(ledPin, HIGH);
    lastMillis = millis();
    while(millis() - lastMillis < 1000); // Wait interval
    
    digitalWrite(ledPin, LOW);    
    lastMillis = millis();
    while(millis() - lastMillis < 1000); // Wait interval
  }
  
  // And now for REAL_MILLIS
  for(uint8_t i = 0; i < 5; i++)
  {
    digitalWrite(ledPin, HIGH);
    lastMillis = millis();
    while(millis() - lastMillis < REAL_MILLIS(1000)); // Wait a real world interval
    
    digitalWrite(ledPin, LOW);
    lastMillis = millis();
    while(millis() - lastMillis < REAL_MILLIS(1000)); // Wait a real world interval
  }
  
}

