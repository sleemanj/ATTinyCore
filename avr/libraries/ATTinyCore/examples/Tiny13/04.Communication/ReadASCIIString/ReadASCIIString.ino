/*
  Read string from Serial, Print string to Serial
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]
  
  Asks your name, says hello.  Not very exciting, but anyway.

  There is an important note here with regard to reading strings over 
  Serial on the Tiny13.  Because of a lack of hardware serial support
  interrupts are disabled for the ENTIRE READ of the string.  That means that
  millis() will "lose time" for the period you are reading the string.

  Just bear that in mind.
       
  Recommended Settings For This Sketch
  ------------------------------------------------------------------------------
  (* indicates non default)
  
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
}

void loop() 
{    
  Serial.println(F("What is your name traveller?"));  
  char buf[10];
  do
  {    
    Serial.read_str(buf, sizeof(buf));   
  } while(!buf[0]);
  
  Serial.print(F("Nice to meet you "));
  Serial.println(buf);
}
