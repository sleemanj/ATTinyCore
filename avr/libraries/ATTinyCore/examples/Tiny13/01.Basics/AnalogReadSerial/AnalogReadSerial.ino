/*
  For ATTiny13 Arduino Pinout: https://goo.gl/ijL0of  
  
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.
  
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
  Serial.begin(57600); // NOTICE the baud rate specified is ignored on the T13
                       //  Instead it is hard coded as follows...
                       //  Processors at 9.6MHz ==> 57600
                       //  Processors at 4.8 and 1.2MHz ==> 9600
}

void loop() 
{  
  uint16_t sensorValue = analogRead(A0); 
  Serial.println(sensorValue);
  delay(1);        
}
