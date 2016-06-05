/*
  Analog Read, Print To Serial
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]
  
  Reads an analog input on pin A2, convert to voltage,  prints the result
  to the serial monitor.
  
  Graphical representation is available using serial plotter 
  (Tools > Serial Plotter menu)
  
  Attach the center pin of a potentiometer to pin A2, and the outside pins to 
  +5V and ground.
     
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

const uint8_t INPUT_PIN = A2;

void setup() 
{
  Serial.begin(57600);         // NOTICE the baud rate specified is ignored on the T13
                               //  Instead it is hard coded as follows...
                               //  Processors at 9.6MHz ==> 57600
                               //  Processors at 4.8 and 1.2MHz ==> 9600
  
  pinMode(INPUT_PIN, INPUT);   // Not strictly necessary because it will be an INPUT already at boot
                               // but for clarity we will include it anyway.  If you are short on space
                               // think about what the chip state is initially to see if you can
                               // omit things like this.
}

void loop() 
{
  uint16_t sensorValue = analogRead(INPUT_PIN);

  // On a tiny13, you really don't have any hope of using floats  
  //  so we will report our voltage in millivolts such that we only
  //  need to do integer maths.
  sensorValue    = sensorValue * (5000 / 1023);
  
  Serial.println(sensorValue);  
  delay(100);
}
