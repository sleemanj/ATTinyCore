Special Considerations for the ATTiny 4/5/9/10 in Arduino ATTinyCore
-------------------------------------------------------------------------------

Older versions of the Arduino IDE did not have the necessary tools to support these chips.  

Starting with about 1.6.13 this should be OK now, but if you insist on using an old version of the IDE then you will find some help at the bottom of this page.

## Uploading

The 4/5/9/10 do not support uploading by serial, nor by SPI.  What they support is called "TPI".

The firmware usually found on cheap chinese versions of the USBasp does not support TPI so you would have to re-flash one of those (see below for tips) with either..

  * The current version of the ["official" USBAsp firmware](http://www.fischl.de/usbasp/) supports uploading via TPI, connect MOSI to TPIDATA (pin 1) and SCK to TPICLK (pin 2).
  * The [1.06-alpha firmware](https://github.com/bperrybap/usbasp/tree/1.06-alpha/bin/firmware/usbasp-v1.06-alpha-2016-05-18) by @bperrybap and @PeterVH supports uploading via TPI and also auto clocking, connect MOSI to TPIDATA (pin 1) and SCK to TPICLK (pin 2).  This one does not support TPI on ATMega48 based USBAsp, the official one may.

There is an [ArduinoISP type TPI uploader](http://junkplusarduino.blogspot.co.nz/p/attiny10-resources.html) but I have not tried it personally.

## Verification Error - Check Thy Volts

The 4/5/9/10 can **not** be programmed with less than 5v, a verification error is what you will typically get because the flash memory didn't actually get changed.  So make sure you had set your programmer for 5v operation if you get a verification error!

## Burn Bootloader

Nope, you don't need to do this on the 4/5/9/10, it won't do anything useful, these chips only have one fuse and the default settings are fine.

## Memory

The 4/5 only have 512K of Flash, keep your expectations in check :-)

All of them only have 32 bytes of RAM.

Pretty much forget about anything to do with Print (that includes Serial) as it's not practically useful in such limitations.  But all the basic functions, that is, digitalRead/Write, analogRead/Write and pinMode are good.

## analogRead

The 4/9 do not have any ADC so you can't use analogRead

## Toolchain for Older IDE Versions

The 4/5/9/10 are not supported by the Arduino IDE's built in gcc in IDE versions at least before 1.6.13,
so in order to use an old version of the IDE with these chips you will need to download
the "official" build of avr-gcc toolchain from Atmel.

  * Linux: http://www.atmel.com/tools/ATMELAVRTOOLCHAINFORLINUX.aspx
  * Windows: http://www.atmel.com/tools/ATMELAVRTOOLCHAINFORWINDOWS.aspx


After downloading the toolchain open your Arduino installation path, and navigate into the hardware/tools/ directory where you will see a directory called "avr".  Copy it to "avr.arduino" as a backkup.

Open the file you downloaded and copy the contents  (avr, bin, doc...) of the top level directory into your hardware/tools/avr/ diretcory overwriting any existing files, (seriously if you can't work this out, you are not ready for using a Tiny 4/5/9/10).

You should be good to go then.

## Reflashing a typical chinese USBasp

The best way, is to have two USBasp so you can flash one with the other, they are only a few dollars and you can always use a spare.

On the "target" (the one being flashed) close the "J2" jumper pads, on some this will be marked, on others it may not be but is usually fairly obvious two pads (the "Baite" ones with a switch which are my favourites have two unmarked pads on the bottom below the ATMega8A).

Connect the two togethor using, MOSI to MOSI, MISO to MISO etc, just use the normal programming cable plug one end into the source and the other into the target is typically fine.

Download the [appropriate hex file](https://github.com/bperrybap/usbasp/tree/1.06-alpha/bin/firmware/usbasp-v1.06-alpha-2016-05-18) for the ATMega which is on your target (which should be either an 8 or an 88, the sub-variant doesn't matter, eg for an ATMega8A based USBasp, use the atmega8 hex.

Use avrdude or whatever to flash it, the [bperrybap repository has some tool to help you there](https://github.com/bperrybap/usbasp/tree/1.06-alpha/bin/firmware) if you are not sure how to drive avrdude yourself (but given you are playing with a Tiny 4/5/9/10 you probably do).

Open the J2 jumper again.
