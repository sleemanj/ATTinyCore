/*
  Generate random numbers and print to Serial
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]
  
  The tiny13 is too small to use the stdlb random number generator
  so instead is provided a simplistic 16bit XOR random number generator.

  The random numbers are not particularly great in a statistical sense, 
  but it's enough to introduce some randomness in your life.

  The tiny_random() function will return numbers from 0 to 65534 inclusive

  The implementation is from here:
  http://www.arklyffe.com/main/2010/08/29/xorshift-pseudorandom-number-generator/
  (modified to subtract 1 from the result to get it rooted to zero).

  It's implemented in the WMath.cpp in this core.
     
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

void setup() 
{
  Serial.begin(57600); // NOTICE the baud rate specified is ignored on the T13
                       //  Instead it is hard coded as follows...
                       //  Processors at 9.6MHz ==> 57600
                       //  Processors at 4.8 and 1.2MHz ==> 9600

  // Seed the generator
  tiny_srandom(analogRead(3));  
}

void loop() 
{
  Serial.println(tiny_random());
  delay(10);
}
