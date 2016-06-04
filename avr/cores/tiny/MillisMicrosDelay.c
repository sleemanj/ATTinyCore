#include "Arduino.h"
#if defined(USE_NEW_MILLIS) && USE_NEW_MILLIS
#include "MillisMicrosDelay.h"

// millis() and micros() are disabled entirely with NO_MILLIS
#ifndef NO_MILLIS
#include <avr/interrupt.h>
/*
volatile MillisMicrosTime_t ovrf=0;

ISR(MILLIS_TIMER_OVERFLOW_VECTOR)
{
  ovrf++; //Increment counter every 256 clock cycles  
}
*/

volatile MillisMicrosTime_t ovrf=0;

/* 
 * The ISR is defined in pins_arduno.c for the variant as it could, well, vary
 * depending on the particular chip, all we care about is that ovrf gets updated
 * every frequency/prescale/256 ticks.  Exactly how each variant chooses to do that
 * is up to it.
 *
 * A typical ISR would look like...

  extern volatile MillisMicrosTime_t ovrf;
  ISR(TIM0_OVF_vect)
  {
    ovrf++; //Increment counter every 256 clock cycles  
  }
  */

MillisMicrosTime_t millis()
{
  MillisMicrosTime_t x;
  uint8_t oldSREG = SREG;
  cli();
      
  // To come up with these calculations I wrote a Google Spreadsheet which includes 
  // an automation script to generated the C code required.  Should you need to
  // add different frequencies in future, feel free to copy the sheet and use it.
  //
  //   https://goo.gl/sgANEr
  //
  // James Sleeman, james@gogo.co.nz, http://sparks.gogo.co.nz/
   
#if (F_CPU / MILLIS_TIMER_PRESCALE) == 16000UL
  // 16 kHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  x = (ovrf * 16) ;


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 32768UL
  // 32.768 kHz
  //     Best Error Possible: 0.0000%  (0 Decimal)
  //    Worst Error Possible: 10.4000% (0.104 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 104000UL
    //  Error: 10.4000% (0.104 Decimal)
    // Jitter: 0.0000% (0 Decimal)
    x = (ovrf * 7) ;
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 40000UL
    //  Error: 4.0000% (0.04 Decimal)
    // Jitter: 0.0000% (0 Decimal)
    x = (ovrf * 7)  + (ovrf / 2);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8000UL
    //  Error: 0.8000% (0.008 Decimal)
    // Jitter: 0.0000% (0 Decimal)
    x = (ovrf * 7)  + (ovrf / 2) + (ovrf / 4);
  #else
    //  Error: 0.0000% (0 Decimal)
    // Jitter: 0.0000% (0 Decimal)
    x = (ovrf * 7)  + (ovrf / 2) + (ovrf / 4) + (ovrf / 16);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 125000UL
  // 125 kHz
  //     Best Error Possible: 0.0054%  (0.000054086100000000004 Decimal)
  //    Worst Error Possible: 2.3438% (0.0234377102 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 23438UL
    //  Error: 2.3438% (0.0234377102 Decimal)
    // Jitter: 0.0053% (0.0000532543 Decimal)
    x = (ovrf * 2) ;
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8189UL
    //  Error: 0.8189% (0.008188850000000001 Decimal)
    // Jitter: 0.0105% (0.00010538 Decimal)
    x = (ovrf * 2)  + (ovrf / 32);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 570UL
    //  Error: 0.0570% (0.0005696364 Decimal)
    // Jitter: 0.0185% (0.0001854254 Decimal)
    x = (ovrf * 2)  + (ovrf / 32) + (ovrf / 64);
  #else
    //  Error: 0.0054% (0.000054086100000000004 Decimal)
    // Jitter: 0.0258% (0.0002581209 Decimal)
    x = (ovrf * 2)  + (ovrf / 32) + (ovrf / 64) + (ovrf / 1024) + (ovrf / 8192) + (ovrf / 65536);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 128000UL
  // 128 kHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  x = (ovrf * 2) ;


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 600000UL
  // 600 kHz
  //     Best Error Possible: 0.0091%  (0.0000913535 Decimal)
  //    Worst Error Possible: 41.4068% (0.4140676903 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 414067UL
    //  Error: 41.4068% (0.4140676903 Decimal)
    // Jitter: 0.0039% (0.0000390023 Decimal)
    x = (ovrf * 0)  + (ovrf / 4);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 121112UL
    //  Error: 12.1112% (0.1211119719 Decimal)
    // Jitter: 0.0103% (0.0001027377 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 8);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 47881UL
    //  Error: 4.7881% (0.0478809068 Decimal)
    // Jitter: 0.0144% (0.00014398660000000001 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 8) + (ovrf / 32);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 11270UL
    //  Error: 1.1270% (0.011270414199999999 Decimal)
    // Jitter: 0.0218% (0.000217923 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 8) + (ovrf / 32) + (ovrf / 64);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 2126UL
    //  Error: 0.2126% (0.0021257019 Decimal)
    // Jitter: 0.0281% (0.0002812586 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 8) + (ovrf / 32) + (ovrf / 64) + (ovrf / 256);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 992UL
    //  Error: 0.0992% (0.0009915898999999998 Decimal)
    // Jitter: 0.0329% (0.0003288179 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 8) + (ovrf / 32) + (ovrf / 64) + (ovrf / 256) + (ovrf / 2048);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 429UL
    //  Error: 0.0429% (0.00042937520000000003 Decimal)
    // Jitter: 0.0374% (0.0003740969 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 8) + (ovrf / 32) + (ovrf / 64) + (ovrf / 256) + (ovrf / 2048) + (ovrf / 4096);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 153UL
    //  Error: 0.0153% (0.00015276230000000002 Decimal)
    // Jitter: 0.0410% (0.0004097833 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 8) + (ovrf / 32) + (ovrf / 64) + (ovrf / 256) + (ovrf / 2048) + (ovrf / 4096) + (ovrf / 8192);
  #else
    //  Error: 0.0091% (0.0000913535 Decimal)
    // Jitter: 0.0480% (0.0004795937 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 8) + (ovrf / 32) + (ovrf / 64) + (ovrf / 256) + (ovrf / 2048) + (ovrf / 4096) + (ovrf / 8192) + (ovrf / 32768);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 800000UL
  // 800 kHz
  //     Best Error Possible: 0.0115%  (0.0001145359 Decimal)
  //    Worst Error Possible: 21.8758% (0.21875796979999998 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 218758UL
    //  Error: 21.8758% (0.21875796979999998 Decimal)
    // Jitter: 0.0068% (0.00006818179999999999 Decimal)
    x = (ovrf * 0)  + (ovrf / 4);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 23455UL
    //  Error: 2.3455% (0.023454928400000002 Decimal)
    // Jitter: 0.0108% (0.0001079545 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 16);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 11258UL
    //  Error: 1.1258% (0.0112578935 Decimal)
    // Jitter: 0.0136% (0.0001362782 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 16) + (ovrf / 256);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 5164UL
    //  Error: 0.5164% (0.0051639588 Decimal)
    // Jitter: 0.0186% (0.00018647909999999998 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 16) + (ovrf / 256) + (ovrf / 512);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 2123UL
    //  Error: 0.2123% (0.0021232221 Decimal)
    // Jitter: 0.0238% (0.0002382033 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 16) + (ovrf / 256) + (ovrf / 512) + (ovrf / 1024);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 608UL
    //  Error: 0.0608% (0.0006080995 Decimal)
    // Jitter: 0.0290% (0.0002903811 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 16) + (ovrf / 256) + (ovrf / 512) + (ovrf / 1024) + (ovrf / 2048);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 237UL
    //  Error: 0.0237% (0.0002371862 Decimal)
    // Jitter: 0.0301% (0.0003012704 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 16) + (ovrf / 256) + (ovrf / 512) + (ovrf / 1024) + (ovrf / 2048) + (ovrf / 8192);
  #else
    //  Error: 0.0115% (0.0001145359 Decimal)
    // Jitter: 0.0387% (0.0003865699 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 16) + (ovrf / 256) + (ovrf / 512) + (ovrf / 1024) + (ovrf / 2048) + (ovrf / 8192) + (ovrf / 32768) + (ovrf / 65536);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 1000000UL
  // 1 MHz
  //     Best Error Possible: 0.0066%  (0.0000658686 Decimal)
  //    Worst Error Possible: 2.3448% (0.0234481704 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 23448UL
    //  Error: 2.3448% (0.0234481704 Decimal)
    // Jitter: 0.0070% (0.0000703129 Decimal)
    x = (ovrf * 0)  + (ovrf / 4);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8200UL
    //  Error: 0.8200% (0.008199986800000001 Decimal)
    // Jitter: 0.0121% (0.0001212963 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 256);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 581UL
    //  Error: 0.0581% (0.0005812023000000001 Decimal)
    // Jitter: 0.0175% (0.0001750009 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 256) + (ovrf / 512);
  #else
    //  Error: 0.0066% (0.0000658686 Decimal)
    // Jitter: 0.0281% (0.0002809513 Decimal)
    x = (ovrf * 0)  + (ovrf / 4) + (ovrf / 256) + (ovrf / 512) + (ovrf / 8192) + (ovrf / 65536);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 1200000UL
  // 1.2 MHz
  //     Best Error Possible: 0.0096%  (0.0000955207 Decimal)
  //    Worst Error Possible: 41.4070% (0.4140701323 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 414069UL
    //  Error: 41.4070% (0.4140701323 Decimal)
    // Jitter: 0.0042% (0.0000424882 Decimal)
    x = (ovrf * 0)  + (ovrf / 8);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 121115UL
    //  Error: 12.1116% (0.1211156348 Decimal)
    // Jitter: 0.0101% (0.00010128149999999999 Decimal)
    x = (ovrf * 0)  + (ovrf / 8) + (ovrf / 16);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 47885UL
    //  Error: 4.7885% (0.047884874900000006 Decimal)
    // Jitter: 0.0141% (0.0001412517 Decimal)
    x = (ovrf * 0)  + (ovrf / 8) + (ovrf / 16) + (ovrf / 64);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 11275UL
    //  Error: 1.1275% (0.0112745348 Decimal)
    // Jitter: 0.0216% (0.00021628579999999998 Decimal)
    x = (ovrf * 0)  + (ovrf / 8) + (ovrf / 16) + (ovrf / 64) + (ovrf / 128);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 2130UL
    //  Error: 0.2130% (0.0021298607 Decimal)
    // Jitter: 0.0278% (0.0002783623 Decimal)
    x = (ovrf * 0)  + (ovrf / 8) + (ovrf / 16) + (ovrf / 64) + (ovrf / 128) + (ovrf / 512);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 995UL
    //  Error: 0.0996% (0.0009957534 Decimal)
    // Jitter: 0.0329% (0.0003288179 Decimal)
    x = (ovrf * 0)  + (ovrf / 8) + (ovrf / 16) + (ovrf / 64) + (ovrf / 128) + (ovrf / 512) + (ovrf / 4096);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 434UL
    //  Error: 0.0434% (0.000433541 Decimal)
    // Jitter: 0.0374% (0.0003740969 Decimal)
    x = (ovrf * 0)  + (ovrf / 8) + (ovrf / 16) + (ovrf / 64) + (ovrf / 128) + (ovrf / 512) + (ovrf / 4096) + (ovrf / 8192);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 157UL
    //  Error: 0.0157% (0.0001569292 Decimal)
    // Jitter: 0.0410% (0.0004097833 Decimal)
    x = (ovrf * 0)  + (ovrf / 8) + (ovrf / 16) + (ovrf / 64) + (ovrf / 128) + (ovrf / 512) + (ovrf / 4096) + (ovrf / 8192) + (ovrf / 16384);
  #else
    //  Error: 0.0096% (0.0000955207 Decimal)
    // Jitter: 0.0480% (0.0004795937 Decimal)
    x = (ovrf * 0)  + (ovrf / 8) + (ovrf / 16) + (ovrf / 64) + (ovrf / 128) + (ovrf / 512) + (ovrf / 4096) + (ovrf / 8192) + (ovrf / 16384) + (ovrf / 65536);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 2000000UL
  // 2 MHz
  //     Best Error Possible: 0.0113%  (0.0001126104 Decimal)
  //    Worst Error Possible: 2.3446% (0.0234464846 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 23445UL
    //  Error: 2.3446% (0.0234464846 Decimal)
    // Jitter: 0.0088% (0.00008823529999999999 Decimal)
    x = (ovrf * 0)  + (ovrf / 8);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8198UL
    //  Error: 0.8198% (0.008198212299999999 Decimal)
    // Jitter: 0.0124% (0.0001235294 Decimal)
    x = (ovrf * 0)  + (ovrf / 8) + (ovrf / 512);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 579UL
    //  Error: 0.0579% (0.0005791708 Decimal)
    // Jitter: 0.0179% (0.00017891220000000002 Decimal)
    x = (ovrf * 0)  + (ovrf / 8) + (ovrf / 512) + (ovrf / 1024);
  #else
    //  Error: 0.0113% (0.0001126104 Decimal)
    // Jitter: 0.0224% (0.0002238139 Decimal)
    x = (ovrf * 0)  + (ovrf / 8) + (ovrf / 512) + (ovrf / 1024) + (ovrf / 16384);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 2400000UL
  // 2.4 MHz
  //     Best Error Possible: 0.0153%  (0.00015272219999999998 Decimal)
  //    Worst Error Possible: 41.4072% (0.4140718057 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 414072UL
    //  Error: 41.4072% (0.4140718057 Decimal)
    // Jitter: 0.0055% (0.0000551471 Decimal)
    x = (ovrf * 0)  + (ovrf / 16);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 121113UL
    //  Error: 12.1113% (0.1211131569 Decimal)
    // Jitter: 0.0106% (0.00010625 Decimal)
    x = (ovrf * 0)  + (ovrf / 16) + (ovrf / 32);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 47881UL
    //  Error: 4.7881% (0.0478814326 Decimal)
    // Jitter: 0.0148% (0.0001484375 Decimal)
    x = (ovrf * 0)  + (ovrf / 16) + (ovrf / 32) + (ovrf / 128);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 11271UL
    //  Error: 1.1271% (0.011270545 Decimal)
    // Jitter: 0.0223% (0.0002229665 Decimal)
    x = (ovrf * 0)  + (ovrf / 16) + (ovrf / 32) + (ovrf / 128) + (ovrf / 256);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 2126UL
    //  Error: 0.2126% (0.0021257332 Decimal)
    // Jitter: 0.0283% (0.0002829912 Decimal)
    x = (ovrf * 0)  + (ovrf / 16) + (ovrf / 32) + (ovrf / 128) + (ovrf / 256) + (ovrf / 1024);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 992UL
    //  Error: 0.0992% (0.0009916172 Decimal)
    // Jitter: 0.0329% (0.0003291789 Decimal)
    x = (ovrf * 0)  + (ovrf / 16) + (ovrf / 32) + (ovrf / 128) + (ovrf / 256) + (ovrf / 1024) + (ovrf / 8192);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 429UL
    //  Error: 0.0429% (0.0004293574 Decimal)
    // Jitter: 0.0377% (0.0003769841 Decimal)
    x = (ovrf * 0)  + (ovrf / 16) + (ovrf / 32) + (ovrf / 128) + (ovrf / 256) + (ovrf / 1024) + (ovrf / 8192) + (ovrf / 16384);
  #else
    //  Error: 0.0153% (0.00015272219999999998 Decimal)
    // Jitter: 0.0413% (0.0004126984 Decimal)
    x = (ovrf * 0)  + (ovrf / 16) + (ovrf / 32) + (ovrf / 128) + (ovrf / 256) + (ovrf / 1024) + (ovrf / 8192) + (ovrf / 16384) + (ovrf / 32768);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 4000000UL
  // 4 MHz
  //     Best Error Possible: 0.0114%  (0.0001138951 Decimal)
  //    Worst Error Possible: 2.3447% (0.0234473254 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 23447UL
    //  Error: 2.3447% (0.0234473254 Decimal)
    // Jitter: 0.0062% (0.0000625 Decimal)
    x = (ovrf * 0)  + (ovrf / 16);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8199UL
    //  Error: 0.8199% (0.008199243200000001 Decimal)
    // Jitter: 0.0120% (0.0001203125 Decimal)
    x = (ovrf * 0)  + (ovrf / 16) + (ovrf / 1024);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 580UL
    //  Error: 0.0580% (0.0005804791 Decimal)
    // Jitter: 0.0177% (0.00017671090000000002 Decimal)
    x = (ovrf * 0)  + (ovrf / 16) + (ovrf / 1024) + (ovrf / 2048);
  #else
    //  Error: 0.0114% (0.0001138951 Decimal)
    // Jitter: 0.0224% (0.0002238095 Decimal)
    x = (ovrf * 0)  + (ovrf / 16) + (ovrf / 1024) + (ovrf / 2048) + (ovrf / 32768);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 4800000UL
  // 4.8 MHz
  //     Best Error Possible: 0.0153%  (0.00015272219999999998 Decimal)
  //    Worst Error Possible: 41.4072% (0.4140718057 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 414072UL
    //  Error: 41.4072% (0.4140718057 Decimal)
    // Jitter: 0.0055% (0.0000551471 Decimal)
    x = (ovrf * 0)  + (ovrf / 32);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 121113UL
    //  Error: 12.1113% (0.1211131569 Decimal)
    // Jitter: 0.0106% (0.00010625 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 64);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 47881UL
    //  Error: 4.7881% (0.0478814326 Decimal)
    // Jitter: 0.0148% (0.0001484375 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 64) + (ovrf / 256);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 11271UL
    //  Error: 1.1271% (0.011270545 Decimal)
    // Jitter: 0.0223% (0.0002229665 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 64) + (ovrf / 256) + (ovrf / 512);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 2126UL
    //  Error: 0.2126% (0.0021257332 Decimal)
    // Jitter: 0.0283% (0.0002829912 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 64) + (ovrf / 256) + (ovrf / 512) + (ovrf / 2048);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 992UL
    //  Error: 0.0992% (0.0009916172 Decimal)
    // Jitter: 0.0329% (0.0003291789 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 64) + (ovrf / 256) + (ovrf / 512) + (ovrf / 2048) + (ovrf / 16384);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 429UL
    //  Error: 0.0429% (0.0004293574 Decimal)
    // Jitter: 0.0377% (0.0003769841 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 64) + (ovrf / 256) + (ovrf / 512) + (ovrf / 2048) + (ovrf / 16384) + (ovrf / 32768);
  #else
    //  Error: 0.0153% (0.00015272219999999998 Decimal)
    // Jitter: 0.0413% (0.0004126984 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 64) + (ovrf / 256) + (ovrf / 512) + (ovrf / 2048) + (ovrf / 16384) + (ovrf / 32768) + (ovrf / 65536);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 6400000UL
  // 6.4 MHz
  //     Best Error Possible: 0.0237%  (0.0002371862 Decimal)
  //    Worst Error Possible: 21.8758% (0.21875796979999998 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 218758UL
    //  Error: 21.8758% (0.21875796979999998 Decimal)
    // Jitter: 0.0068% (0.00006818179999999999 Decimal)
    x = (ovrf * 0)  + (ovrf / 32);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 23455UL
    //  Error: 2.3455% (0.023454928400000002 Decimal)
    // Jitter: 0.0108% (0.0001079545 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 128);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 11258UL
    //  Error: 1.1258% (0.0112578935 Decimal)
    // Jitter: 0.0136% (0.0001362782 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 128) + (ovrf / 2048);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 5164UL
    //  Error: 0.5164% (0.0051639588 Decimal)
    // Jitter: 0.0186% (0.00018647909999999998 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 128) + (ovrf / 2048) + (ovrf / 4096);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 2123UL
    //  Error: 0.2123% (0.0021232221 Decimal)
    // Jitter: 0.0238% (0.0002382033 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 128) + (ovrf / 2048) + (ovrf / 4096) + (ovrf / 8192);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 608UL
    //  Error: 0.0608% (0.0006080995 Decimal)
    // Jitter: 0.0290% (0.0002903811 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 128) + (ovrf / 2048) + (ovrf / 4096) + (ovrf / 8192) + (ovrf / 16384);
  #else
    //  Error: 0.0237% (0.0002371862 Decimal)
    // Jitter: 0.0301% (0.0003012704 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 128) + (ovrf / 2048) + (ovrf / 4096) + (ovrf / 8192) + (ovrf / 16384) + (ovrf / 65536);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 8000000UL
  // 8 MHz
  //     Best Error Possible: 0.0114%  (0.0001138951 Decimal)
  //    Worst Error Possible: 2.3447% (0.0234473254 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 23447UL
    //  Error: 2.3447% (0.0234473254 Decimal)
    // Jitter: 0.0062% (0.0000625 Decimal)
    x = (ovrf * 0)  + (ovrf / 32);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8199UL
    //  Error: 0.8199% (0.008199243200000001 Decimal)
    // Jitter: 0.0120% (0.0001203125 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 2048);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 580UL
    //  Error: 0.0580% (0.0005804791 Decimal)
    // Jitter: 0.0177% (0.00017671090000000002 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 2048) + (ovrf / 4096);
  #else
    //  Error: 0.0114% (0.0001138951 Decimal)
    // Jitter: 0.0224% (0.0002238095 Decimal)
    x = (ovrf * 0)  + (ovrf / 32) + (ovrf / 2048) + (ovrf / 4096) + (ovrf / 65536);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 9600000UL
  // 9.6 MHz
  //     Best Error Possible: 0.0429%  (0.0004293574 Decimal)
  //    Worst Error Possible: 41.4072% (0.4140718057 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 414072UL
    //  Error: 41.4072% (0.4140718057 Decimal)
    // Jitter: 0.0055% (0.0000551471 Decimal)
    x = (ovrf * 0)  + (ovrf / 64);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 121113UL
    //  Error: 12.1113% (0.1211131569 Decimal)
    // Jitter: 0.0106% (0.00010625 Decimal)
    x = (ovrf * 0)  + (ovrf / 64) + (ovrf / 128);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 47881UL
    //  Error: 4.7881% (0.0478814326 Decimal)
    // Jitter: 0.0148% (0.0001484375 Decimal)
    x = (ovrf * 0)  + (ovrf / 64) + (ovrf / 128) + (ovrf / 512);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 11271UL
    //  Error: 1.1271% (0.011270545 Decimal)
    // Jitter: 0.0223% (0.0002229665 Decimal)
    x = (ovrf * 0)  + (ovrf / 64) + (ovrf / 128) + (ovrf / 512) + (ovrf / 1024);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 2126UL
    //  Error: 0.2126% (0.0021257332 Decimal)
    // Jitter: 0.0283% (0.0002829912 Decimal)
    x = (ovrf * 0)  + (ovrf / 64) + (ovrf / 128) + (ovrf / 512) + (ovrf / 1024) + (ovrf / 4096);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 992UL
    //  Error: 0.0992% (0.0009916172 Decimal)
    // Jitter: 0.0329% (0.0003291789 Decimal)
    x = (ovrf * 0)  + (ovrf / 64) + (ovrf / 128) + (ovrf / 512) + (ovrf / 1024) + (ovrf / 4096) + (ovrf / 32768);
  #else
    //  Error: 0.0429% (0.0004293574 Decimal)
    // Jitter: 0.0377% (0.0003769841 Decimal)
    x = (ovrf * 0)  + (ovrf / 64) + (ovrf / 128) + (ovrf / 512) + (ovrf / 1024) + (ovrf / 4096) + (ovrf / 32768) + (ovrf / 65536);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 12000000UL
  // 12 MHz
  //     Best Error Possible: 0.0146%  (0.0001455417 Decimal)
  //    Worst Error Possible: 26.7588% (0.267588149 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 267588UL
    //  Error: 26.7588% (0.267588149 Decimal)
    // Jitter: 0.0065% (0.0000647321 Decimal)
    x = (ovrf * 0)  + (ovrf / 64);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 84492UL
    //  Error: 8.4492% (0.0844921373 Decimal)
    // Jitter: 0.0098% (0.0000977444 Decimal)
    x = (ovrf * 0)  + (ovrf / 64) + (ovrf / 256);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 38726UL
    //  Error: 3.8726% (0.038726386 Decimal)
    // Jitter: 0.0160% (0.0001597744 Decimal)
    x = (ovrf * 0)  + (ovrf / 64) + (ovrf / 256) + (ovrf / 1024);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 15849UL
    //  Error: 1.5849% (0.015849382 Decimal)
    // Jitter: 0.0188% (0.0001879699 Decimal)
    x = (ovrf * 0)  + (ovrf / 64) + (ovrf / 256) + (ovrf / 1024) + (ovrf / 2048);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 4417UL
    //  Error: 0.4417% (0.0044165132000000005 Decimal)
    // Jitter: 0.0268% (0.0002679426 Decimal)
    x = (ovrf * 0)  + (ovrf / 64) + (ovrf / 256) + (ovrf / 1024) + (ovrf / 2048) + (ovrf / 4096);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 1566UL
    //  Error: 0.1566% (0.0015659282000000001 Decimal)
    // Jitter: 0.0307% (0.00030690359999999996 Decimal)
    x = (ovrf * 0)  + (ovrf / 64) + (ovrf / 256) + (ovrf / 1024) + (ovrf / 2048) + (ovrf / 4096) + (ovrf / 16384);
  #else
    //  Error: 0.0146% (0.0001455417 Decimal)
    // Jitter: 0.0372% (0.0003718387 Decimal)
    x = (ovrf * 0)  + (ovrf / 64) + (ovrf / 256) + (ovrf / 1024) + (ovrf / 2048) + (ovrf / 4096) + (ovrf / 16384) + (ovrf / 32768);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 16000000UL
  // 16 MHz
  //     Best Error Possible: 0.0580%  (0.0005804791 Decimal)
  //    Worst Error Possible: 2.3447% (0.0234473254 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 23447UL
    //  Error: 2.3447% (0.0234473254 Decimal)
    // Jitter: 0.0062% (0.0000625 Decimal)
    x = (ovrf * 0)  + (ovrf / 64);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8199UL
    //  Error: 0.8199% (0.008199243200000001 Decimal)
    // Jitter: 0.0120% (0.0001203125 Decimal)
    x = (ovrf * 0)  + (ovrf / 64) + (ovrf / 4096);
  #else
    //  Error: 0.0580% (0.0005804791 Decimal)
    // Jitter: 0.0177% (0.00017671090000000002 Decimal)
    x = (ovrf * 0)  + (ovrf / 64) + (ovrf / 4096) + (ovrf / 8192);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 20000000UL
  // 20 MHz
  //     Best Error Possible: 0.1080%  (0.0010804776999999998 Decimal)
  //    Worst Error Possible: 38.9659% (0.38965908730000004 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 389659UL
    //  Error: 38.9659% (0.38965908730000004 Decimal)
    // Jitter: 0.0079% (0.0000788352 Decimal)
    x = (ovrf * 0)  + (ovrf / 128);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 84494UL
    //  Error: 8.4494% (0.0844941297 Decimal)
    // Jitter: 0.0163% (0.00016339070000000001 Decimal)
    x = (ovrf * 0)  + (ovrf / 128) + (ovrf / 256);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8211UL
    //  Error: 0.8211% (0.00821125 Decimal)
    // Jitter: 0.0218% (0.0002175182 Decimal)
    x = (ovrf * 0)  + (ovrf / 128) + (ovrf / 256) + (ovrf / 1024);
  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 3454UL
    //  Error: 0.3454% (0.0034536073999999997 Decimal)
    // Jitter: 0.0284% (0.0002839416 Decimal)
    x = (ovrf * 0)  + (ovrf / 128) + (ovrf / 256) + (ovrf / 1024) + (ovrf / 16384);
  #else
    //  Error: 0.1080% (0.0010804776999999998 Decimal)
    // Jitter: 0.0367% (0.0003666667 Decimal)
    x = (ovrf * 0)  + (ovrf / 128) + (ovrf / 256) + (ovrf / 1024) + (ovrf / 16384) + (ovrf / 32768);
  #endif
    
#else
  #error This CPU frequency is not defined
#endif
  SREG = oldSREG;
  return x;
}

MillisMicrosTime_t micros()
{
  MillisMicrosTime_t x;
  uint8_t  oldSREG = SREG;
  cli();
  
  // To come up with these calculations I wrote a Google Spreadsheet which includes 
  // an automation script to generated the C code required.  Should you need to
  // add different frequencies in future, feel free to copy the sheet and use it.
  //
  //   https://goo.gl/sgANEr
  //
  // James Sleeman, james@gogo.co.nz, http://sparks.gogo.co.nz/
  
#if (F_CPU / MILLIS_TIMER_PRESCALE) == 16000UL
  // 16 kHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  x = (ovrf * 16000) ;


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 32768UL
  // 32.768 kHz
  //  Error: 0.0127% (0.0001274919 Decimal)
  // Jitter: 0.0064% (0.00006399179999999999 Decimal)

  x = (ovrf * 7812)  + (ovrf / 2);


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 125000UL
  // 125 kHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  x = (ovrf * 2048) ;


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 128000UL
  // 128 kHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  x = (ovrf * 2000) ;


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 600000UL
  // 600 kHz
  //     Best Error Possible: 0.0037%  (0.000036858 Decimal)
  //    Worst Error Possible: 0.1562% (0.0015624011 Decimal)

  #if      ACCEPTABLE_MICROS_ERROR_PPM >= 1562UL
    //  Error: 0.1562% (0.0015624011 Decimal)
    // Jitter: 0.0058% (0.000057859400000000004 Decimal)
    x = (ovrf * 426) ;
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 391UL
    //  Error: 0.0391% (0.0003910875 Decimal)
    // Jitter: 0.0102% (0.0001019122 Decimal)
    x = (ovrf * 426)  + (ovrf / 2);
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 107UL
    //  Error: 0.0107% (0.00010718529999999999 Decimal)
    // Jitter: 0.0189% (0.000189256 Decimal)
    x = (ovrf * 426)  + (ovrf / 2) + (ovrf / 8);
  #else
    //  Error: 0.0037% (0.000036858 Decimal)
    // Jitter: 0.0263% (0.0002625 Decimal)
    x = (ovrf * 426)  + (ovrf / 2) + (ovrf / 8) + (ovrf / 32) + (ovrf / 128) + (ovrf / 512) + (ovrf / 2048) + (ovrf / 8192) + (ovrf / 32768);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 800000UL
  // 800 kHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  x = (ovrf * 320) ;


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 1000000UL
  // 1 MHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  x = (ovrf * 256) ;


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 1200000UL
  // 1.2 MHz
  //     Best Error Possible: 0.0042%  (0.0000423357 Decimal)
  //    Worst Error Possible: 0.1562% (0.0015623306 Decimal)

  #if      ACCEPTABLE_MICROS_ERROR_PPM >= 1562UL
    //  Error: 0.1562% (0.0015623306 Decimal)
    // Jitter: 0.0055% (0.0000551636 Decimal)
    x = (ovrf * 213) ;
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 398UL
    //  Error: 0.0398% (0.0003979971 Decimal)
    // Jitter: 0.0102% (0.0001019122 Decimal)
    x = (ovrf * 213)  + (ovrf / 4);
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 115UL
    //  Error: 0.0115% (0.0001147185 Decimal)
    // Jitter: 0.0179% (0.0001787935 Decimal)
    x = (ovrf * 213)  + (ovrf / 4) + (ovrf / 16);
  #else
    //  Error: 0.0042% (0.0000423357 Decimal)
    // Jitter: 0.0255% (0.0002545819 Decimal)
    x = (ovrf * 213)  + (ovrf / 4) + (ovrf / 16) + (ovrf / 64) + (ovrf / 256) + (ovrf / 1024) + (ovrf / 4096) + (ovrf / 16384) + (ovrf / 65536);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 2000000UL
  // 2 MHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  x = (ovrf * 128) ;


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 2400000UL
  // 2.4 MHz
  //     Best Error Possible: 0.0059%  (0.0000591303 Decimal)
  //    Worst Error Possible: 0.6257% (0.00625683 Decimal)

  #if      ACCEPTABLE_MICROS_ERROR_PPM >= 6257UL
    //  Error: 0.6257% (0.00625683 Decimal)
    // Jitter: 0.0030% (0.0000301493 Decimal)
    x = (ovrf * 106) ;
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 1575UL
    //  Error: 0.1575% (0.0015753006 Decimal)
    // Jitter: 0.0085% (0.0000849592 Decimal)
    x = (ovrf * 106)  + (ovrf / 2);
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 413UL
    //  Error: 0.0413% (0.0004128806 Decimal)
    // Jitter: 0.0157% (0.0001567544 Decimal)
    x = (ovrf * 106)  + (ovrf / 2) + (ovrf / 8);
  #else
    //  Error: 0.0059% (0.0000591303 Decimal)
    // Jitter: 0.0334% (0.00033427019999999997 Decimal)
    x = (ovrf * 106)  + (ovrf / 2) + (ovrf / 8) + (ovrf / 32) + (ovrf / 128) + (ovrf / 512) + (ovrf / 2048) + (ovrf / 8192) + (ovrf / 32768);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 4000000UL
  // 4 MHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  x = (ovrf * 64) ;


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 4800000UL
  // 4.8 MHz
  //     Best Error Possible: 0.0054%  (0.0000543994 Decimal)
  //    Worst Error Possible: 0.6250% (0.006250170100000001 Decimal)

  #if      ACCEPTABLE_MICROS_ERROR_PPM >= 6250UL
    //  Error: 0.6250% (0.006250170100000001 Decimal)
    // Jitter: 0.0056% (0.0000556046 Decimal)
    x = (ovrf * 53) ;
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 1571UL
    //  Error: 0.1571% (0.0015707754 Decimal)
    // Jitter: 0.0113% (0.0001130754 Decimal)
    x = (ovrf * 53)  + (ovrf / 4);
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 409UL
    //  Error: 0.0409% (0.00040873 Decimal)
    // Jitter: 0.0166% (0.00016610730000000002 Decimal)
    x = (ovrf * 53)  + (ovrf / 4) + (ovrf / 16);
  #else
    //  Error: 0.0054% (0.0000543994 Decimal)
    // Jitter: 0.0341% (0.0003409332 Decimal)
    x = (ovrf * 53)  + (ovrf / 4) + (ovrf / 16) + (ovrf / 64) + (ovrf / 256) + (ovrf / 1024) + (ovrf / 4096) + (ovrf / 16384) + (ovrf / 65536);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 6400000UL
  // 6.4 MHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  x = (ovrf * 40) ;


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 8000000UL
  // 8 MHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  x = (ovrf * 32) ;


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 9600000UL
  // 9.6 MHz
  //     Best Error Possible: 0.0068%  (0.0000677557 Decimal)
  //    Worst Error Possible: 2.5000% (0.0249999037 Decimal)

  #if      ACCEPTABLE_MICROS_ERROR_PPM >= 25000UL
    //  Error: 2.5000% (0.0249999037 Decimal)
    // Jitter: 0.0057% (0.000056533099999999996 Decimal)
    x = (ovrf * 26) ;
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 6260UL
    //  Error: 0.6260% (0.0062602516 Decimal)
    // Jitter: 0.0068% (0.00006789860000000001 Decimal)
    x = (ovrf * 26)  + (ovrf / 2);
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 1583UL
    //  Error: 0.1583% (0.0015833386 Decimal)
    // Jitter: 0.0135% (0.0001353933 Decimal)
    x = (ovrf * 26)  + (ovrf / 2) + (ovrf / 8);
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 422UL
    //  Error: 0.0422% (0.0004219071 Decimal)
    // Jitter: 0.0207% (0.0002065505 Decimal)
    x = (ovrf * 26)  + (ovrf / 2) + (ovrf / 8) + (ovrf / 32);
  #else
    //  Error: 0.0068% (0.0000677557 Decimal)
    // Jitter: 0.0384% (0.0003841938 Decimal)
    x = (ovrf * 26)  + (ovrf / 2) + (ovrf / 8) + (ovrf / 32) + (ovrf / 128) + (ovrf / 512) + (ovrf / 2048) + (ovrf / 8192) + (ovrf / 32768);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 12000000UL
  // 12 MHz
  //     Best Error Possible: 0.0060%  (0.000059592599999999996 Decimal)
  //    Worst Error Possible: 1.5625% (0.0156250709 Decimal)

  #if      ACCEPTABLE_MICROS_ERROR_PPM >= 15625UL
    //  Error: 1.5625% (0.0156250709 Decimal)
    // Jitter: 0.0053% (0.0000525339 Decimal)
    x = (ovrf * 21) ;
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 3914UL
    //  Error: 0.3914% (0.0039139441 Decimal)
    // Jitter: 0.0108% (0.0001075354 Decimal)
    x = (ovrf * 21)  + (ovrf / 4);
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 994UL
    //  Error: 0.0994% (0.0009935546 Decimal)
    // Jitter: 0.0116% (0.0001164998 Decimal)
    x = (ovrf * 21)  + (ovrf / 4) + (ovrf / 16);
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 271UL
    //  Error: 0.0271% (0.0002709633 Decimal)
    // Jitter: 0.0176% (0.0001763528 Decimal)
    x = (ovrf * 21)  + (ovrf / 4) + (ovrf / 16) + (ovrf / 64);
  #else
    //  Error: 0.0060% (0.000059592599999999996 Decimal)
    // Jitter: 0.0241% (0.00024061930000000002 Decimal)
    x = (ovrf * 21)  + (ovrf / 4) + (ovrf / 16) + (ovrf / 64) + (ovrf / 256) + (ovrf / 1024) + (ovrf / 4096) + (ovrf / 16384) + (ovrf / 65536);
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 16000000UL
  // 16 MHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  x = (ovrf * 16) ;


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 20000000UL
  // 20 MHz
  //     Best Error Possible: 0.0077%  (0.0000766896 Decimal)
  //    Worst Error Possible: 6.2500% (0.0625000096 Decimal)

  #if      ACCEPTABLE_MICROS_ERROR_PPM >= 62500UL
    //  Error: 6.2500% (0.0625000096 Decimal)
    // Jitter: 0.0058% (0.000058070500000000006 Decimal)
    x = (ovrf * 12) ;
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 23448UL
    //  Error: 2.3448% (0.0234480564 Decimal)
    // Jitter: 0.0073% (0.0000734384 Decimal)
    x = (ovrf * 12)  + (ovrf / 2);
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 3927UL
    //  Error: 0.3927% (0.0039272088 Decimal)
    // Jitter: 0.0094% (0.00009441930000000001 Decimal)
    x = (ovrf * 12)  + (ovrf / 2) + (ovrf / 4);
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 1496UL
    //  Error: 0.1496% (0.0014961941999999998 Decimal)
    // Jitter: 0.0167% (0.0001671609 Decimal)
    x = (ovrf * 12)  + (ovrf / 2) + (ovrf / 4) + (ovrf / 32);
  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 286UL
    //  Error: 0.0286% (0.0002857473 Decimal)
    // Jitter: 0.0211% (0.0002114706 Decimal)
    x = (ovrf * 12)  + (ovrf / 2) + (ovrf / 4) + (ovrf / 32) + (ovrf / 64);
  #else
    //  Error: 0.0077% (0.0000766896 Decimal)
    // Jitter: 0.0368% (0.00036802030000000004 Decimal)
    x = (ovrf * 12)  + (ovrf / 2) + (ovrf / 4) + (ovrf / 32) + (ovrf / 64) + (ovrf / 512) + (ovrf / 1024) + (ovrf / 8192) + (ovrf / 16384);
  #endif
#else
  #error This CPU frequency is not defined
#endif
  
  SREG = oldSREG;
  return x;
}
#endif

void delay(DelayTime_t ms)
{
  while(ms--){
    delayMicroseconds(1000); 
  }
#if 0
#ifdef NO_MILLIS
  while(ms--){
    delayMicroseconds(1000); 
  }
#else
  // If millis() is present, that means we have an interrupt running which will
  // screw up delayMicroseconds() unless we disable interrupts (this problem
  // exists in the Arduino core too, not much you can do about it without 
  // spending code on checking for interrupts and adjusting and even then
  // we have no way to know how big/often any other interrupts that might be
  // configured are).
  //
  // Anyway, we can't use delayMicroseconds() for this if millis() is available
  // or the delay will be quite inaccurate.
  
  MillisMicrosTime_t current = millis();
  while(millis() - current < ms);
  return;
#endif
#endif
}

// For clock-counting/sim/debug you might want to add
//   __attribute__ ((noinline)) 
// so it's easier to see what's going on in the decompilation
void delayMicrosecondsWithoutMillisInterruptAdjustment(DelayMicrosecondsTime_t us)
{
  // This is pretty much the standard Arduino delayMicroseconds() however I have 
  // recalculated all the numbers for improved consistency and accuracy, and 
  // have done away with any attempt to handle delays shorter than the minimum 
  // overhead + 1 busy-loop.  That is, if you try and delayMicroseconds() for 
  // shorter than we can do, it will just return immediately.  
  //
  // As of writing, the minimum us delay to actually get a busy-loop are:
  //
  //  24MHz: 1
  //  20MHz: 4
  //  16MHz: 2
  //  12MHz: 6
  //  9.6MHz: 9
  //  8MHz: 3
  //  6.4MHz: 12
  //  4.8MHz: 16
  //  4.0MHz: 4
  //  2.4MHz: 33
  //  2.0MHz: 9
  //  1.2MHz: 64
  //  1.0MHz: 21
  //  800kHz: 91
  //  600kHz: 132
  //  128kHz: 323
  //  125kHz: 329
  //  32.768kHz: 643
  //  16kHz: 1126
  //
  // anything shorter than those will return in close to ~0us, anything longer
  // than those should be pretty close to the requested us.
  //
  // To come up with these calculations I wrote a Google Spreadsheet which includes 
  // an automation script to generated the C code required.  Should you need to
  // add different frequencies in future, feel free to copy the sheet and use it.
  //
  //   https://goo.gl/sgANEr
  //
  // James Sleeman, james@gogo.co.nz, http://sparks.gogo.co.nz/
  
#if F_CPU >= 24000000UL
  // 24MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 0) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 6.0000 * us
  //        Adjustment Takes: 4 Clocks (Approx)
  us = ( us * 6) ;

  // Compensate for the combined overhead time of 0.5833us
  // by subtracting 3 Loop Cycles
  // us at this point is at least 6 Loops - 1us
  //       Subtraction Takes: 2 Clocks
  us -= 3;


#elif F_CPU >= 20000000UL
  // 20MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 3) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 5.0000 * us
  //        Adjustment Takes: 57 Clocks (Approx)
  us = ( us * 5) ;

  // Compensate for the combined overhead time of 3.3500us
  // by subtracting 16 Loop Cycles
  // us at this point is at least 20 Loops - 4us
  //       Subtraction Takes: 2 Clocks
  us -= 16;


#elif F_CPU >= 16000000UL
  // 16MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 1) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 4.0000 * us
  //        Adjustment Takes: 8 Clocks (Approx)
  us = ( us * 4) ;

  // Compensate for the combined overhead time of 1.1250us
  // by subtracting 4 Loop Cycles
  // us at this point is at least 8 Loops - 2us
  //       Subtraction Takes: 2 Clocks
  us -= 4;


#elif F_CPU >= 12000000UL
  // 12MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 5) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 3.0000 * us
  //        Adjustment Takes: 50 Clocks (Approx)
  us = ( us * 3) ;

  // Compensate for the combined overhead time of 5.0000us
  // by subtracting 15 Loop Cycles
  // us at this point is at least 18 Loops - 6us
  //       Subtraction Takes: 2 Clocks
  us -= 15;


#elif F_CPU >= 9600000UL
  // 9.6MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 8) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 2.4000 * us
  //        Adjustment Takes: 68 Clocks (Approx)
  us = ( us * 2) + (us / 4) + (us / 8) + (us / 64) + (us / 128) ;

  // Compensate for the combined overhead time of 8.1250us
  // by subtracting 19 Loop Cycles
  // us at this point is at least 21 Loops - 9us
  //       Subtraction Takes: 2 Clocks
  us -= 19;


#elif F_CPU >= 8000000UL
  // 8MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 2) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 2.0000 * us
  //        Adjustment Takes: 6 Clocks (Approx)
  us = ( us * 2) ;

  // Compensate for the combined overhead time of 2.0000us
  // by subtracting 4 Loop Cycles
  // us at this point is at least 6 Loops - 3us
  //       Subtraction Takes: 2 Clocks
  us -= 4;


#elif F_CPU >= 6400000UL
  // 6.4MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 11) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.6000 * us
  //        Adjustment Takes: 59 Clocks (Approx)
  us = ( us * 1) + (us / 2) + (us / 16) + (us / 32) + (us / 256);

  // Compensate for the combined overhead time of 10.7813us
  // by subtracting 17 Loop Cycles
  // us at this point is at least 18 Loops - 12us
  //       Subtraction Takes: 2 Clocks
  us -= 17;


#elif F_CPU >= 4800000UL
  // 4.8MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 15) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.2000 * us
  //        Adjustment Takes: 54 Clocks (Approx)
  us = ( us * 1) + (us / 8) + (us / 16) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 13.3333us
  // by subtracting 16 Loop Cycles
  // us at this point is at least 19 Loops - 16us
  //       Subtraction Takes: 2 Clocks
  us -= 16;


#elif F_CPU >= 4000000UL
  // 4MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 3) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.0000 * us
  //        Adjustment Takes: 0 Clocks (Approx)
  us = ( us * 1) ;

  // Compensate for the combined overhead time of 2.5000us
  // by subtracting 2 Loop Cycles
  // us at this point is at least 4 Loops - 4us
  //       Subtraction Takes: 2 Clocks
  us -= 2;


#elif F_CPU >= 2400000UL
  // 2.4MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 32) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.6000 * us
  //        Adjustment Takes: 55 Clocks (Approx)
  us = ( us * 0) + (us / 2) + (us / 16) + (us / 32) + (us / 256);

  // Compensate for the combined overhead time of 27.0833us
  // by subtracting 16 Loop Cycles
  // us at this point is at least 19 Loops - 33us
  //       Subtraction Takes: 2 Clocks
  us -= 16;


#elif F_CPU >= 2000000UL
  // 2MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 8) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.5000 * us
  //        Adjustment Takes: 2 Clocks (Approx)
  us = ( us * 0) + (us / 2) ;

  // Compensate for the combined overhead time of 6.0000us
  // by subtracting 3 Loop Cycles
  // us at this point is at least 4 Loops - 9us
  //       Subtraction Takes: 2 Clocks
  us -= 3;


#elif F_CPU >= 1200000UL
  // 1.2MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 63) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.3000 * us
  //        Adjustment Takes: 65 Clocks (Approx)
  us = ( us * 0) + (us / 4) + (us / 32) + (us / 64) ;

  // Compensate for the combined overhead time of 62.5000us
  // by subtracting 18 Loop Cycles
  // us at this point is at least 19 Loops - 64us
  //       Subtraction Takes: 2 Clocks
  us -= 18;


#elif F_CPU >= 1000000UL
  // 1MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 20) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.2500 * us
  //        Adjustment Takes: 6 Clocks (Approx)
  us = ( us * 0) + (us / 4) ;

  // Compensate for the combined overhead time of 16.0000us
  // by subtracting 4 Loop Cycles
  // us at this point is at least 5 Loops - 21us
  //       Subtraction Takes: 2 Clocks
  us -= 4;


#elif F_CPU >= 800000UL
  // 800kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 90) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.2000 * us
  //        Adjustment Takes: 50 Clocks (Approx)
  us = ( us * 0) + (us / 8) + (us / 16) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 75.0000us
  // by subtracting 15 Loop Cycles
  // us at this point is at least 16 Loops - 91us
  //       Subtraction Takes: 2 Clocks
  us -= 15;


#elif F_CPU >= 600000UL
  // 600kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 131) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.1500 * us
  //        Adjustment Takes: 56 Clocks (Approx)
  us = ( us * 0) + (us / 8) + (us / 64) + (us / 128) ;

  // Compensate for the combined overhead time of 110.0000us
  // by subtracting 16 Loop Cycles
  // us at this point is at least 19 Loops - 132us
  //       Subtraction Takes: 2 Clocks
  us -= 16;


#elif F_CPU >= 128000UL
  // 128kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 322) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0320 * us
  //        Adjustment Takes: 27 Clocks (Approx)
  us = ( us * 0) + (us / 32) ;

  // Compensate for the combined overhead time of 289.0625us
  // by subtracting 9 Loop Cycles
  // us at this point is at least 10 Loops - 323us
  //       Subtraction Takes: 2 Clocks
  us -= 9;


#elif F_CPU >= 125000UL
  // 125kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 328) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0313 * us
  //        Adjustment Takes: 27 Clocks (Approx)
  us = ( us * 0) + (us / 32) ;

  // Compensate for the combined overhead time of 296.0000us
  // by subtracting 9 Loop Cycles
  // us at this point is at least 10 Loops - 329us
  //       Subtraction Takes: 2 Clocks
  us -= 9;


#elif F_CPU >= 32768UL
  // 32.768kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 642) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0082 * us
  //        Adjustment Takes: 7 Clocks (Approx)
  us = ( us * 0) + (us / 128) ;

  // Compensate for the combined overhead time of 518.7988us
  // by subtracting 4 Loop Cycles
  // us at this point is at least 5 Loops - 643us
  //       Subtraction Takes: 2 Clocks
  us -= 4;


#elif F_CPU >= 16000UL
  // 16kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 1125) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0040 * us
  //        Adjustment Takes: 4 Clocks (Approx)
  us = ( us * 0) + (us / 256);

  // Compensate for the combined overhead time of 875.0000us
  // by subtracting 3 Loop Cycles
  // us at this point is at least 4 Loops - 1126us
  //       Subtraction Takes: 2 Clocks
  us -= 3;


#endif

  // busy wait
  __asm__ __volatile__ (
    "1: sbiw %0,1" "\n\t" // 2 cycles
    "brne 1b" : "=w" (us) : "0" (us) // 2 cycles
  );
  // return = 4 cycles
}

void delayMicrosecondsAdjustedForMillisInterrupt(DelayMicrosecondsTime_t us)
{
#if MILLIS_TIMER_PRESCALE == 1
#if F_CPU >= 24000000UL
  // 24MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 10) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.9152 * us
  //        Adjustment Takes: 63 Clocks (Approx)
  us = ( us * 1) + (us / 2) + (us / 4) + (us / 8) + (us / 32) + (us / 128) ;

  // Compensate for the combined overhead time of 3.9684us
  // by subtracting 18 Loop Cycles
  // us at this point is at least 19 Loops - 11us
  //       Subtraction Takes: 2 Clocks
  us -= 18;


#elif F_CPU >= 20000000UL
  // 20MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 13) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.9814 * us
  //        Adjustment Takes: 89 Clocks (Approx)
  us = ( us * 1) + (us / 2) + (us / 4) + (us / 8) + (us / 16) + (us / 32) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 4.9500us
  // by subtracting 24 Loop Cycles
  // us at this point is at least 25 Loops - 14us
  //       Subtraction Takes: 2 Clocks
  us -= 24;


#elif F_CPU >= 16000000UL
  // 16MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 12) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.8028 * us
  //        Adjustment Takes: 75 Clocks (Approx)
  us = ( us * 1) + (us / 2) + (us / 4) + (us / 32) + (us / 64) + (us / 256);

  // Compensate for the combined overhead time of 5.3125us
  // by subtracting 21 Loop Cycles
  // us at this point is at least 22 Loops - 13us
  //       Subtraction Takes: 2 Clocks
  us -= 21;


#elif F_CPU >= 12000000UL
  // 12MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 7) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.5673 * us
  //        Adjustment Takes: 32 Clocks (Approx)
  us = ( us * 1) + (us / 2) + (us / 16) + (us / 256);

  // Compensate for the combined overhead time of 3.5000us
  // by subtracting 10 Loop Cycles
  // us at this point is at least 12 Loops - 8us
  //       Subtraction Takes: 2 Clocks
  us -= 10;


#elif F_CPU >= 9600000UL
  // 9.6MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 12) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.8395 * us
  //        Adjustment Takes: 73 Clocks (Approx)
  us = ( us * 1) + (us / 2) + (us / 4) + (us / 16) + (us / 64) + (us / 128) ;

  // Compensate for the combined overhead time of 11.2801us
  // by subtracting 21 Loop Cycles
  // us at this point is at least 22 Loops - 13us
  //       Subtraction Takes: 2 Clocks
  us -= 21;


#elif F_CPU >= 8000000UL
  // 8MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 25) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.2427 * us
  //        Adjustment Takes: 109 Clocks (Approx)
  us = ( us * 1) + (us / 8) + (us / 16) + (us / 32) + (us / 64) + (us / 128) ;

  // Compensate for the combined overhead time of 14.8750us
  // by subtracting 29 Loop Cycles
  // us at this point is at least 30 Loops - 26us
  //       Subtraction Takes: 2 Clocks
  us -= 29;


#elif F_CPU >= 6400000UL
  // 6.4MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 11) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.0756 * us
  //        Adjustment Takes: 37 Clocks (Approx)
  us = ( us * 1) + (us / 16) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 7.3438us
  // by subtracting 11 Loop Cycles
  // us at this point is at least 12 Loops - 12us
  //       Subtraction Takes: 2 Clocks
  us -= 11;


#elif F_CPU >= 4800000UL
  // 4.8MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 11) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.8787 * us
  //        Adjustment Takes: 25 Clocks (Approx)
  us = ( us * 0) + (us / 2) + (us / 4) + (us / 8) ;

  // Compensate for the combined overhead time of 7.2917us
  // by subtracting 8 Loop Cycles
  // us at this point is at least 10 Loops - 12us
  //       Subtraction Takes: 2 Clocks
  us -= 8;


#elif F_CPU >= 4000000UL
  // 4MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 17) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.7665 * us
  //        Adjustment Takes: 40 Clocks (Approx)
  us = ( us * 0) + (us / 2) + (us / 4) + (us / 64) ;

  // Compensate for the combined overhead time of 12.5000us
  // by subtracting 12 Loop Cycles
  // us at this point is at least 13 Loops - 18us
  //       Subtraction Takes: 2 Clocks
  us -= 12;


#elif F_CPU >= 2400000UL
  // 2.4MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 9) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.5073 * us
  //        Adjustment Takes: 6 Clocks (Approx)
  us = ( us * 0) + (us / 2) + (us / 256);

  // Compensate for the combined overhead time of 6.6667us
  // by subtracting 4 Loop Cycles
  // us at this point is at least 5 Loops - 10us
  //       Subtraction Takes: 2 Clocks
  us -= 4;


#elif F_CPU >= 2000000UL
  // 2MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 64) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.4339 * us
  //        Adjustment Takes: 93 Clocks (Approx)
  us = ( us * 0) + (us / 4) + (us / 8) + (us / 32) + (us / 64) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 51.5000us
  // by subtracting 25 Loop Cycles
  // us at this point is at least 27 Loops - 65us
  //       Subtraction Takes: 2 Clocks
  us -= 25;


#elif F_CPU >= 1200000UL
  // 1.2MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 105) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.2299 * us
  //        Adjustment Takes: 73 Clocks (Approx)
  us = ( us * 0) + (us / 8) + (us / 16) + (us / 32) + (us / 128) ;

  // Compensate for the combined overhead time of 90.2409us
  // by subtracting 20 Loop Cycles
  // us at this point is at least 22 Loops - 106us
  //       Subtraction Takes: 2 Clocks
  us -= 20;


#elif F_CPU >= 1000000UL
  // 1MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 103) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.2323 * us
  //        Adjustment Takes: 77 Clocks (Approx)
  us = ( us * 0) + (us / 8) + (us / 16) + (us / 32) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 87.0000us
  // by subtracting 21 Loop Cycles
  // us at this point is at least 22 Loops - 104us
  //       Subtraction Takes: 2 Clocks
  us -= 21;


#elif F_CPU >= 800000UL
  // 800kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 77) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.1885 * us
  //        Adjustment Takes: 39 Clocks (Approx)
  us = ( us * 0) + (us / 8) + (us / 16) ;

  // Compensate for the combined overhead time of 61.2500us
  // by subtracting 12 Loop Cycles
  // us at this point is at least 13 Loops - 78us
  //       Subtraction Takes: 2 Clocks
  us -= 12;


#elif F_CPU >= 600000UL
  // 600kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 113) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.1434 * us
  //        Adjustment Takes: 49 Clocks (Approx)
  us = ( us * 0) + (us / 8) + (us / 64) ;

  // Compensate for the combined overhead time of 98.3333us
  // by subtracting 14 Loop Cycles
  // us at this point is at least 15 Loops - 114us
  //       Subtraction Takes: 2 Clocks
  us -= 14;


#elif F_CPU >= 128000UL
  // 128kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 322) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0317 * us
  //        Adjustment Takes: 27 Clocks (Approx)
  us = ( us * 0) + (us / 32) ;

  // Compensate for the combined overhead time of 289.0625us
  // by subtracting 9 Loop Cycles
  // us at this point is at least 10 Loops - 323us
  //       Subtraction Takes: 2 Clocks
  us -= 9;


#elif F_CPU >= 125000UL
  // 125kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 552) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0310 * us
  //        Adjustment Takes: 43 Clocks (Approx)
  us = ( us * 0) + (us / 64) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 424.0000us
  // by subtracting 13 Loop Cycles
  // us at this point is at least 14 Loops - 553us
  //       Subtraction Takes: 2 Clocks
  us -= 13;


#elif F_CPU >= 32768UL
  // 32.768kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 642) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0082 * us
  //        Adjustment Takes: 7 Clocks (Approx)
  us = ( us * 0) + (us / 128) ;

  // Compensate for the combined overhead time of 518.7988us
  // by subtracting 4 Loop Cycles
  // us at this point is at least 5 Loops - 643us
  //       Subtraction Takes: 2 Clocks
  us -= 4;


#elif F_CPU >= 16000UL
  // 16kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 1125) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0040 * us
  //        Adjustment Takes: 4 Clocks (Approx)
  us = ( us * 0) + (us / 256);

  // Compensate for the combined overhead time of 875.0000us
  // by subtracting 3 Loop Cycles
  // us at this point is at least 4 Loops - 1126us
  //       Subtraction Takes: 2 Clocks
  us -= 3;


#endif
#elif MILLIS_TIMER_PRESCALE == 8
#if F_CPU >= 24000000UL
  // 24MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 6) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 4.7371 * us
  //        Adjustment Takes: 108 Clocks (Approx)
  us = ( us * 4) + (us / 2) + (us / 8) + (us / 16) + (us / 32) + (us / 64) ;

  // Compensate for the combined overhead time of 5.1039us
  // by subtracting 29 Loop Cycles
  // us at this point is at least 31 Loops - 7us
  //       Subtraction Takes: 2 Clocks
  us -= 29;


#elif F_CPU >= 20000000UL
  // 20MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 4) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 4.2002 * us
  //        Adjustment Takes: 58 Clocks (Approx)
  us = ( us * 4) + (us / 8) + (us / 16) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 3.4000us
  // by subtracting 17 Loop Cycles
  // us at this point is at least 20 Loops - 5us
  //       Subtraction Takes: 2 Clocks
  us -= 17;


#elif F_CPU >= 16000000UL
  // 16MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 10) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 3.4712 * us
  //        Adjustment Takes: 122 Clocks (Approx)
  us = ( us * 3) + (us / 4) + (us / 8) + (us / 16) + (us / 32) ;

  // Compensate for the combined overhead time of 8.2500us
  // by subtracting 33 Loop Cycles
  // us at this point is at least 36 Loops - 11us
  //       Subtraction Takes: 2 Clocks
  us -= 33;


#elif F_CPU >= 12000000UL
  // 12MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 6) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 2.6924 * us
  //        Adjustment Takes: 51 Clocks (Approx)
  us = ( us * 2) + (us / 2) + (us / 8) + (us / 16) + (us / 256);

  // Compensate for the combined overhead time of 5.0833us
  // by subtracting 15 Loop Cycles
  // us at this point is at least 17 Loops - 7us
  //       Subtraction Takes: 2 Clocks
  us -= 15;


#elif F_CPU >= 9600000UL
  // 9.6MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 10) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 2.3119 * us
  //        Adjustment Takes: 82 Clocks (Approx)
  us = ( us * 2) + (us / 4) + (us / 32) + (us / 64) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 9.9483us
  // by subtracting 23 Loop Cycles
  // us at this point is at least 24 Loops - 11us
  //       Subtraction Takes: 2 Clocks
  us -= 23;


#elif F_CPU >= 8000000UL
  // 8MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 12) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.8584 * us
  //        Adjustment Takes: 72 Clocks (Approx)
  us = ( us * 1) + (us / 2) + (us / 4) + (us / 16) + (us / 32) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 10.2500us
  // by subtracting 20 Loop Cycles
  // us at this point is at least 22 Loops - 13us
  //       Subtraction Takes: 2 Clocks
  us -= 20;


#elif F_CPU >= 6400000UL
  // 6.4MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 4) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.5081 * us
  //        Adjustment Takes: 13 Clocks (Approx)
  us = ( us * 1) + (us / 2) + (us / 128) ;

  // Compensate for the combined overhead time of 3.5938us
  // by subtracting 5 Loop Cycles
  // us at this point is at least 7 Loops - 5us
  //       Subtraction Takes: 2 Clocks
  us -= 5;


#elif F_CPU >= 4800000UL
  // 4.8MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 15) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.1476 * us
  //        Adjustment Takes: 57 Clocks (Approx)
  us = ( us * 1) + (us / 8) + (us / 64) + (us / 256);

  // Compensate for the combined overhead time of 13.9583us
  // by subtracting 16 Loop Cycles
  // us at this point is at least 18 Loops - 16us
  //       Subtraction Takes: 2 Clocks
  us -= 16;


#elif F_CPU >= 4000000UL
  // 4MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 27) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.9633 * us
  //        Adjustment Takes: 86 Clocks (Approx)
  us = ( us * 0) + (us / 2) + (us / 4) + (us / 8) + (us / 16) + (us / 64) + (us / 128) ;

  // Compensate for the combined overhead time of 24.0000us
  // by subtracting 24 Loop Cycles
  // us at this point is at least 25 Loops - 28us
  //       Subtraction Takes: 2 Clocks
  us -= 24;


#elif F_CPU >= 2400000UL
  // 2.4MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 33) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.5866 * us
  //        Adjustment Takes: 63 Clocks (Approx)
  us = ( us * 0) + (us / 2) + (us / 16) + (us / 64) + (us / 128) ;

  // Compensate for the combined overhead time of 30.4167us
  // by subtracting 18 Loop Cycles
  // us at this point is at least 19 Loops - 34us
  //       Subtraction Takes: 2 Clocks
  us -= 18;


#elif F_CPU >= 2000000UL
  // 2MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 64) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.4907 * us
  //        Adjustment Takes: 108 Clocks (Approx)
  us = ( us * 0) + (us / 4) + (us / 8) + (us / 16) + (us / 32) + (us / 64) + (us / 256);

  // Compensate for the combined overhead time of 59.0000us
  // by subtracting 29 Loop Cycles
  // us at this point is at least 31 Loops - 65us
  //       Subtraction Takes: 2 Clocks
  us -= 29;


#elif F_CPU >= 1200000UL
  // 1.2MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 49) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.2890 * us
  //        Adjustment Takes: 37 Clocks (Approx)
  us = ( us * 0) + (us / 4) + (us / 32) + (us / 256);

  // Compensate for the combined overhead time of 40.6584us
  // by subtracting 11 Loop Cycles
  // us at this point is at least 13 Loops - 50us
  //       Subtraction Takes: 2 Clocks
  us -= 11;


#elif F_CPU >= 1000000UL
  // 1MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 127) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.2476 * us
  //        Adjustment Takes: 109 Clocks (Approx)
  us = ( us * 0) + (us / 8) + (us / 16) + (us / 32) + (us / 64) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 119.0000us
  // by subtracting 29 Loop Cycles
  // us at this point is at least 31 Loops - 128us
  //       Subtraction Takes: 2 Clocks
  us -= 29;


#elif F_CPU >= 800000UL
  // 800kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 87) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.1985 * us
  //        Adjustment Takes: 46 Clocks (Approx)
  us = ( us * 0) + (us / 8) + (us / 16) + (us / 128) ;

  // Compensate for the combined overhead time of 70.0000us
  // by subtracting 14 Loop Cycles
  // us at this point is at least 16 Loops - 88us
  //       Subtraction Takes: 2 Clocks
  us -= 14;


#elif F_CPU >= 600000UL
  // 600kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 127) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.1491 * us
  //        Adjustment Takes: 56 Clocks (Approx)
  us = ( us * 0) + (us / 8) + (us / 64) + (us / 128) ;

  // Compensate for the combined overhead time of 110.0000us
  // by subtracting 16 Loop Cycles
  // us at this point is at least 19 Loops - 128us
  //       Subtraction Takes: 2 Clocks
  us -= 16;


#elif F_CPU >= 128000UL
  // 128kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 322) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0320 * us
  //        Adjustment Takes: 27 Clocks (Approx)
  us = ( us * 0) + (us / 32) ;

  // Compensate for the combined overhead time of 289.0625us
  // by subtracting 9 Loop Cycles
  // us at this point is at least 10 Loops - 323us
  //       Subtraction Takes: 2 Clocks
  us -= 9;


#elif F_CPU >= 125000UL
  // 125kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 552) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0312 * us
  //        Adjustment Takes: 43 Clocks (Approx)
  us = ( us * 0) + (us / 64) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 424.0000us
  // by subtracting 13 Loop Cycles
  // us at this point is at least 14 Loops - 553us
  //       Subtraction Takes: 2 Clocks
  us -= 13;


#elif F_CPU >= 32768UL
  // 32.768kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 642) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0082 * us
  //        Adjustment Takes: 7 Clocks (Approx)
  us = ( us * 0) + (us / 128) ;

  // Compensate for the combined overhead time of 518.7988us
  // by subtracting 4 Loop Cycles
  // us at this point is at least 5 Loops - 643us
  //       Subtraction Takes: 2 Clocks
  us -= 4;


#elif F_CPU >= 16000UL
  // 16kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 1125) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0040 * us
  //        Adjustment Takes: 4 Clocks (Approx)
  us = ( us * 0) + (us / 256);

  // Compensate for the combined overhead time of 875.0000us
  // by subtracting 3 Loop Cycles
  // us at this point is at least 4 Loops - 1126us
  //       Subtraction Takes: 2 Clocks
  us -= 3;


#endif
#elif MILLIS_TIMER_PRESCALE == 64
#if F_CPU >= 24000000UL
  // 24MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 6) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 5.8065 * us
  //        Adjustment Takes: 131 Clocks (Approx)
  us = ( us * 5) + (us / 2) + (us / 4) + (us / 32) + (us / 64) + (us / 128) ;

  // Compensate for the combined overhead time of 5.9030us
  // by subtracting 35 Loop Cycles
  // us at this point is at least 39 Loops - 7us
  //       Subtraction Takes: 2 Clocks
  us -= 35;


#elif F_CPU >= 20000000UL
  // 20MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 3) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 4.8837 * us
  //        Adjustment Takes: 40 Clocks (Approx)
  us = ( us * 4) + (us / 2) + (us / 4) + (us / 8) + (us / 128) ;

  // Compensate for the combined overhead time of 2.5000us
  // by subtracting 12 Loop Cycles
  // us at this point is at least 19 Loops - 4us
  //       Subtraction Takes: 2 Clocks
  us -= 12;


#elif F_CPU >= 16000000UL
  // 16MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 9) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 3.9253 * us
  //        Adjustment Takes: 134 Clocks (Approx)
  us = ( us * 3) + (us / 2) + (us / 4) + (us / 8) + (us / 32) + (us / 64) ;

  // Compensate for the combined overhead time of 9.0000us
  // by subtracting 36 Loop Cycles
  // us at this point is at least 38 Loops - 10us
  //       Subtraction Takes: 2 Clocks
  us -= 36;


#elif F_CPU >= 12000000UL
  // 12MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 9) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 2.9578 * us
  //        Adjustment Takes: 89 Clocks (Approx)
  us = ( us * 2) + (us / 2) + (us / 4) + (us / 8) + (us / 16) + (us / 64) + (us / 256);

  // Compensate for the combined overhead time of 8.2500us
  // by subtracting 24 Loop Cycles
  // us at this point is at least 28 Loops - 10us
  //       Subtraction Takes: 2 Clocks
  us -= 24;


#elif F_CPU >= 9600000UL
  // 9.6MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 6) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 2.3886 * us
  //        Adjustment Takes: 40 Clocks (Approx)
  us = ( us * 2) + (us / 4) + (us / 8) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 5.2331us
  // by subtracting 13 Loop Cycles
  // us at this point is at least 15 Loops - 7us
  //       Subtraction Takes: 2 Clocks
  us -= 13;


#elif F_CPU >= 8000000UL
  // 8MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 13) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.9811 * us
  //        Adjustment Takes: 89 Clocks (Approx)
  us = ( us * 1) + (us / 2) + (us / 4) + (us / 8) + (us / 16) + (us / 32) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 12.3750us
  // by subtracting 24 Loop Cycles
  // us at this point is at least 25 Loops - 14us
  //       Subtraction Takes: 2 Clocks
  us -= 24;


#elif F_CPU >= 6400000UL
  // 6.4MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 13) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.5879 * us
  //        Adjustment Takes: 67 Clocks (Approx)
  us = ( us * 1) + (us / 2) + (us / 16) + (us / 64) + (us / 128) ;

  // Compensate for the combined overhead time of 12.0313us
  // by subtracting 19 Loop Cycles
  // us at this point is at least 21 Loops - 14us
  //       Subtraction Takes: 2 Clocks
  us -= 19;


#elif F_CPU >= 4800000UL
  // 4.8MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 14) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 1.1932 * us
  //        Adjustment Takes: 47 Clocks (Approx)
  us = ( us * 1) + (us / 8) + (us / 16) + (us / 256);

  // Compensate for the combined overhead time of 11.8750us
  // by subtracting 14 Loop Cycles
  // us at this point is at least 16 Loops - 15us
  //       Subtraction Takes: 2 Clocks
  us -= 14;


#elif F_CPU >= 4000000UL
  // 4MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 33) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.9953 * us
  //        Adjustment Takes: 113 Clocks (Approx)
  us = ( us * 0) + (us / 2) + (us / 4) + (us / 8) + (us / 16) + (us / 32) + (us / 64) + (us / 128) ;

  // Compensate for the combined overhead time of 30.7500us
  // by subtracting 30 Loop Cycles
  // us at this point is at least 32 Loops - 34us
  //       Subtraction Takes: 2 Clocks
  us -= 30;


#elif F_CPU >= 2400000UL
  // 2.4MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 31) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.5983 * us
  //        Adjustment Takes: 55 Clocks (Approx)
  us = ( us * 0) + (us / 2) + (us / 16) + (us / 32) + (us / 256);

  // Compensate for the combined overhead time of 27.0833us
  // by subtracting 16 Loop Cycles
  // us at this point is at least 19 Loops - 32us
  //       Subtraction Takes: 2 Clocks
  us -= 16;


#elif F_CPU >= 2000000UL
  // 2MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 68) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.4988 * us
  //        Adjustment Takes: 115 Clocks (Approx)
  us = ( us * 0) + (us / 4) + (us / 8) + (us / 16) + (us / 32) + (us / 64) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 62.5000us
  // by subtracting 31 Loop Cycles
  // us at this point is at least 32 Loops - 69us
  //       Subtraction Takes: 2 Clocks
  us -= 31;


#elif F_CPU >= 1200000UL
  // 1.2MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 70) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.2986 * us
  //        Adjustment Takes: 65 Clocks (Approx)
  us = ( us * 0) + (us / 4) + (us / 32) + (us / 64) ;

  // Compensate for the combined overhead time of 62.7975us
  // by subtracting 18 Loop Cycles
  // us at this point is at least 20 Loops - 71us
  //       Subtraction Takes: 2 Clocks
  us -= 18;


#elif F_CPU >= 1000000UL
  // 1MHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 127) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.2497 * us
  //        Adjustment Takes: 109 Clocks (Approx)
  us = ( us * 0) + (us / 8) + (us / 16) + (us / 32) + (us / 64) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 119.0000us
  // by subtracting 29 Loop Cycles
  // us at this point is at least 31 Loops - 128us
  //       Subtraction Takes: 2 Clocks
  us -= 29;


#elif F_CPU >= 800000UL
  // 800kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 87) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.1998 * us
  //        Adjustment Takes: 50 Clocks (Approx)
  us = ( us * 0) + (us / 8) + (us / 16) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 75.0000us
  // by subtracting 15 Loop Cycles
  // us at this point is at least 16 Loops - 88us
  //       Subtraction Takes: 2 Clocks
  us -= 15;


#elif F_CPU >= 600000UL
  // 600kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 127) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.1499 * us
  //        Adjustment Takes: 56 Clocks (Approx)
  us = ( us * 0) + (us / 8) + (us / 64) + (us / 128) ;

  // Compensate for the combined overhead time of 110.0000us
  // by subtracting 16 Loop Cycles
  // us at this point is at least 19 Loops - 128us
  //       Subtraction Takes: 2 Clocks
  us -= 16;


#elif F_CPU >= 128000UL
  // 128kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 322) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0320 * us
  //        Adjustment Takes: 27 Clocks (Approx)
  us = ( us * 0) + (us / 32) ;

  // Compensate for the combined overhead time of 289.0625us
  // by subtracting 9 Loop Cycles
  // us at this point is at least 10 Loops - 323us
  //       Subtraction Takes: 2 Clocks
  us -= 9;


#elif F_CPU >= 125000UL
  // 125kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 552) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0312 * us
  //        Adjustment Takes: 43 Clocks (Approx)
  us = ( us * 0) + (us / 64) + (us / 128) + (us / 256);

  // Compensate for the combined overhead time of 424.0000us
  // by subtracting 13 Loop Cycles
  // us at this point is at least 14 Loops - 553us
  //       Subtraction Takes: 2 Clocks
  us -= 13;


#elif F_CPU >= 32768UL
  // 32.768kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 642) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0082 * us
  //        Adjustment Takes: 7 Clocks (Approx)
  us = ( us * 0) + (us / 128) ;

  // Compensate for the combined overhead time of 518.7988us
  // by subtracting 4 Loop Cycles
  // us at this point is at least 5 Loops - 643us
  //       Subtraction Takes: 2 Clocks
  us -= 4;


#elif F_CPU >= 16000UL
  // 16kHz

  // Initialisation Overhead: 5 Clocks (Average)

  // Minimum us we can achieve
  //        Comparison Takes: 3 Clocks
  if(us <= 1125) return;

  // Convert us into a loop-counter for a busy-wait loop we will do.
  //   the following approximates 0.0040 * us
  //        Adjustment Takes: 4 Clocks (Approx)
  us = ( us * 0) + (us / 256);

  // Compensate for the combined overhead time of 875.0000us
  // by subtracting 3 Loop Cycles
  // us at this point is at least 4 Loops - 1126us
  //       Subtraction Takes: 2 Clocks
  us -= 3;


#endif
#endif
    // busy wait
  __asm__ __volatile__ (
    "1: sbiw %0,1" "\n\t" // 2 cycles
    "brne 1b" : "=w" (us) : "0" (us) // 2 cycles
  );
  // return = 4 cycles
}

#endif