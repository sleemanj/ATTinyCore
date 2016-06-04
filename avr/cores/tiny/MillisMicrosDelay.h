#include "Arduino.h"
#ifndef MillisMicrosDelay_h
#define MillisMicrosDelay_h
#if defined(USE_NEW_MILLIS) && USE_NEW_MILLIS
/**
  * millis(), micros(), delay(), delayMicroseconds(), and REAL_MILLIS(), REAL_MICROS()
  * 
  *  Authored by James Sleeman ( james@gogo.co.nz , http://sparks.gogo.co.nz/ ) 
  *  to replace the functions provided by wiring.c (wiring_lite.c) with the following
  *  goals...
  * 
  *  1. Handle more frequencies properly. 
  *  2. Have better accuracy vs real world for millis() and micros()
  *  3. Be able to configure the accuracy you want, versus using less code space.
  *  4. Be able to easily compensate for low accuracy when doing common types of operation.
  *  5. Simplify/standardize operations.
  *
  *  Defined In This File
  *  ----------------------------------------------------------------------
  *  millis() - the number of milliseconds since boot/overflow
  *  micros() - the number of microseconds since boot/overflow
  *  delay()  - delay for a number of milliseconds
  * 
  *  delayMicroseconds()                  - delay for a number of microseconds
  *       Note that, like the Arduino core itself, delayMicroseconds() does not
  *       disable interrupts during it's execution, nor does it attempt to 
  *       compensate for interrupts happening during it.  If you call this function
  *       with interrupts enabled (and without NO_MILLIS), the timing will be wrong.
  * 
  *  delayMicrosecondsAdjustedForMillisInterrupt() - delay for a number of microseconds
  *       This function produces a microseconds delay and compensates for
  *       the time taken in the millis interrupt which may fire one or more times
  *       during the delay period.  If you call this function with interrupts
  *       disabled, then the timing will be wrong. The compensation is by no means
  *       perfect but it's pretty close.
  * 
  *  REAL_MILLIS(x) - for x milliseconds in the real world, return the number of millis() that pass in the microcontroller
  *  REAL_MICROS(x) - for x microseconds in the real world, return the number of micros() that pass in the microcontroller
  * 
  *  ACCEPTABLE_MILLIS_ERROR_PPM - the error expressed in PPM which is acceptable in a return from millis()
  *                              - NB: The error will always be on the "slow" side, real world is ahead.
  * 
  *  ACCEPTABLE_MICROS_ERROR_PPM - the error expressed in PPM which is acceptable in a return from micros()
  *                              - NB: The error will always be on the "slow" side, real world is ahead.
  * 
  *  Usage of REAL_MICROS() and REAL_MILLIS()
  *  ----------------------------------------------------------------------
  *
  *  Quite often you will have some code that looks like this...
  *   
  *     unsigned long lastTime = millis();
  *     while(millis() - lastTime < 1000);
  *
  *  which in a perfect world would get you 1000ms of delay but the world 
  *  is not perfect and there are errors induced, even if you configure
  *  (as below) for a high accuracy, there is almost always some error
  *  due to rounding (of course this assumes a perfect clock signal too..)
  *
  *  To help with this two macros are available, for example...
  *
  *     unsigned long lastTime = millis();
  *     while(millis() - lastTime < REAL_MILLIS(1000));
  *
  *  (and similar for REAL_MICROS()) which will return the number of millis()
  *  which corresponds to that many millis() in the real world.  For example
  *  if for your frequency and accuracy configuration 990 millis() pass for each
  *  real world millisecond, then REAL_MILLIS(1000) would return 1010 so you will
  *  wait for 1010 millis() which is 1000 real world milliseconds.
  *
  *  Configuring Your Timing Accuracy vs Code Size Trade Off
  *  ----------------------------------------------------------------------
  *
  *  The accuracy of the millis() values can be increased/decreased 
  *  with more accuracy requiring more code (flash usage)
  *
  *  Accuracy is defined as an error level, you can set the acceptable 
  *  error for both millis() and micros(), expressed in parts per million 
  *  (because floats are not supported in preprocessor macros).
  *
  *  The functions will never give a result with error worse than 5% out
  *  (50000 ppm).
  *
  *  Anyway, long story short, set ACCEPTABLE_MILLIS_ERROR_PPM 
  *   and ACCEPTABLE_MICROS_ERROR_PPM to the maximum error you can tolerate
  *   expressed in parts per million from 0 to 1000000.
  *
  *    Example 1: You are OK with losing 1 second every hour
  *      1/3600 = 0.0002777' fraction
  *             * 100
  *             = 0.0278     percent
  *             * 10000
  *             = 278        ppm
  *      #define ACCEPTABLE_MILLIS_ERROR_PPM 278
  *
  *   Example 2: You are ok with losing 5 minutes a day
  *      5/(60 mins * 24 hours) = 0.00347 fraction
  *                             * 100
  *                             = 0.3472  percent
  *                             * 10000
  *                             = 3472    ppm
  *     #define ACCEPTABLE_MILLIS_ERROR_PPM 3472
  *
  *   Example 3: You are ok with losing 10 microseconds for every 1000 microseconds
  *      10/1000 = 0.01   fraction
  *              * 100  
  *              = 1      percent
  *              * 10000
  *              = 10000  ppm
  *    #define ACCEPTABLE_MICROS_ERROR_PPM 10000
  *
  *  Aside: Of course youc an go straight from your fraction to ppm by multiplying by 
  *  a million, but most people just instinctivly understand percent rather than ppm.
  *  Blame the C preprocessor for your mental gymnastics because if it supported
  *  floats in it's expressions I'd have used percent!
  *
  *  Generally speaking a lower acceptable error will require more flash memory
  *  due to extra calculations required.
  *  
  *  Not all frequencies will show a differenceat all however
  *    for example 16kHz and 128kHz are exact and do not exhibit any error.
  *
  *  Finally note of course that this error is under the assumption that you have
  *  an ideal clock source for your AVR... which if you are using the internal
  *  oscillator is very far from the truth.  If your clock wanders, your accuracy
  *  wanders with it.  The internal oscillator wanders.
*/

typedef uint32_t MillisMicrosTime_t;
typedef uint16_t DelayTime_t             ;
typedef uint16_t DelayMicrosecondsTime_t;

// NB DelayMicrosecondsTime_t can't be less than uint16_t.  
//    Firstly because millis() will use delayMicroseconds(1000) if NO_MILLIS
//    is defined.
//
//    Secondly because it just doesn't give enough realistic resolution.
//
// Example:
//    9.6MHz, consult the delayMicroseconds() code and you will find the multiplier  
//    of 2.4 loops per us, at 16bit, 2.4 * 27306us = 65535 loop, 2.4 * 106 = 254 loop
//    so an 8 bit DelayMicrosecondsTime_t (which is also used as the loop counter)
//    would allow only allow 106uS maximum input
//
//   For frequencies <= 4 MHz we get to use the full width of DelayMicrosecondsTime_t
//   as the us input, so
//   2.4Mhz, 16bit: 65535 us maximum delayMicroseconds(), 8 bit: 255us 
//
//   For frequencies < 260kHz however the overhead in delayMicroseconds() is more than
//   255us itself, and if you try to request less than the overhead, you get nothing.
//
//  So realistically there is the limited band 260kHz <= x <= 4Mhz where you might want
//  to use uint8_t here, if you are OK with only being able to delayMicroseconds(255) 
//  at a maximum and you have millis() available for delay() to use (or don't use delay()).

void delay(DelayTime_t);
void delayMicrosecondsWithoutMillisInterruptAdjustment(DelayMicrosecondsTime_t);
void delayMicrosecondsAdjustedForMillisInterrupt(DelayMicrosecondsTime_t);

#ifdef NO_MILLIS
  // If the millis interrupt handler isn't being used at all
  // then we do not need to adjust for it in delayMicroseconds
  #define delayMicroseconds delayMicrosecondsWithoutMillisInterruptAdjustment  
#else
  // Conversely if we do have millis() running then we are probably
  // best to use our adjusted one instead of the normal one
  #define delayMicroseconds delayMicrosecondsAdjustedForMillisInterrupt  
#endif

#ifndef NO_MILLIS
MillisMicrosTime_t millis();
MillisMicrosTime_t micros();
  
#ifndef ACCEPTABLE_MILLIS_ERROR_PPM
  #ifdef ACCEPTABLE_MICROS_ERROR_PPM
    #define ACCEPTABLE_MILLIS_ERROR_PPM ACCEPTABLE_MICROS_ERROR_PPM 
  #else
  //#define ACCEPTABLE_MILLIS_ERROR_PPM 1
  #define ACCEPTABLE_MILLIS_ERROR_PPM 1000000
  #endif
#endif

#ifndef ACCEPTABLE_MICROS_ERROR_PPM
  #define ACCEPTABLE_MICROS_ERROR_PPM ACCEPTABLE_MILLIS_ERROR_PPM
#endif
  
// MILLIS_TIMER_OVERFLOW_VECTOR is the name of the vector which will be used
//  vector names can be found in the avr includes, eg 
// iotn13a.h:#define TIM0_OVF_vect      _VECTOR(3)  /* Timer/Counter0 Overflow */
//
// This should be set by pins_arduino.h
#ifndef MILLIS_TIMER_OVERFLOW_VECTOR
  #define MILLIS_TIMER_OVERFLOW_VECTOR TIM0_OVF_vect
#endif

// The prescaler which is being used on the overflow vector, you CAN set this in
// pins_arduino.h if your specific chip can't handle 1/8/64 prescaling, otherwise
// just leave it as the default's below which should work.
//
// The prescale is passed into turnOnMillis() for you to use when setting 
// up your timer.
#ifndef MILLIS_TIMER_PRESCALE
  #if   F_CPU >  16000000UL
    // Down to 16MHz /64 gets at worst ~1ms per tick
    #define MILLIS_TIMER_PRESCALE 64  
  #elif F_CPU >   2000000UL
    // Down to 2MHz /8 Prescale gets at worst ~1ms per tick
    // Best = 128us at 16MHz    
    #define MILLIS_TIMER_PRESCALE 8
  #else
    // 2Mhz or below we really need to run without prescaling
    // Best  @ 2Mhz, 128us per tick
    // Worst @ 16kHz, 16000us (16ms) per tick
    #define MILLIS_TIMER_PRESCALE 1
  #endif
#endif    

  // REAL_MILLIS(x)
#if (F_CPU / MILLIS_TIMER_PRESCALE) == 16000UL
  // 16 kHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  // The below define is equivalent to AVR_MICROS * (1-0)
  // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) (AVR_MILLIS)


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 32768UL
  // 32.768 kHz
  //     Best Error Possible: 0.0000%  (0 Decimal)
  //    Worst Error Possible: 10.4000% (0.104 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 104000UL
    //  Error: 10.4000% (0.104 Decimal)
    // Jitter: 0.0000% (0 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.104)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (896000ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 40000UL
    //  Error: 4.0000% (0.04 Decimal)
    // Jitter: 0.0000% (0 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.04)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (960000ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8000UL
    //  Error: 0.8000% (0.008 Decimal)
    // Jitter: 0.0000% (0 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.008)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (992000ULL)) / 1000000UL ))

  #else
    //  Error: 0.0000% (0 Decimal)
    // Jitter: 0.0000% (0 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) (AVR_MILLIS)
  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 125000UL
  // 125 kHz
  //     Best Error Possible: 0.0054%  (0.000054086100000000004 Decimal)
  //    Worst Error Possible: 2.3438% (0.0234377102 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 23438UL
    //  Error: 2.3438% (0.0234377102 Decimal)
    // Jitter: 0.0053% (0.0000532543 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.023438)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (976562ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8189UL
    //  Error: 0.8189% (0.008188850000000001 Decimal)
    // Jitter: 0.0105% (0.00010538 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.008189)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (991811ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 570UL
    //  Error: 0.0570% (0.0005696364 Decimal)
    // Jitter: 0.0185% (0.0001854254 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.00057)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999430ULL)) / 1000000UL ))

  #else
    //  Error: 0.0054% (0.000054086100000000004 Decimal)
    // Jitter: 0.0258% (0.0002581209 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000054)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999946ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 128000UL
  // 128 kHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  // The below define is equivalent to AVR_MICROS * (1-0)
  // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) (AVR_MILLIS)


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 600000UL
  // 600 kHz
  //     Best Error Possible: 0.0091%  (0.0000913535 Decimal)
  //    Worst Error Possible: 41.4068% (0.4140676903 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 414067UL
    //  Error: 41.4068% (0.4140676903 Decimal)
    // Jitter: 0.0039% (0.0000390023 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.414068)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (585932ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 121112UL
    //  Error: 12.1112% (0.1211119719 Decimal)
    // Jitter: 0.0103% (0.0001027377 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.121112)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (878888ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 47881UL
    //  Error: 4.7881% (0.0478809068 Decimal)
    // Jitter: 0.0144% (0.00014398660000000001 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.047881)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (952119ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 11270UL
    //  Error: 1.1270% (0.011270414199999999 Decimal)
    // Jitter: 0.0218% (0.000217923 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.01127)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (988730ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 2126UL
    //  Error: 0.2126% (0.0021257019 Decimal)
    // Jitter: 0.0281% (0.0002812586 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.002126)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (997874ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 992UL
    //  Error: 0.0992% (0.0009915898999999998 Decimal)
    // Jitter: 0.0329% (0.0003288179 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000992)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999008ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 429UL
    //  Error: 0.0429% (0.00042937520000000003 Decimal)
    // Jitter: 0.0374% (0.0003740969 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000429)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999571ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 153UL
    //  Error: 0.0153% (0.00015276230000000002 Decimal)
    // Jitter: 0.0410% (0.0004097833 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000153)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999847ULL)) / 1000000UL ))

  #else
    //  Error: 0.0091% (0.0000913535 Decimal)
    // Jitter: 0.0480% (0.0004795937 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000091)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999909ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 800000UL
  // 800 kHz
  //     Best Error Possible: 0.0115%  (0.0001145359 Decimal)
  //    Worst Error Possible: 21.8758% (0.21875796979999998 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 218758UL
    //  Error: 21.8758% (0.21875796979999998 Decimal)
    // Jitter: 0.0068% (0.00006818179999999999 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.218758)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (781242ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 23455UL
    //  Error: 2.3455% (0.023454928400000002 Decimal)
    // Jitter: 0.0108% (0.0001079545 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.023455)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (976545ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 11258UL
    //  Error: 1.1258% (0.0112578935 Decimal)
    // Jitter: 0.0136% (0.0001362782 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.011258)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (988742ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 5164UL
    //  Error: 0.5164% (0.0051639588 Decimal)
    // Jitter: 0.0186% (0.00018647909999999998 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.005164)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (994836ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 2123UL
    //  Error: 0.2123% (0.0021232221 Decimal)
    // Jitter: 0.0238% (0.0002382033 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.002123)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (997877ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 608UL
    //  Error: 0.0608% (0.0006080995 Decimal)
    // Jitter: 0.0290% (0.0002903811 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000608)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999392ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 237UL
    //  Error: 0.0237% (0.0002371862 Decimal)
    // Jitter: 0.0301% (0.0003012704 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000237)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999763ULL)) / 1000000UL ))

  #else
    //  Error: 0.0115% (0.0001145359 Decimal)
    // Jitter: 0.0387% (0.0003865699 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000115)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999885ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 1000000UL
  // 1 MHz
  //     Best Error Possible: 0.0066%  (0.0000658686 Decimal)
  //    Worst Error Possible: 2.3448% (0.0234481704 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 23448UL
    //  Error: 2.3448% (0.0234481704 Decimal)
    // Jitter: 0.0070% (0.0000703129 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.023448)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (976552ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8200UL
    //  Error: 0.8200% (0.008199986800000001 Decimal)
    // Jitter: 0.0121% (0.0001212963 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.0082)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (991800ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 581UL
    //  Error: 0.0581% (0.0005812023000000001 Decimal)
    // Jitter: 0.0175% (0.0001750009 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000581)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999419ULL)) / 1000000UL ))

  #else
    //  Error: 0.0066% (0.0000658686 Decimal)
    // Jitter: 0.0281% (0.0002809513 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000066)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999934ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 1200000UL
  // 1.2 MHz
  //     Best Error Possible: 0.0096%  (0.0000955207 Decimal)
  //    Worst Error Possible: 41.4070% (0.4140701323 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 414069UL
    //  Error: 41.4070% (0.4140701323 Decimal)
    // Jitter: 0.0042% (0.0000424882 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.41407)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (585930ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 121115UL
    //  Error: 12.1116% (0.1211156348 Decimal)
    // Jitter: 0.0101% (0.00010128149999999999 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.121116)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (878884ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 47885UL
    //  Error: 4.7885% (0.047884874900000006 Decimal)
    // Jitter: 0.0141% (0.0001412517 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.047885)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (952115ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 11275UL
    //  Error: 1.1275% (0.0112745348 Decimal)
    // Jitter: 0.0216% (0.00021628579999999998 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.011275)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (988725ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 2130UL
    //  Error: 0.2130% (0.0021298607 Decimal)
    // Jitter: 0.0278% (0.0002783623 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.00213)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (997870ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 995UL
    //  Error: 0.0996% (0.0009957534 Decimal)
    // Jitter: 0.0329% (0.0003288179 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000996)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999004ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 434UL
    //  Error: 0.0434% (0.000433541 Decimal)
    // Jitter: 0.0374% (0.0003740969 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000434)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999566ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 157UL
    //  Error: 0.0157% (0.0001569292 Decimal)
    // Jitter: 0.0410% (0.0004097833 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000157)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999843ULL)) / 1000000UL ))

  #else
    //  Error: 0.0096% (0.0000955207 Decimal)
    // Jitter: 0.0480% (0.0004795937 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000096)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999904ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 2000000UL
  // 2 MHz
  //     Best Error Possible: 0.0113%  (0.0001126104 Decimal)
  //    Worst Error Possible: 2.3446% (0.0234464846 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 23445UL
    //  Error: 2.3446% (0.0234464846 Decimal)
    // Jitter: 0.0088% (0.00008823529999999999 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.023446)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (976554ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8198UL
    //  Error: 0.8198% (0.008198212299999999 Decimal)
    // Jitter: 0.0124% (0.0001235294 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.008198)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (991802ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 579UL
    //  Error: 0.0579% (0.0005791708 Decimal)
    // Jitter: 0.0179% (0.00017891220000000002 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000579)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999421ULL)) / 1000000UL ))

  #else
    //  Error: 0.0113% (0.0001126104 Decimal)
    // Jitter: 0.0224% (0.0002238139 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000113)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999887ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 2400000UL
  // 2.4 MHz
  //     Best Error Possible: 0.0153%  (0.00015272219999999998 Decimal)
  //    Worst Error Possible: 41.4072% (0.4140718057 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 414072UL
    //  Error: 41.4072% (0.4140718057 Decimal)
    // Jitter: 0.0055% (0.0000551471 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.414072)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (585928ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 121113UL
    //  Error: 12.1113% (0.1211131569 Decimal)
    // Jitter: 0.0106% (0.00010625 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.121113)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (878887ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 47881UL
    //  Error: 4.7881% (0.0478814326 Decimal)
    // Jitter: 0.0148% (0.0001484375 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.047881)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (952119ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 11271UL
    //  Error: 1.1271% (0.011270545 Decimal)
    // Jitter: 0.0223% (0.0002229665 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.011271)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (988729ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 2126UL
    //  Error: 0.2126% (0.0021257332 Decimal)
    // Jitter: 0.0283% (0.0002829912 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.002126)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (997874ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 992UL
    //  Error: 0.0992% (0.0009916172 Decimal)
    // Jitter: 0.0329% (0.0003291789 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000992)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999008ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 429UL
    //  Error: 0.0429% (0.0004293574 Decimal)
    // Jitter: 0.0377% (0.0003769841 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000429)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999571ULL)) / 1000000UL ))

  #else
    //  Error: 0.0153% (0.00015272219999999998 Decimal)
    // Jitter: 0.0413% (0.0004126984 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000153)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999847ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 4000000UL
  // 4 MHz
  //     Best Error Possible: 0.0114%  (0.0001138951 Decimal)
  //    Worst Error Possible: 2.3447% (0.0234473254 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 23447UL
    //  Error: 2.3447% (0.0234473254 Decimal)
    // Jitter: 0.0062% (0.0000625 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.023447)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (976553ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8199UL
    //  Error: 0.8199% (0.008199243200000001 Decimal)
    // Jitter: 0.0120% (0.0001203125 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.008199)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (991801ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 580UL
    //  Error: 0.0580% (0.0005804791 Decimal)
    // Jitter: 0.0177% (0.00017671090000000002 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.00058)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999420ULL)) / 1000000UL ))

  #else
    //  Error: 0.0114% (0.0001138951 Decimal)
    // Jitter: 0.0224% (0.0002238095 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000114)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999886ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 4800000UL
  // 4.8 MHz
  //     Best Error Possible: 0.0153%  (0.00015272219999999998 Decimal)
  //    Worst Error Possible: 41.4072% (0.4140718057 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 414072UL
    //  Error: 41.4072% (0.4140718057 Decimal)
    // Jitter: 0.0055% (0.0000551471 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.414072)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (585928ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 121113UL
    //  Error: 12.1113% (0.1211131569 Decimal)
    // Jitter: 0.0106% (0.00010625 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.121113)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (878887ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 47881UL
    //  Error: 4.7881% (0.0478814326 Decimal)
    // Jitter: 0.0148% (0.0001484375 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.047881)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (952119ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 11271UL
    //  Error: 1.1271% (0.011270545 Decimal)
    // Jitter: 0.0223% (0.0002229665 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.011271)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (988729ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 2126UL
    //  Error: 0.2126% (0.0021257332 Decimal)
    // Jitter: 0.0283% (0.0002829912 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.002126)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (997874ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 992UL
    //  Error: 0.0992% (0.0009916172 Decimal)
    // Jitter: 0.0329% (0.0003291789 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000992)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999008ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 429UL
    //  Error: 0.0429% (0.0004293574 Decimal)
    // Jitter: 0.0377% (0.0003769841 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000429)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999571ULL)) / 1000000UL ))

  #else
    //  Error: 0.0153% (0.00015272219999999998 Decimal)
    // Jitter: 0.0413% (0.0004126984 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000153)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999847ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 6400000UL
  // 6.4 MHz
  //     Best Error Possible: 0.0237%  (0.0002371862 Decimal)
  //    Worst Error Possible: 21.8758% (0.21875796979999998 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 218758UL
    //  Error: 21.8758% (0.21875796979999998 Decimal)
    // Jitter: 0.0068% (0.00006818179999999999 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.218758)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (781242ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 23455UL
    //  Error: 2.3455% (0.023454928400000002 Decimal)
    // Jitter: 0.0108% (0.0001079545 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.023455)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (976545ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 11258UL
    //  Error: 1.1258% (0.0112578935 Decimal)
    // Jitter: 0.0136% (0.0001362782 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.011258)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (988742ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 5164UL
    //  Error: 0.5164% (0.0051639588 Decimal)
    // Jitter: 0.0186% (0.00018647909999999998 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.005164)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (994836ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 2123UL
    //  Error: 0.2123% (0.0021232221 Decimal)
    // Jitter: 0.0238% (0.0002382033 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.002123)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (997877ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 608UL
    //  Error: 0.0608% (0.0006080995 Decimal)
    // Jitter: 0.0290% (0.0002903811 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000608)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999392ULL)) / 1000000UL ))

  #else
    //  Error: 0.0237% (0.0002371862 Decimal)
    // Jitter: 0.0301% (0.0003012704 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000237)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999763ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 8000000UL
  // 8 MHz
  //     Best Error Possible: 0.0114%  (0.0001138951 Decimal)
  //    Worst Error Possible: 2.3447% (0.0234473254 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 23447UL
    //  Error: 2.3447% (0.0234473254 Decimal)
    // Jitter: 0.0062% (0.0000625 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.023447)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (976553ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8199UL
    //  Error: 0.8199% (0.008199243200000001 Decimal)
    // Jitter: 0.0120% (0.0001203125 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.008199)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (991801ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 580UL
    //  Error: 0.0580% (0.0005804791 Decimal)
    // Jitter: 0.0177% (0.00017671090000000002 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.00058)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999420ULL)) / 1000000UL ))

  #else
    //  Error: 0.0114% (0.0001138951 Decimal)
    // Jitter: 0.0224% (0.0002238095 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000114)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999886ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 9600000UL
  // 9.6 MHz
  //     Best Error Possible: 0.0429%  (0.0004293574 Decimal)
  //    Worst Error Possible: 41.4072% (0.4140718057 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 414072UL
    //  Error: 41.4072% (0.4140718057 Decimal)
    // Jitter: 0.0055% (0.0000551471 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.414072)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (585928ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 121113UL
    //  Error: 12.1113% (0.1211131569 Decimal)
    // Jitter: 0.0106% (0.00010625 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.121113)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (878887ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 47881UL
    //  Error: 4.7881% (0.0478814326 Decimal)
    // Jitter: 0.0148% (0.0001484375 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.047881)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (952119ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 11271UL
    //  Error: 1.1271% (0.011270545 Decimal)
    // Jitter: 0.0223% (0.0002229665 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.011271)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (988729ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 2126UL
    //  Error: 0.2126% (0.0021257332 Decimal)
    // Jitter: 0.0283% (0.0002829912 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.002126)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (997874ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 992UL
    //  Error: 0.0992% (0.0009916172 Decimal)
    // Jitter: 0.0329% (0.0003291789 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000992)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999008ULL)) / 1000000UL ))

  #else
    //  Error: 0.0429% (0.0004293574 Decimal)
    // Jitter: 0.0377% (0.0003769841 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000429)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999571ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 12000000UL
  // 12 MHz
  //     Best Error Possible: 0.0146%  (0.0001455417 Decimal)
  //    Worst Error Possible: 26.7588% (0.267588149 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 267588UL
    //  Error: 26.7588% (0.267588149 Decimal)
    // Jitter: 0.0065% (0.0000647321 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.267588)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (732412ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 84492UL
    //  Error: 8.4492% (0.0844921373 Decimal)
    // Jitter: 0.0098% (0.0000977444 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.084492)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (915508ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 38726UL
    //  Error: 3.8726% (0.038726386 Decimal)
    // Jitter: 0.0160% (0.0001597744 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.038726)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (961274ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 15849UL
    //  Error: 1.5849% (0.015849382 Decimal)
    // Jitter: 0.0188% (0.0001879699 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.015849)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (984151ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 4417UL
    //  Error: 0.4417% (0.0044165132000000005 Decimal)
    // Jitter: 0.0268% (0.0002679426 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.004417)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (995583ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 1566UL
    //  Error: 0.1566% (0.0015659282000000001 Decimal)
    // Jitter: 0.0307% (0.00030690359999999996 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.001566)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (998434ULL)) / 1000000UL ))

  #else
    //  Error: 0.0146% (0.0001455417 Decimal)
    // Jitter: 0.0372% (0.0003718387 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000146)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999854ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 16000000UL
  // 16 MHz
  //     Best Error Possible: 0.0580%  (0.0005804791 Decimal)
  //    Worst Error Possible: 2.3447% (0.0234473254 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 23447UL
    //  Error: 2.3447% (0.0234473254 Decimal)
    // Jitter: 0.0062% (0.0000625 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.023447)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (976553ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8199UL
    //  Error: 0.8199% (0.008199243200000001 Decimal)
    // Jitter: 0.0120% (0.0001203125 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.008199)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (991801ULL)) / 1000000UL ))

  #else
    //  Error: 0.0580% (0.0005804791 Decimal)
    // Jitter: 0.0177% (0.00017671090000000002 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.00058)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (999420ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 20000000UL
  // 20 MHz
  //     Best Error Possible: 0.1080%  (0.0010804776999999998 Decimal)
  //    Worst Error Possible: 38.9659% (0.38965908730000004 Decimal)

  #if      ACCEPTABLE_MILLIS_ERROR_PPM >= 389659UL
    //  Error: 38.9659% (0.38965908730000004 Decimal)
    // Jitter: 0.0079% (0.0000788352 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.389659)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (610341ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 84494UL
    //  Error: 8.4494% (0.0844941297 Decimal)
    // Jitter: 0.0163% (0.00016339070000000001 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.084494)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (915506ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 8211UL
    //  Error: 0.8211% (0.00821125 Decimal)
    // Jitter: 0.0218% (0.0002175182 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.008211)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (991789ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MILLIS_ERROR_PPM >= 3454UL
    //  Error: 0.3454% (0.0034536073999999997 Decimal)
    // Jitter: 0.0284% (0.0002839416 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.003454)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (996546ULL)) / 1000000UL ))

  #else
    //  Error: 0.1080% (0.0010804776999999998 Decimal)
    // Jitter: 0.0367% (0.0003666667 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.00108)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MILLIS(AVR_MILLIS) ((uint32_t) ((((uint32_t)(AVR_MILLIS)) * (998920ULL)) / 1000000UL ))

  #endif
#endif
  
  // REAL_MICROS(x)
#if (F_CPU / MILLIS_TIMER_PRESCALE) == 16000UL
  // 16 kHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  // The below define is equivalent to AVR_MICROS * (1-0)
  // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) (AVR_MICROS)


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 32768UL
  // 32.768 kHz
  //  Error: 0.0127% (0.0001274919 Decimal)
  // Jitter: 0.0064% (0.00006399179999999999 Decimal)

  // The below define is equivalent to AVR_MICROS * (1-0.000127)
  // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999873ULL)) / 1000000UL ))



#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 125000UL
  // 125 kHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  // The below define is equivalent to AVR_MICROS * (1-0)
  // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) (AVR_MICROS)


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 128000UL
  // 128 kHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  // The below define is equivalent to AVR_MICROS * (1-0)
  // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) (AVR_MICROS)


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 600000UL
  // 600 kHz
  //     Best Error Possible: 0.0037%  (0.000036858 Decimal)
  //    Worst Error Possible: 0.1562% (0.0015624011 Decimal)

  #if      ACCEPTABLE_MICROS_ERROR_PPM >= 1562UL
    //  Error: 0.1562% (0.0015624011 Decimal)
    // Jitter: 0.0058% (0.000057859400000000004 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.001562)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (998438ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 391UL
    //  Error: 0.0391% (0.0003910875 Decimal)
    // Jitter: 0.0102% (0.0001019122 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000391)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999609ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 107UL
    //  Error: 0.0107% (0.00010718529999999999 Decimal)
    // Jitter: 0.0189% (0.000189256 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000107)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999893ULL)) / 1000000UL ))

  #else
    //  Error: 0.0037% (0.000036858 Decimal)
    // Jitter: 0.0263% (0.0002625 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000037)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999963ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 800000UL
  // 800 kHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  // The below define is equivalent to AVR_MICROS * (1-0)
  // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) (AVR_MICROS)


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 1000000UL
  // 1 MHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  // The below define is equivalent to AVR_MICROS * (1-0)
  // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) (AVR_MICROS)


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 1200000UL
  // 1.2 MHz
  //     Best Error Possible: 0.0042%  (0.0000423357 Decimal)
  //    Worst Error Possible: 0.1562% (0.0015623306 Decimal)

  #if      ACCEPTABLE_MICROS_ERROR_PPM >= 1562UL
    //  Error: 0.1562% (0.0015623306 Decimal)
    // Jitter: 0.0055% (0.0000551636 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.001562)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (998438ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 398UL
    //  Error: 0.0398% (0.0003979971 Decimal)
    // Jitter: 0.0102% (0.0001019122 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000398)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999602ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 115UL
    //  Error: 0.0115% (0.0001147185 Decimal)
    // Jitter: 0.0179% (0.0001787935 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000115)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999885ULL)) / 1000000UL ))

  #else
    //  Error: 0.0042% (0.0000423357 Decimal)
    // Jitter: 0.0255% (0.0002545819 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000042)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999958ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 2000000UL
  // 2 MHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  // The below define is equivalent to AVR_MICROS * (1-0)
  // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) (AVR_MICROS)


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 2400000UL
  // 2.4 MHz
  //     Best Error Possible: 0.0059%  (0.0000591303 Decimal)
  //    Worst Error Possible: 0.6257% (0.00625683 Decimal)

  #if      ACCEPTABLE_MICROS_ERROR_PPM >= 6257UL
    //  Error: 0.6257% (0.00625683 Decimal)
    // Jitter: 0.0030% (0.0000301493 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.006257)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (993743ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 1575UL
    //  Error: 0.1575% (0.0015753006 Decimal)
    // Jitter: 0.0085% (0.0000849592 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.001575)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (998425ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 413UL
    //  Error: 0.0413% (0.0004128806 Decimal)
    // Jitter: 0.0157% (0.0001567544 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000413)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999587ULL)) / 1000000UL ))

  #else
    //  Error: 0.0059% (0.0000591303 Decimal)
    // Jitter: 0.0334% (0.00033427019999999997 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000059)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999941ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 4000000UL
  // 4 MHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  // The below define is equivalent to AVR_MICROS * (1-0)
  // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) (AVR_MICROS)


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 4800000UL
  // 4.8 MHz
  //     Best Error Possible: 0.0054%  (0.0000543994 Decimal)
  //    Worst Error Possible: 0.6250% (0.006250170100000001 Decimal)

  #if      ACCEPTABLE_MICROS_ERROR_PPM >= 6250UL
    //  Error: 0.6250% (0.006250170100000001 Decimal)
    // Jitter: 0.0056% (0.0000556046 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.00625)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (993750ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 1571UL
    //  Error: 0.1571% (0.0015707754 Decimal)
    // Jitter: 0.0113% (0.0001130754 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.001571)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (998429ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 409UL
    //  Error: 0.0409% (0.00040873 Decimal)
    // Jitter: 0.0166% (0.00016610730000000002 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000409)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999591ULL)) / 1000000UL ))

  #else
    //  Error: 0.0054% (0.0000543994 Decimal)
    // Jitter: 0.0341% (0.0003409332 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000054)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999946ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 6400000UL
  // 6.4 MHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  // The below define is equivalent to AVR_MICROS * (1-0)
  // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) (AVR_MICROS)


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 8000000UL
  // 8 MHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  // The below define is equivalent to AVR_MICROS * (1-0)
  // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) (AVR_MICROS)


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 9600000UL
  // 9.6 MHz
  //     Best Error Possible: 0.0068%  (0.0000677557 Decimal)
  //    Worst Error Possible: 2.5000% (0.0249999037 Decimal)

  #if      ACCEPTABLE_MICROS_ERROR_PPM >= 25000UL
    //  Error: 2.5000% (0.0249999037 Decimal)
    // Jitter: 0.0057% (0.000056533099999999996 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.025)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (975000ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 6260UL
    //  Error: 0.6260% (0.0062602516 Decimal)
    // Jitter: 0.0068% (0.00006789860000000001 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.00626)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (993740ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 1583UL
    //  Error: 0.1583% (0.0015833386 Decimal)
    // Jitter: 0.0135% (0.0001353933 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.001583)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (998417ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 422UL
    //  Error: 0.0422% (0.0004219071 Decimal)
    // Jitter: 0.0207% (0.0002065505 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000422)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999578ULL)) / 1000000UL ))

  #else
    //  Error: 0.0068% (0.0000677557 Decimal)
    // Jitter: 0.0384% (0.0003841938 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000068)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999932ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 12000000UL
  // 12 MHz
  //     Best Error Possible: 0.0060%  (0.000059592599999999996 Decimal)
  //    Worst Error Possible: 1.5625% (0.0156250709 Decimal)

  #if      ACCEPTABLE_MICROS_ERROR_PPM >= 15625UL
    //  Error: 1.5625% (0.0156250709 Decimal)
    // Jitter: 0.0053% (0.0000525339 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.015625)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (984375ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 3914UL
    //  Error: 0.3914% (0.0039139441 Decimal)
    // Jitter: 0.0108% (0.0001075354 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.003914)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (996086ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 994UL
    //  Error: 0.0994% (0.0009935546 Decimal)
    // Jitter: 0.0116% (0.0001164998 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000994)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999006ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 271UL
    //  Error: 0.0271% (0.0002709633 Decimal)
    // Jitter: 0.0176% (0.0001763528 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000271)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999729ULL)) / 1000000UL ))

  #else
    //  Error: 0.0060% (0.000059592599999999996 Decimal)
    // Jitter: 0.0241% (0.00024061930000000002 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.00006)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999940ULL)) / 1000000UL ))

  #endif


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 16000000UL
  // 16 MHz
  //  Error: 0.0000% (0 Decimal)
  // Jitter: 0.0000% (0 Decimal)

  // The below define is equivalent to AVR_MICROS * (1-0)
  // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) (AVR_MICROS)


#elif (F_CPU / MILLIS_TIMER_PRESCALE) == 20000000UL
  // 20 MHz
  //     Best Error Possible: 0.0077%  (0.0000766896 Decimal)
  //    Worst Error Possible: 6.2500% (0.0625000096 Decimal)

  #if      ACCEPTABLE_MICROS_ERROR_PPM >= 62500UL
    //  Error: 6.2500% (0.0625000096 Decimal)
    // Jitter: 0.0058% (0.000058070500000000006 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.0625)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (937500ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 23448UL
    //  Error: 2.3448% (0.0234480564 Decimal)
    // Jitter: 0.0073% (0.0000734384 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.023448)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (976552ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 3927UL
    //  Error: 0.3927% (0.0039272088 Decimal)
    // Jitter: 0.0094% (0.00009441930000000001 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.003927)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (996073ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 1496UL
    //  Error: 0.1496% (0.0014961941999999998 Decimal)
    // Jitter: 0.0167% (0.0001671609 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.001496)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (998504ULL)) / 1000000UL ))

  #elif    ACCEPTABLE_MICROS_ERROR_PPM >= 286UL
    //  Error: 0.0286% (0.0002857473 Decimal)
    // Jitter: 0.0211% (0.0002114706 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000286)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999714ULL)) / 1000000UL ))

  #else
    //  Error: 0.0077% (0.0000766896 Decimal)
    // Jitter: 0.0368% (0.00036802030000000004 Decimal)

    // The below define is equivalent to AVR_MICROS * (1-0.000077)
    // (integer math in case the optimizer won't evaluate floats at compile time).
  #define REAL_MICROS(AVR_MICROS) ((uint32_t) ((((uint32_t)(AVR_MICROS)) * (999923ULL)) / 1000000UL ))

  #endif
#endif

#endif

#endif
#endif