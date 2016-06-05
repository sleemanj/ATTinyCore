/*
  Read Analog, Display as PWM of LED
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]
  
  Reads an analog input pin, maps the result to a range from 0 to 255
  and uses the result to set the pulsewidth modulation (PWM) of an output pin.
  Also prints the results to the serial monitor.

  The circuit:
  * potentiometer connected to A2.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  * LED connected from digital pin 1 to ground via resistor

  IMPORTANT!  See the non-default settings below which are required.
     
  Recommended Settings For This Sketch
  ------------------------------------------------------------------------------
  (* indicates non default)
  
  Tools > Board                 : ATTiny13
  Tools > Processor Version     : ATTiny13
  Tools > Use Bootloader        : No (ISP Programmer Upload)
  Tools > Processor Speed       : 9.6MHz Internal Oscillator
  Tools > Millis, Tone Support  : Millis Available, No Tone
  Tools > Millis Accuracy       : 1.666%
* Tools > Print Support         : Dec Only Supported
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

const uint8_t INPUT_PIN = A2;  // Analog input pin that the potentiometer is attached to
const uint8_t analogOutPin = 1; // Analog output pin that the LED is attached to

uint16_t sensorValue = 0;        // value read from the pot
uint8_t  outputValue = 0;        // value output to the PWM (analog out)

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
  
  pinMode(analogOutPin, OUTPUT);
}


void loop() {
  sensorValue = analogRead(INPUT_PIN);
  
  // map it to the range of the analog out:
  //  notice here that we don't use the Arduino map() function
  //  because we don't have space for it, instead we just 
  //  divide the output by 4
  //  this works because 1023 / 4 is 255.75 which is 
  //  close enough, and 4 being a binary divisor means
  //  the division will be optimised by the compiler to a bitshift
  outputValue = sensorValue/4;;
  
  // change the analog out value:
  analogWrite(analogOutPin, outputValue);

  // print the results to the serial monitor:
  Serial.print(sensorValue);
  Serial.print(',');
  Serial.println(outputValue);

  delay(50);
}
