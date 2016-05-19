/*
  For ATTiny13 Arduino Pinout: https://goo.gl/ijL0of  

  You don't have much flash or ram to work with.  Remember to think about data type sizes!

  Running short on memory, try this tool to help you track down optimisable areas:
    http://sparks.gogo.co.nz/avr-ram-use.html
  
  
  Serial.begin([baud]); 
  ---------------------
  The Baud Rate is IGNORED on the Tiny13 due to using a simplified Half Duplex serial system.  
  The actual Baud Rate used is dependant on the Processor Speed
  9.6MHz will be 57600 Baud
  4.8MHz will be 9600 Baud
  1.2MHz will be 9600 Baud
   
  Good Luck With Your Itsy Bitsy Teeny Weeny AVR Arduineee
 */
 
void setup() 
{

}

void loop() 
{

}
