Special Considerations for the ATTiny 4/5/9/10
-------------------------------------------------------------------------------

## Toolchain

The 4/5/9/10 are not presently supported by the Arduino IDE's built in gcc, 
so in order to use the IDE with these chips you will need to download
the "official" build of avr-gcc toolchain from Atmel.

  * Linux: http://www.atmel.com/tools/ATMELAVRTOOLCHAINFORLINUX.aspx
  * Windows: http://www.atmel.com/tools/ATMELAVRTOOLCHAINFORWINDOWS.aspx


After downloading the toolchain open your Arduino installation path, and navigate into the hardware/tools/ directory where you will see a directory called "avr".  Copy it to "avr.arduino" as a backkup.

Open the file you downloaded and copy the contents  (avr, bin, doc...) of the top level directory into your hardware/tools/avr/ diretcory overwriting any existing files, (seriously if you can't work this out, you are not ready for using a Tiny 4/5/9/10).

You should be good to go then.

## Memory

The 4/5 only have 512K of Flash, keep your expectations in check :-)

All of them only have 32 bytes of RAM.

Pretty much forget about anything to do with Print (that includes Serial) as it's not practically useful in such limitations.  But all the basic functions, that is, digitalRead/Write, analogRead/Write and pinMode are good.

## analogRead

The 4/9 do not have any ADC so you can't use analogRead
