/*
  For ATTiny13 Arduino Pinout: https://goo.gl/ijL0of  
  
  Counts the times you press the button.

  ### IMPORTANT ###
  Due to the demands of Serial/Print/Stream, to fit this sketch in the Tiny13 
  you must choose the "Smallest Core (No Millis, No Tone)" Core Size.
  
  [ LED_PIN ]    -> [Resistor 240R]   -> [LED] -> [Ground]
  [ BUTTON_PIN ] -> [ Button ]     -> [Gnd]

 */

const uint8_t    buttonPin  = 3;    // the pin that the pushbutton is attached to
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

