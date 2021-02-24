# Senior-Design
These are the files utilized in my senior design project which includes a PCB board design provided here, and integration with an arduino Mega 2560.
The main code that I believe is beneficial here would be the code used for the TLC5955 LED driver. There was not much information out there when I begain working on this at the time.
I was able to write functions to bit-bang the colors and intensity I wanted at "fast" speeds for an arduino since I used register manipulation directly

NOTE: the register manipulation will only translate perfectly to the same atmel chip used on said arduino board.

I have not gone back and cleaned this up, but I figured that I take from plenty of broken thoughts in the open-source world so I might as well give back what I have just sitting around incase it's useful to someone
