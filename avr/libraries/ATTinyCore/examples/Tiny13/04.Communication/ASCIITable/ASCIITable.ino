/*
  Print a table of ASCII characters to the Serial Monitor
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]
  
  Prints out byte values in all possible formats:
  * as raw binary values
  * as ASCII-encoded decimal, hex, octal, and binary values
  
  For more on ASCII, see http://www.asciitable.com and http://en.wikipedia.org/wiki/ASCII
     
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
  Serial.begin(57600);    // NOTICE the baud rate specified is ignored on the T13
                          //  Instead it is hard coded as follows...
                          //  Processors at 9.6MHz ==> 57600
                          //  Processors at 4.8 and 1.2MHz ==> 9600
  
  // prints title with ending line break
  Serial.println(F("ASCII Table ~ Character Map"));
}

// first visible ASCIIcharacter '!' is number 33:
uint8_t thisByte = 33;
// you can also write ASCII characters in single quotes.
// for example. '!' is the same as 33, so you could also use this:
//int thisByte = '!';

void loop() 
{
  // prints value unaltered, i.e. the raw binary version of the
  // byte. The serial monitor interprets all bytes as
  // ASCII, so 33, the first number,  will show up as '!'
  Serial.write(thisByte);

  Serial.print(F(", dec: "));
  // prints value as string as an ASCII-encoded decimal (base 10).
  // Decimal is the  default format for Serial.print() and Serial.println(),
  // so no modifier is needed:
  Serial.print(thisByte);
  // But you can declare the modifier for decimal if you want to.
  //this also works if you uncomment it:

  // Serial.print(thisByte, DEC);


  Serial.print(F(", hex: "));
  // prints value as string in hexadecimal (base 16):
  Serial.print(thisByte, HEX);
  
  Serial.print(F(", bin: "));
  // prints value as string in binary (base 2)
  // also prints ending line break:
  Serial.println(thisByte, BIN);

  // if printed last visible character '~' or 126, stop:
  if (thisByte == 126) {    // you could also use if (thisByte == '~') {
    // This loop loops forever and does nothing
    while (true) {
      continue;
    }
  }
  // go on to the next character
  thisByte++;
}
