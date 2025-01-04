/*

'Pad-ino' MIDI 6 Drum Pads Controller by Plank guitars and musical things by Shambien (Daniele Muscetta) - https://www.shambien.com

v.01 - initial version

Works on Arduino Pro Micro / Leonardo - UNTESTED on any other board (but it should work on all those based on the Atmega32U4 chipset).


Contains portions of code, inspiration and teachings derived from various sources such as: 
- Arduino MIDIUSB Library https://github.com/arduino-libraries/MIDIUSB
- Simple DYI Electronic Music Projects https://github.com/diyelectromusic/sdemp
- Nerd Musician (Gustavo Silveira) https://github.com/silveirago and https://www.youtube.com/@NerdMusician
- BeginnerToPro https://www.youtube.com/@BeginnerTopro https://www.youtube.com/watch?v=5SL-W1Ynn3c (the hardware circuit I built - piezo's, resistors and diodes - is based on this video)
- Autonomia Digital https://www.youtube.com/watch?v=sU0ZmjipUU0 https://www.youtube.com/@autonomiadigital9664

...and many other sources I consulted and learned from but I did not bookmark.

I cannot thank all of the authors of the above enough for teaching me! May God bless you always.
-


*/

#include "Libraries\usb_rename.h"
#include "Libraries\GM_Percussions.h"

// TODO understand if it's better to use this library or not (it's meant for potentiometers, might not be the right choice for piezos...)
#include <ResponsiveAnalogRead.h>

// TODO check why the stuff from Silveira use different codepaths and libraries on different boards (Uno vs Pro Micro == ATMEGA328 vs ATMEGA32U4)
#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <frequencyToNote.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>


// TODO - final name?
// USBRename PadIno = USBRename("Pad-ino", "Plank", "1");



// #define DEBUG     1   // Will print DEBUG information
// #define VELDEBUG     1   // Will print DEBUG information just for velocity info (if knock > threshold)
// #define MIDIDEBUG     1   // Will print DEBUG information for MIDI messages to be sent

#define THRESHOLD 20  // Value at which the drum triggers // TODO adjust

// overridden functions (defined later below)
static void MIDI_setup();
static void MIDI_noteOn(int ch, int note, int velocity);
static void MIDI_noteOff(int ch, int note);


// TODO check if special treatment happens for channel 10 (channel 10 where drums are usually located) or not
// the MIDI_noteOn function takes care of channel-1 since in code it's zero based
const int MIDI_CHANNEL = 10; 


// use if using with ATmega328 (uno, mega, nano...)
// 31250 for MIDI class compliant | 115200 fastest
const int BAUD_RATE = 31250;


// Pads mapped to analog inputs
const int Pad1 = A0;
const int Pad2 = A1;
const int Pad3 = A2;
const int Pad4 = A3;
// Arduino Pro Micro does not have A4 and A5
const int Pad5 = A6; // labelled '4' on Arduino Pro Micro
const int Pad6 = A8; // labelled '8' on Arduino Pro Micro


// Notes to be sent by each pad (uses GM library definitions)
const int Note1 = CLOSED_HI_HAT;
const int Note2 = HI_FLOOR_TOM;
const int Note3 = LO_CONGA;
const int Note4 = LO_FLOOR_TOM;
const int Note5 = ELECTRIC_SNARE;
const int Note6 = BASS_DRUM_1;


// for ResponsiveAnalogRead resolution and mapping velocities
const float snapMultiplier = 0.01; // (0.0 - 1.0) - Increase for faster, but less smooth reading
const int potMin = 10;
const int potMax = 256; // might need tweaking based on how sensitive the hardware pads and piezo are (MAX: 1023)

// TODO not sure if the ResponsiveAnalogRead library is useful or not for Piezo's... it was meant for potentiometers
ResponsiveAnalogRead analog0(Pad1, true, snapMultiplier);
ResponsiveAnalogRead analog1(Pad2, true, snapMultiplier);
ResponsiveAnalogRead analog2(Pad3, true, snapMultiplier);
ResponsiveAnalogRead analog3(Pad4, true, snapMultiplier);
ResponsiveAnalogRead analog4(Pad5, true, snapMultiplier);
ResponsiveAnalogRead analog5(Pad6, true, snapMultiplier);



void setup() {

  #ifdef DEBUG
    // start serial communication for debugging
    Serial.begin(BAUD_RATE);

    Serial.println("Debug mode");
    Serial.println();
  #endif

  pinMode(Pad1, INPUT);
  pinMode(Pad2, INPUT);
  pinMode(Pad3, INPUT);
  pinMode(Pad4, INPUT);
  pinMode(Pad5, INPUT);
  pinMode(Pad6, INPUT);


  analog0.setAnalogResolution(potMax);
  analog1.setAnalogResolution(potMax);
  analog2.setAnalogResolution(potMax);
  analog3.setAnalogResolution(potMax);
  analog4.setAnalogResolution(potMax);
  analog5.setAnalogResolution(potMax);


}

void loop() {

  // update the ResponsiveAnalogRead object every loop
  analog0.update();
  analog1.update();
  analog2.update();
  analog3.update();
  analog4.update();
  analog5.update();



  
  // analog0
  #ifdef DEBUG
    Serial.print(analog0.getRawValue());
    Serial.print("\t");
    Serial.print("Analog 0: " + analog0.getRawValue());
    
    // if the responsive value has change, print out 'changed'
    if(analog0.hasChanged()) {
      Serial.print("\tchanged");
    }
    
    Serial.println("");
  #endif

  if (checkForKnock(analog0.getRawValue())) {
    #ifdef MIDIDEBUG
      Serial.print("Channel");
      Serial.print("\t");
      Serial.print(MIDI_CHANNEL);
      Serial.print("\t");
      Serial.print(" ;");
      Serial.print("Note");
      Serial.print("\t");
      Serial.print(Note1);
      Serial.print("\t");
      Serial.print(" ;");
      Serial.print("\t");
      Serial.println("Velocity");
      Serial.print("\t");
      Serial.println(map(analog0.getRawValue(), potMin, potMax, 0, 127));
    #endif

    MIDI_noteOn(MIDI_CHANNEL, Note1, map(analog0.getRawValue(), potMin, potMax, 0, 127));
    delay(20);
  };




  // analog1
  #ifdef DEBUG
    Serial.print(analog1.getRawValue());
    Serial.print("\t");
    Serial.print("Analog 1: " + analog1.getRawValue());
    
    // if the responsive value has change, print out 'changed'
    if(analog1.hasChanged()) {
      Serial.print("\tchanged");
    }
    
    Serial.println("");
  #endif

  if (checkForKnock(analog1.getRawValue())) {
    #ifdef MIDIDEBUG
      Serial.print("Channel");
      Serial.print("\t");
      Serial.print(MIDI_CHANNEL);
      Serial.print("\t");
      Serial.print(" ;");
      Serial.print("Note");
      Serial.print("\t");
      Serial.print(Note2);
      Serial.print("\t");
      Serial.print(" ;");
      Serial.print("\t");
      Serial.println("Velocity");
      Serial.print("\t");
      Serial.println(map(analog1.getRawValue(), potMin, potMax, 0, 127));
    #endif

    MIDI_noteOn(MIDI_CHANNEL, Note2, map(analog1.getRawValue(), potMin, potMax, 0, 127));
    delay(20);
  };




  // analog2
  #ifdef DEBUG
    Serial.print(analog2.getRawValue());
    Serial.print("\t");
    Serial.print("Analog 2: " + analog2.getRawValue());
    
    // if the responsive value has change, print out 'changed'
    if(analog2.hasChanged()) {
      Serial.print("\tchanged");
    }
    
    Serial.println("");
  #endif

  if (checkForKnock(analog2.getRawValue())) {
    #ifdef MIDIDEBUG
      Serial.print("Channel");
      Serial.print("\t");
      Serial.print(MIDI_CHANNEL);
      Serial.print("\t");
      Serial.print(" ;");
      Serial.print("Note");
      Serial.print("\t");
      Serial.print(Note3);
      Serial.print("\t");
      Serial.print(" ;");
      Serial.print("\t");
      Serial.println("Velocity");
      Serial.print("\t");
      Serial.println(map(analog2.getRawValue(), potMin, potMax, 0, 127));
    #endif

    MIDI_noteOn(MIDI_CHANNEL, Note3, map(analog2.getRawValue(), potMin, potMax, 0, 127));
    delay(20);
  };




  // analog3
  #ifdef DEBUG
    Serial.print(analog3.getRawValue());
    Serial.print("\t");
    Serial.print("Analog 3: " + analog3.getRawValue());
    
    // if the responsive value has change, print out 'changed'
    if(analog3.hasChanged()) {
      Serial.print("\tchanged");
    }
    
    Serial.println("");
  #endif

  if (checkForKnock(analog3.getRawValue())) {
    #ifdef MIDIDEBUG
      Serial.print("Channel");
      Serial.print("\t");
      Serial.print(MIDI_CHANNEL);
      Serial.print("\t");
      Serial.print(" ;");
      Serial.print("Note");
      Serial.print("\t");
      Serial.print(Note4);
      Serial.print("\t");
      Serial.print(" ;");
      Serial.print("\t");
      Serial.println("Velocity");
      Serial.print("\t");
      Serial.println(map(analog3.getRawValue(), potMin, potMax, 0, 127));
    #endif

    MIDI_noteOn(MIDI_CHANNEL, Note4, map(analog3.getRawValue(), potMin, potMax, 0, 127));
    delay(20);
  };




  // analog4
  #ifdef DEBUG
    Serial.print(analog4.getRawValue());
    Serial.print("\t");
    Serial.print("Analog 4: " + analog4.getRawValue());
    
    // if the responsive value has change, print out 'changed'
    if(analog4.hasChanged()) {
      Serial.print("\tchanged");
    }
    
    Serial.println("");
  #endif

  if (checkForKnock(analog4.getRawValue())) {
    #ifdef MIDIDEBUG
      Serial.print("Channel");
      Serial.print("\t");
      Serial.print(MIDI_CHANNEL);
      Serial.print("\t");
      Serial.print(" ;");
      Serial.print("Note");
      Serial.print("\t");
      Serial.print(Note5);
      Serial.print("\t");
      Serial.print(" ;");
      Serial.print("\t");
      Serial.println("Velocity");
      Serial.print("\t");
      Serial.println(map(analog4.getRawValue(), potMin, potMax, 0, 127));
    #endif

    MIDI_noteOn(MIDI_CHANNEL, Note5, map(analog4.getRawValue(), potMin, potMax, 0, 127));
    delay(20);
  };




  // analog5
  #ifdef DEBUG
    Serial.print(analog5.getRawValue());
    Serial.print("\t");
    Serial.print("Analog 5: " + analog5.getRawValue());
    
    // if the responsive value has change, print out 'changed'
    if(analog5.hasChanged()) {
      Serial.print("\tchanged");
    }

    Serial.println("");
  #endif

  if (checkForKnock(analog5.getRawValue())) {
    #ifdef MIDIDEBUG
      Serial.print("Channel");
      Serial.print("\t");
      Serial.print(MIDI_CHANNEL);
      Serial.print("\t");
      Serial.print(" ;");
      Serial.print("Note");
      Serial.print("\t");
      Serial.print(Note6);
      Serial.print("\t");
      Serial.print(" ;");
      Serial.print("\t");
      Serial.println("Velocity");
      Serial.print("\t");
      Serial.println(map(analog5.getRawValue(), potMin, potMax, 0, 127));
    #endif

    MIDI_noteOn(MIDI_CHANNEL, Note6, map(analog5.getRawValue(), potMin, potMax, 0, 127));
    delay(20);
  };


}



// this function checks to see if a detected knock is above min threshold
bool checkForKnock(int value) {
  if (value > THRESHOLD) {
    #ifdef VELDEBUG
      Serial.print("Valid knock of value ");
      Serial.println(value);
    #endif

    return true;
  }
  else {
    #ifdef DEBUG
      Serial.print("Bad knock value ");
      Serial.println(value);
    #endif

    return false;
  }
}







// MIDI Functions overrides
// See https://www.midi.org/specifications/item/table-1-summary-of-midi-message

void MIDI_setup()
{

}

void MIDI_noteOn(int ch, int note, int velocity)
{
  #ifdef DEBUG
    Serial.print("MIDI_noteOn ");
    Serial.println(note);
  #endif

  // midiEventPacket_t noteOn = {0x09, 0x90 | (ch-1), note & 0x7F, velocity & 0x7F};
  midiEventPacket_t noteOn = {0x09, 0x90 | (ch-1), note, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void MIDI_noteOff(int ch, int note)
{
  #ifdef DEBUG
    Serial.print("MIDI_noteOff ");
    Serial.println(note);
  #endif

  // midiEventPacket_t noteOff = {0x08, 0x80 | (ch-1), note, 1};
  midiEventPacket_t noteOff = {0x08, 0x80 | (ch-1), note, 1};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}





