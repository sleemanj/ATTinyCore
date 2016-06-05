/*
  Count button presses, output count to Serial.
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]
  
  Each time you release the button the count will increase and print to 
  the Seral monitor.
    
  [ BUTTON_PIN ] -> [ Button ]     -> [Gnd]

     
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

const uint8_t    buttonPin  = 2;    // the pin that the pushbutton is attached to
uint16_t  buttonPushCounter = 0;   // counter for the number of button presses
uint8_t  buttonState        = HIGH; // current button state (initial = high because pullup)

void setup() 
{  
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(57600); // NOTICE the baud rate specified is ignored on the T13
                          //  Instead it is hard coded as follows...
                          //  Processors at 9.6MHz ==> 57600
                          //  Processors at 4.8 and 1.2MHz ==> 9600
}


void loop() 
{
  if(digitalRead(buttonPin) != buttonState)
  {
    delay(50);
    
    if(digitalRead(buttonPin) != buttonState)
    {
      buttonState = !buttonState;

      if(buttonState)
      {
        // the new state is HIGH, so the old state was LOW
        //   rising edge, button is released               
        Serial.println(++buttonPushCounter);        
      }
      else
      {
        // the new state is LOW, so the old state was HIGH
        //   falling edge, button is pressed

        // No Operation
      }
      
    }
  }  
}

