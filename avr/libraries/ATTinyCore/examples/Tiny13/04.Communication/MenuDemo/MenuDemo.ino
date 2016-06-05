/*
  Print menu to Serial, read selection from Serial
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]
  
  Gives you some options, allows you to pick from them by sending a single
  character.  Blocks until you choose.
       
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
  
  Serial.println(F("\n~~ Tiny Menu ~~\n"));  
  Serial.println(F("1. Say Hello"));
  Serial.println(F("2. Say Goodbye"));
  Serial.println(F("3. Sing"));
  Serial.println(F("4. Jack"));
  
  switch(Serial.read_char_blocking())
  {
    case '1':
      Serial.println(F(">> Hello"));
      break;
    case '2':
      Serial.println(F(">> Goodbye"));
      break;
    case '3':
      Serial.println(F(">> Daisy, daisy, give me your answer do."));
      break;
    case '4':
      for(uint8_t x = 0; x < 100; x++)
      {
        Serial.println(F("All work and no play makes Jack a dull boy."));       
      }
      break;
    default:
      Serial.println(F("* Unkown Command *"));
      break;
  }
  delay(1000);
}
