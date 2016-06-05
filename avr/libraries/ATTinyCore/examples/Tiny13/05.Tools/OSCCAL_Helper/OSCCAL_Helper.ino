/*
  OSCCAL Helper
  ------------------------------------------------------------------------------
  [ Pinout:  https://goo.gl/ijL0of ]
  
  This tool allows you to calibrate the Internal RC Oscillator of your ATTiny13

  Connect a potentiometer to A2 (Vcc one side, Gnd the other, and the Wiper to 
  A2).  The value of the potentiometer doesn't matter, whatever you have handy.
  
  Set the pot to about half way through it's range.

  Upload sketch and open the serial monitor.

  The sketch will repeatedly print the following:

    D1: xxxxHz, H/L=yyyus
    OSCCAL: zz

  xxxxHz is the frequency  (full period High+Low) that is in theory being 
         put on Digital Pin 1

  yyyus  is the number of microseconds that the pin is high/low for
         ie, the pin is on for yyy microseconds, and then is off
         for yyy microseconds
         
  zz     is the current OSCCAL value

  If you are getting garbage (and you have set the correct Baud Rate - see below)
  then your OSCCAL is way out, turn the potentiometer until you find somewhere
  that works,
  
  You can turn the potentiometer to adjust the OSCCAL value a maximum of +/-7
  up or down from it's factory value.

  If you have a logic analyzer, oscilloscope, or frequency counter you 
  can use that to measure the signal on Digital Pin 1 and adjust the potentiometer
  until you have it as close as you can (you won't get it exact, +/- 10Hz 
  realistically) to the frequency or time given in the Serial Monitor.  Remember
  the Frequency is the full period frequency covering a complete HIGH and LOW pair,
  while the Time (H/L=yyyus) is the time that the pin is HIGH, or LOW (ie, half the 
  period).  It will be obvious if you have to double or halve the numbers in your
  la/scope/counter.
  
  Now you can read the OSCCAL value printed in the Serial Monitor.  To use this 
  OSCCAL value, simply put:
  
     OSCCAL = [your value];  

   in your setup(), easy as that.
    
  
  Recommended Settings For This Sketch
  ------------------------------------------------------------------------------
  (* non default value)
   
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
  
  If you get garbage output:
  
    1. Check baud rate as above
    2. Check if you have anything else connected to TX/RX like an LED
    3. Check OSCCAL (see Examples > 05.Tools > OSCCAL_Helper    
  
  Pinout
  ------------------------------------------------------------------------------
  For ATTiny13 Arduino Pinout: https://goo.gl/ijL0of  
  
  Important: 
    pinMode() must only be used with the "digital pin numbers" 0 .. n
    pins default to INPUT, you do not need to pinMode() to INPUT if you are only
    ever doing an analogRead() from the pin. 
    
    analogRead() must only be used with the "analog pin numbers" A0 .. n
        
  
  Space Saving Tips
  ------------------------------------------------------------------------------
  
  You don't have much flash or ram to work with.  Remember to think about 
  datatype sizes!  Use the options under the Tools menu to reduce capabilities
  hopefully in exchange for more code size (especially Millis and Print).
  
  Running short on memory, try this tool to help you track down optimisable areas:
    http://sparks.gogo.co.nz/avr-ram-use.html  
  
  Good Luck With Your Itsy Bitsy Teeny Weeny AVR Arduineee
  
*/

#if defined(NO_MILLIS) && NO_MILLIS
  #error Millis must be enabled for this sketch.
#endif

uint8_t defaultOSCCAL = OSCCAL;

void setup() 
{

  delay(1000);
 
  // We are tight on space here so using direct port manipulation   
  DDRB |= 0b00000011;
  
  // Output F_CPU / PRESCALE / 256 on digital pin 1 (physical pin 6)
  TCCR0A = 0b00010010; // Toggle Output on Match 
  OCR0A = 255;         // Match This 
}


void loop() 
{ 
  uint8_t x = defaultOSCCAL - 7 + (analogRead(A2) / 64);
  
  OSCCAL = x;    
  Serial.print(F("\nD1: "));  
  Serial.print((uint32_t) ((F_CPU / MILLIS_TIMER_PRESCALE / 256)/2));  
  Serial.print(F("Hz, H/L="));
  
  Serial.print((uint32_t) (1000000 / (F_CPU / MILLIS_TIMER_PRESCALE / 256)));
  Serial.println(F("us"));
  
  Serial.print(F("OSCCAL: "));
  Serial.println((uint32_t) x); 
  delay(500);
}
