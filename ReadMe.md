# Pad-ino
a MIDI 6 Drum Pads Controller based on Arduino 
by Plank guitars and musical things by Shambien (Daniele Muscetta) - https://www.shambien.com

<img src="https://github.com/dani3l3/PadIno/blob/master/img/build-process-13.jpg?raw=true" alt="Pad-ino picture" width="250">

2025 01 04 - v.01 - initial version (still needs some tweaking of thresholds for sensitivity and some code/comments cleanup. Also I tried incorporating the USB-Rename library but it still doesn't work so it's commented out, TBD.)

Works on Arduino Pro Micro / Leonardo - UNTESTED on any other board (but it should work on all those based on the Atmega32U4 chipset).

It can be built for under 10$ with the following electronic components:
1. Arduino Pro Micro (or Leonardo)
1. 6 ceramic piezo discs
1. 6 1MegaOhm resistors
1. 6 Zener diodes 5.1V

Besides the code, in this repository you can find [several pictures I took during various phases of the build process](./img/).

Contains portions of code, inspiration and teachings derived from various sources such as: 
- Arduino MIDIUSB Library https://github.com/arduino-libraries/MIDIUSB
- Responsive Analog Read library https://github.com/dxinteractive/ResponsiveAnalogRead
- Simple DYI Electronic Music Projects https://github.com/diyelectromusic/sdemp
- Nerd Musician (Gustavo Silveira) https://github.com/silveirago and https://www.youtube.com/@NerdMusician
- BeginnerToPro https://www.youtube.com/@BeginnerTopro https://www.youtube.com/watch?v=5SL-W1Ynn3c (the hardware circuit I built - piezo's, resistors and diodes - is based on this video's circuit, even if it was for an 'octapad' and mine has only six pads)
- Autonomia Digital https://www.youtube.com/watch?v=sU0ZmjipUU0 https://www.youtube.com/@autonomiadigital9664

...and many other sources I consulted and learned from but I did not bookmark (but you might find them by searching for keywords such as 'octapad piezo arduino' on YouTube).

I cannot thank enough all of the authors of the above for teaching me thru their tutorials, experiences and code! 
May God bless you always.

Keep grooving!

Shambien


