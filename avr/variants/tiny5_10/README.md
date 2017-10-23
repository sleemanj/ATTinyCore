Special Considerations for the ATTiny 4/5/9/10 in Arduino ATTinyCore
-------------------------------------------------------------------------------

Older versions of the Arduino IDE did not have the necessary tools to support these chips.  

Starting with about 1.6.13 this should be OK now, but if you insist on using an old version of the IDE then you will find some help at the bottom of this page.

## Uploading

The 4/5/9/10 do not support uploading by serial, nor by SPI.  What they support is called "TPI".

The current version of the ["official" USBAsp firmware](http://www.fischl.de/usbasp/) supports uploading via TPI, connect MOSI to TPIDATA (pin 1) and SCK to TPICLK (pin 2).  This is how I upload to the Tiny 4/5/9/10.

Unfortunately the "auto clocking" firmware usually found on cheap chinese versions of the USBasp does not support TPI so you would have to re-flash one of those with the official firmware to get it to work.

There is an [ArduinoISP type TPI uploader](http://junkplusarduino.blogspot.co.nz/p/attiny10-resources.html) but I have not tried it personally.

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
