/*

'Pad-ino' 
a MIDI 6 Drum Pads Controller by Plank guitars and musical things by Shambien (Daniele Muscetta) - https://www.shambien.com

2025 01 04 - v1.0 - initial version.
2025 01 06 - v1.1 - code/unused references cleanup, fixed USB-Rename
2025 01 08 - v1.2 - performance improvements

For more information and credits refer to ReadMe in the GitHub repository https://github.com/dani3l3/PadIno


*/

#include "Libraries\usb_rename.cpp"
#include "Libraries\GM_Percussions.h"

#include <ResponsiveAnalogRead.h>
#include <MIDIUSB.h>

// Rename the device
USBRename PadIno = USBRename("Pad-ino", "Plank", "1.2");

// DEBUG Directives, leave them commented out for production use - only enable them for debugging and setting the right thresholds, etc
// #define DEBUG     1   // Will print DEBUG information
// #define VELDEBUG     1   // Will print DEBUG information just for velocity info (if knock > threshold)
// #define MIDIDEBUG     1   // Will print DEBUG information for MIDI messages to be sent

// Value at which the drum triggers / get considered 'hit hard enough' 
// TODO adjust based on Pad's sensitivy (which depends on the physical build and piezo's but mostly the enclosure, to filter out adjacent piezo's picking up knocks from other ones)
#define THRESHOLD 30   

// overridden functions (defined later below)
static void MIDI_setup();
static void MIDI_noteOn(int ch, int note, int velocity);
static void MIDI_noteOff(int ch, int note);

// Channel 10 is where drums are usually located
// the MIDI_noteOn function takes care of channel-1 since in code it's zero based so this is the human-readable channel
const int MIDI_CHANNEL = 10; 

// 31250 for MIDI class compliant but not really important here in this context as we use USBMIDI, not regular midi, and it is actually only used for the debugging information to serial
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
const float snapMultiplier = 0.5; // (0.0 - 1.0) - Increase for faster, but less smooth reading
const int potMin = THRESHOLD;
const int potMax = 511; // TODO might need tweaking based on how sensitive the hardware pads and piezo are (MAX: 1023)

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

  if(analog0.hasChanged()) {
    int a0value = analog0.getRawValue();
    if (checkForKnock(a0value)) {
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
        Serial.println(map(a0value, potMin, potMax, 0, 127));
      #endif

      MIDI_noteOn(MIDI_CHANNEL, Note1, map(a0value, potMin, potMax, 0, 127));
          
      // MIDI_noteOff(MIDI_CHANNEL, Note1); //it might be needed if you use this with a synth but typically not needed to trigger drum samples and it would slow down
      // delay(5); //can use it for debug but it will slow down responsiveness of the device
    
    }
    return; // to mininize triggering of adjacent pads
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

  if(analog1.hasChanged()) {
    int a1value = analog1.getRawValue();
    if (checkForKnock(a1value)) {
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
        Serial.println(map(a1value, potMin, potMax, 0, 127));
      #endif

      MIDI_noteOn(MIDI_CHANNEL, Note2, map(a1value, potMin, potMax, 0, 127));

      // MIDI_noteOff(MIDI_CHANNEL, Note2); //it might be needed if you use this with a synth but typically not needed to trigger drum samples and it would slow down
      // delay(5); //can use it for debug but it will slow down responsiveness of the device

    }
    return; // to mininize triggering of adjacent pads
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

  if(analog2.hasChanged()) {
    int a2value = analog2.getRawValue();
    if (checkForKnock(a2value)) {
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
        Serial.println(map(a2value, potMin, potMax, 0, 127));
      #endif

      MIDI_noteOn(MIDI_CHANNEL, Note3, map(a2value, potMin, potMax, 0, 127));
      
      // MIDI_noteOff(MIDI_CHANNEL, Note3); //it might be needed if you use this with a synth but typically not needed to trigger drum samples and it would slow down
      // delay(5); //can use it for debug but it will slow down responsiveness of the device
      
    }
    return; // to mininize triggering of adjacent pads
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

  if(analog3.hasChanged()) {
    int a3value = analog3.getRawValue();
    if (checkForKnock(a3value)) {
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
        Serial.println(map(a3value, potMin, potMax, 0, 127));
      #endif

      MIDI_noteOn(MIDI_CHANNEL, Note4, map(a3value, potMin, potMax, 0, 127));
      
      // MIDI_noteOff(MIDI_CHANNEL, Note4); //it might be needed if you use this with a synth but typically not needed to trigger drum samples and it would slow down
      // delay(5); //can use it for debug but it will slow down responsiveness of the device
      
    }
    return; // to mininize triggering of adjacent pads
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

  if(analog4.hasChanged()) {
    int a4value = analog4.getRawValue();
    if (checkForKnock(a4value)) {
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
        Serial.println(map(a4value, potMin, potMax, 0, 127));
      #endif

      MIDI_noteOn(MIDI_CHANNEL, Note5, map(a4value, potMin, potMax, 0, 127));

      // MIDI_noteOff(MIDI_CHANNEL, Note5); //it might be needed if you use this with a synth but typically not needed to trigger drum samples and it would slow down
      // delay(5); //can use it for debug but it will slow down responsiveness of the device

    }
    return; // to mininize triggering of adjacent pads
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

  if(analog5.hasChanged()) {
    int a5value = analog5.getRawValue();
    if (checkForKnock(a5value)) {
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
        Serial.println(map(a5value, potMin, potMax, 0, 127));
      #endif

      MIDI_noteOn(MIDI_CHANNEL, Note6, map(a5value, potMin, potMax, 0, 127));
      
      // MIDI_noteOff(MIDI_CHANNEL, Note6); //it might be needed if you use this with a synth but typically not needed to trigger drum samples and it would slow down
      // delay(5); //can use it for debug but it will slow down responsiveness of the device
      
    }
    return; // to mininize triggering of adjacent pads
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






