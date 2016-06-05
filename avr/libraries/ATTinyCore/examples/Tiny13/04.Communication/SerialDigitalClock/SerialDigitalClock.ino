/*
  Repeatedly print the time to the Serial Monitor
  ------------------------------------------------------------------------------
  
  [ See pinout: https://goo.gl/ijL0of ]

  Note!  Using the internal RC oscillator as a clock isn't going to give you
  good results, simply the internal oscillator is not very accurate, nor stable.

  Using the techniques below, you could expect to gain or lose 5 to 10
  minutes a day **if you have calibrated your oscillator (OSCCAL)**.  If 
  not then your error will potentially be quite a lot more.

  Note that even though we are being a "clock" we don't need totally accurate 
  millis because we are using REAL_MILLIS() - as long as millis() ticks along
  at a constant rate, REAL_MILLIS() provides an adjustment to be more accurate.

  However there is still interaction between REAL_MILLIS() and millis' accuracy.  
  You still want to get as much accuracy into millis() as you can afford.
    
  Recommended Settings For This Sketch
  ------------------------------------------------------------------------------
  (* indicates non default)
  
  Tools > Board                 : ATTiny13
  Tools > Processor Version     : ATTiny13
  Tools > Use Bootloader        : No (ISP Programmer Upload)
  Tools > Processor Speed       : 9.6MHz Internal Oscillator
  Tools > Millis, Tone Support  : Millis Available, No Tone
* Tools > Millis Accuracy       : 10%
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


uint8_t  hours = 0, mins = 0, secs = 0;    
uint32_t lastMillis = 0;

void setup() 
{
  // If you have a calibration number (OSCCAL) for your chip enter it here
  // and uncomment.  The OSCCAL is specific to the chip you are looking at
  // right now, not the type of chip, the actual physical chip you have in 
  // your hand, at the specific temperature in your vicinity right now, 
  // whatever that is.  If you have not calibrated it, then the chip will
  // have a default calibration value built into it at the Atmel factor.
  // OSCCAL=84;
  
  Serial.begin(57600); // NOTICE the baud rate specified is ignored on the T13
                       //  Instead it is hard coded as follows...
                       //  Processors at 9.6MHz ==> 57600
                       //  Processors at 4.8 and 1.2MHz ==> 9600          
}



void loop() 
{  
  if(millis()-lastMillis >= REAL_MILLIS(1000))
  {
    // 1000ms has passed, because we used REAL_MILLIS we know that 
    // regardless our millis() accuracy that reasonably close to 1000ms
    // has passed int he real world.      
    lastMillis += REAL_MILLIS(1000);
    
    secs++;
    if(secs == 60)
    {
      secs = 0;
      mins++;
    }
    
    if(mins == 60)
    {
      mins = 0;
      hours++;
    }
    
    if(hours == 24)
    {
      hours = 0;
    }
  
    Serial.print( hours );
    Serial.print(':');
    Serial.print( mins );
    Serial.print(':');
    Serial.println(secs);        
  }
    
}  
