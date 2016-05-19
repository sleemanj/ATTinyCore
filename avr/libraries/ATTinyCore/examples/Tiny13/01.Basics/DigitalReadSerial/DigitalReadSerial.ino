/*
  For ATTiny13 Arduino Pinout: https://goo.gl/ijL0of  
  
  Reads a digital input on pin 2, prints the result to the serial monitor
  
  Serial.begin([baud]); 
  ---------------------
  The Baud Rate is IGNORED on the Tiny13 due to using a simplified Half Duplex serial system.  
  The actual Baud Rate used is dependant on the Processor Speed
  9.6MHz will be 57600 Baud
  4.8MHz will be 9600 Baud
  1.2MHz will be 9600 Baud
   
  Good Luck With Your Itsy Bitsy Teeny Weeny AVR Arduineee
 */
 
const uint8_t BUTTON_PIN = 2;


void setup() 
{
  Serial.begin(57600); // NOTICE the baud rate specified is ignored on the T13
                       //  Instead it is hard coded as follows...
                       //  Processors at 9.6MHz ==> 57600
                       //  Processors at 4.8 and 1.2MHz ==> 9600
                       
  pinMode(BUTTON_PIN, INPUT);
}

void loop() 
{  
  uint8_t buttonState = digitalRead(BUTTON_PIN);
   
  Serial.println(buttonState);
  delay(1);      
}



