/*
  Read Digital Pin, Output To Serial
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]
  
  Reads a digital input on pin 2, prints the result to the serial monitor

  [ BUTTON_PIN ] +-> [ BUTTON ] -> [ Vcc ]
                 |
                 \-> [10k Resistor] -> [ Gnd ]
     
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

const uint8_t BUTTON_PIN = 2;


void setup() 
{                       
  Serial.begin(57600);         // NOTICE the baud rate specified is ignored on the T13
                               //  Instead it is hard coded as follows...
                               //  Processors at 9.6MHz ==> 57600
                               //  Processors at 4.8 and 1.2MHz ==> 9600
  
  pinMode(BUTTON_PIN, INPUT);  // Not strictly necessary because it will be an INPUT already at boot
                               // but for clarity we will include it anyway.  If you are short on space
                               // think about what the chip state is initially to see if you can
                               // omit things like this.
}

void loop() 
{  
  uint8_t buttonState = digitalRead(BUTTON_PIN);
   
  Serial.println(buttonState);
  delay(100);      
}



