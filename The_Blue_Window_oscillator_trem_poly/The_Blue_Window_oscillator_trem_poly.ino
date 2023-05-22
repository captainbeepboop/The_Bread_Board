#include <MIDI.h>
#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/saw2048_int8.h> // sine table for oscillator
#include <tables/saw2048_int8.h>
#include <tables/sin2048_int8.h>// sine table for oscillator
#include <ADSR.h>
#include <mozzi_midi.h>
#include <Adafruit_NeoPixel.h>
#include <StateVariable.h>
#ifdef __AVR__
#endif

#define PIN 13
#define NUMPIXELS 8 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

MIDI_CREATE_DEFAULT_INSTANCE();
Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> aSaw(SAW2048_DATA);
Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> aSaw2(SAW2048_DATA);
Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> aSaw3(SAW2048_DATA);
ADSR <CONTROL_RATE, CONTROL_RATE> envelope1;
ADSR <CONTROL_RATE, CONTROL_RATE> envelope2;
ADSR <CONTROL_RATE, CONTROL_RATE> envelope3;
 // Hz, powers of 2 are most reliable


#define knob1a 2
#define knob1b 5
#define knob1c 4
#define knob2a 3
#define knob2b 6
#define knob2c 7
#define switch1 10
#define switch2 11
#define button 12
#define pot1 A0
#define pot2 A1
#define pitchpin A5
#define triggerpin A3
#define CONTROL_RATE 128
#define MIDIPIN A4
#define TRIGGERPIN2 A2


volatile int knob1value = 0;
volatile int knob2value = 0;
int gain1 = 50;
int gain2 = 50;
int gain3 = 50;

int gain1b = 0;
int gain2b = 0;
int gain3b = 0;

int basepitch = 220;
int pitch1 = 220;
int pitch2 = 220;
int pitch3 = 220;
int vibrato = 0;

bool knob1state = 0;
bool knob1previous = 1;
bool knob2state = 0;
bool knob2previous = 1;

bool envtriggerprevious = 0;
bool envtrigger = 1;
bool spintrigger = 0;
bool VIBRATOSTATE = 1;

bool midisource = 1;

unsigned int duration, attack, decay, sustain, release_ms;

const int numReadings = 30;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int pitchshift = 0;

void HandleNoteOn(byte channel, byte note, byte velocity) {

if(channel == 2)
{
  pitch1=mtof(float(note));
  
  int pot1value = mozziAnalogRead(pot1);
  int pot2value = mozziAnalogRead(pot2);
  int decay = map(pot1value,0,1023,100,2000);
  int attack = map(pot2value,0,1023,0,1000);
envelope1.setTimes(attack,decay,sustain,release_ms);
if(mozziAnalogRead(triggerpin) < 100)
 {envelope1.noteOn();}
 midisource = 1; 
}
  
  if(channel == 3)
{
  pitch2=mtof(float(note));
  
  int pot1value = mozziAnalogRead(pot1);
  int pot2value = mozziAnalogRead(pot2);
  int decay = map(pot1value,0,1023,100,2000);
  int attack = map(pot2value,0,1023,0,1000);
envelope2.setTimes(attack,decay,sustain,release_ms);
  if(mozziAnalogRead(triggerpin) < 100)
  {
  envelope2.noteOn();
}
midisource = 1;  
}
  if(channel == 4)
{
  pitch3=mtof(float(note));
  
  int pot1value = mozziAnalogRead(pot1);
  int pot2value = mozziAnalogRead(pot2);
  int decay = map(pot1value,0,1023,100,2000);
  int attack = map(pot2value,0,1023,0,1000);
envelope3.setTimes(attack,decay,sustain,release_ms);
  if(mozziAnalogRead(triggerpin) < 100)
  {
  envelope3.noteOn();
}
midisource = 1;  
}
  
if(channel == 5)
{pitch1=mtof(float(note));
if(mozziAnalogRead(triggerpin) < 100)
  {
    gain1b = 1;
  }
midisource = 0;
}
  
if(channel == 6)
{pitch2=mtof(float(note));
if(mozziAnalogRead(triggerpin) < 100)
  {
    gain2b = 1;
  }
midisource = 0;
}
  
if(channel == 7)
{pitch3=mtof(float(note));
if(mozziAnalogRead(triggerpin) < 100)
  {
    gain3b = 1;
  }
midisource = 0;
}


}

void HandleNoteOff(byte channel, byte note, byte velocity) {
if(channel == 5)
{gain1b = 0;}
if(channel == 6)
{gain2b = 0;}
if(channel == 7)
{gain3b = 0;}
}



void setup(){
  
for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;}
  
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  
  startMozzi(CONTROL_RATE);
  aSaw.setFreq(pitch1);
  aSaw2.setFreq(pitch2);
  aSaw3.setFreq(pitch3);


Serial.begin(9600);
pinMode(knob1a, INPUT_PULLUP);
pinMode(knob1b, INPUT_PULLUP);
pinMode(knob1c, INPUT_PULLUP);
pinMode(knob2a, INPUT_PULLUP);
pinMode(knob2b, INPUT_PULLUP);
pinMode(knob2c, INPUT_PULLUP);
pinMode(switch1, INPUT_PULLUP);
pinMode(switch2, INPUT_PULLUP);
pinMode(button, INPUT_PULLUP);
pinMode(MIDIPIN, INPUT_PULLUP);
pinMode(pot1, INPUT);
pinMode(pitchpin, INPUT);
pinMode(triggerpin, INPUT_PULLUP);
pinMode(TRIGGERPIN2, OUTPUT);
attachInterrupt(digitalPinToInterrupt(knob1a), encoder1, FALLING);
attachInterrupt(digitalPinToInterrupt(knob2a), encoder2, FALLING);

  // Connect the HandleNoteOn function to the library, so it is called upon reception of a NoteOn.
  MIDI.setHandleNoteOn(HandleNoteOn);  // Put only the name of the function
  MIDI.setHandleNoteOff(HandleNoteOff);  // Put only the name of the function  // Initiate MIDI communications, listen to all channels (not needed with Teensy usbMIDI)
  MIDI.setHandleControlChange(MyCCFunction);
  MIDI.begin(MIDI_CHANNEL_OMNI);

  envelope1.setADLevels(255,64);
  envelope2.setADLevels(255,64);
  envelope3.setADLevels(255,64);

int attack = 0;
int decay = 2000;
int sustain = 0;
int release = 0;
envelope1.setTimes(attack,decay,sustain,release_ms);
envelope2.setTimes(attack,decay,sustain,release_ms);
envelope3.setTimes(attack,decay,sustain,release_ms);
  
}

void MyCCFunction(byte channel, byte number, byte value) {
if(number == 123)
{gain1b = 0;
gain2b = 0;
gain3b = 0;}
}

void updateControl(){
  
if(mozziAnalogRead(MIDIPIN)<100)
{VIBRATOSTATE = 0;}
else
{VIBRATOSTATE = 1;}

  
total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = mozziAnalogRead(pitchpin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
int pitchshift = mozziAnalogRead(pitchpin) - average;
if(pitchshift < -20)
{spintrigger = 0;}
if(pitchshift > 20)
{spintrigger = 1;}

digitalWrite(TRIGGERPIN2, spintrigger);

//MIDI.read();

int pot1value = mozziAnalogRead(pot1);
int pot2value = mozziAnalogRead(pot2);

bool envtrigger = digitalRead(button);
if(envtrigger > envtriggerprevious)
{
gain1b = 1;
gain2b = 1;
gain3b = 1;
envelope1.noteOn();
envelope2.noteOn();
envelope3.noteOn();
int decay = map(pot1value,0,1023,100,2000);
int attack = map(pot2value,0,1023,0,1000);
envelope1.setTimes(attack,decay,sustain,release_ms);
envelope2.setTimes(attack,decay,sustain,release_ms);
envelope3.setTimes(attack,decay,sustain,release_ms);
}
envtriggerprevious = envtrigger;
if(envtrigger == 0)
{
gain1b = 0;
gain2b = 0;
gain3b = 0; 
}

if(digitalRead(knob1c)==0 && knob1previous==1)
{knob1state = !knob1state;
//only make it happen on first click
knob1value = 0;
knob1previous = 0;
}
knob1previous = digitalRead(knob1c);

if(digitalRead(knob2c)==0 && knob2previous==1)
{knob2state = !knob2state;
//only make it happen on first click
knob2value = 0;
knob2previous = 0;
}
knob2previous = digitalRead(knob2c);

if(VIBRATOSTATE == 1)
{pitchshift = 0;}
aSaw.setFreq(pitch1 + pitchshift);
if(knob1state == 0)
{aSaw2.setFreq(pitch2 + pitchshift);}
else
{aSaw2.setFreq(pitch1 + knob1value + pitchshift);}
if(knob2state == 0)
{aSaw3.setFreq(pitch3 + pitchshift);}
else
{aSaw3.setFreq(pitch1 + knob2value + pitchshift);}

envelope1.update();
envelope2.update();
envelope3.update();

gain1 = envelope1.next(); // this is where it's different to an audio rate envelope
gain2 = envelope2.next(); // this is where it's different to an audio rate envelope
gain3 = envelope3.next(); // this is where it's different to an audio rate envelope

pixels.clear();

if(midisource == 1)
{int pixel = map(gain1,10,240,0,8);
pixels.setPixelColor(pixel-1, pixels.Color(0, 0, 255));}
else
{if(gain1b == 1)
{pixels.setPixelColor(0, pixels.Color(0, 0, 255));
pixels.setPixelColor(1, pixels.Color(0, 0, 255));}
if(gain2b == 1)
{pixels.setPixelColor(3, pixels.Color(0, 0, 255));
pixels.setPixelColor(4, pixels.Color(0, 0, 255));}
if(gain3b == 1)
{pixels.setPixelColor(6, pixels.Color(0, 0, 255));
pixels.setPixelColor(7, pixels.Color(0, 0, 255));}}

pixels.show();

if(pot1value==1023)
{envelope1.noteOn();
envelope2.noteOn();
envelope3.noteOn();
}

if(digitalRead(switch1)==0)
{gain2 = 0;
 gain2b = 0; 
}


if(digitalRead(switch2)==0)
{gain3 = 0;
  gain3b = 0;
}

}

void encoder1() {
 if (digitalRead(knob1a) == digitalRead(knob1b)) {
 knob1value++;
 }
 else {
 knob1value--;
 }
}

void encoder2() {
 if (digitalRead(knob2a) == digitalRead(knob2b)) {
 knob2value++;
 }
 else {
 knob2value--;
 }
}




AudioOutput_t updateAudio(){
if(midisource == 1)
{return MonoOutput::from8Bit(aSaw.next()*gain1/273+aSaw2.next()*gain2/273+aSaw3.next()*gain3/273);}
else
{return MonoOutput::from8Bit(aSaw.next()*gain1b+aSaw2.next()*gain2b+aSaw3.next()*gain3b);}
}


void loop(){
  MIDI.read();
  audioHook(); // required here
}


