// Adafruit NeoPixel - Version: Latest 
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN        12
#define NUMPIXELS 8

#define DELAYVAL 500 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/whitenoise8192_int8.h> // recorded audio wavetable
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <Ead.h> // exponential attack decay
#include <EventDelay.h>
#include <mozzi_rand.h>

#define CONTROL_RATE 256 // Hz, powers of 2 are most reliable

Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin2(SIN2048_DATA);
Oscil<WHITENOISE8192_NUM_CELLS, AUDIO_RATE> aNoise(WHITENOISE8192_DATA);
Oscil<WHITENOISE8192_NUM_CELLS, AUDIO_RATE> aNoise2(WHITENOISE8192_DATA);
Oscil<WHITENOISE8192_NUM_CELLS, AUDIO_RATE> aNoise3(WHITENOISE8192_DATA);

Ead kEnvelope(CONTROL_RATE); // resolution will be CONTROL_RATE
Ead kEnvelope2(CONTROL_RATE); // resolution will be CONTROL_RATE
Ead kEnvelope3(CONTROL_RATE);

int gain;
int gain2;
int lastbutton1 = 1;
int lastbutton2 = 1;
int lastbutton3 = 1;
int noiselevel;
int noiselevel2;
int noiselevel3;
int noisegain;
int noisegain2;
int noisegain3;
int pitch = 50;
int pitch2 = 50;

int myLights[3] = {0,0,0};


void setup() {
  
pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

startMozzi(CONTROL_RATE); // :)
aNoise.setFreq((float)AUDIO_RATE/WHITENOISE8192_SAMPLERATE);
aNoise2.setFreq((float)AUDIO_RATE/WHITENOISE8192_SAMPLERATE);
aNoise3.setFreq((float)AUDIO_RATE/WHITENOISE8192_SAMPLERATE);

aSin.setFreq(pitch); // set the frequency
aSin2.setFreq(pitch2); // set the frequency

pinMode(2, INPUT_PULLUP);
pinMode(3, INPUT_PULLUP);
pinMode(4, INPUT_PULLUP);

Serial.begin(9600);

}

void updateControl(){
  
aNoise.setPhase(rand((unsigned int)WHITENOISE8192_NUM_CELLS));
aNoise2.setPhase(rand((unsigned int)WHITENOISE8192_NUM_CELLS));
aNoise3.setPhase(rand((unsigned int)WHITENOISE8192_NUM_CELLS));

int kickvolknob = mozziAnalogRead(0);
int kickpitchknob = mozziAnalogRead(1);
int kickdecayknob = mozziAnalogRead(2);
int snarevolknob = mozziAnalogRead(3);
int snarepitchknob = mozziAnalogRead(4);
int snaredecayknob = mozziAnalogRead(5);
int snarenoiseknob = mozziAnalogRead(6);
int hatvolknob = mozziAnalogRead(7);

int lowpitch = map(kickpitchknob, 1023, 0, 30, 200);
int lowpitch2 = map(snarepitchknob, 1023, 0, 30, 400);
int decaykick = map(kickdecayknob, 1023, 0, 200, 2000);
int decaysnare = map(snaredecayknob, 1023, 0, 200, 2000);
int noiselevelsnare = map(snarenoiseknob, 1023, 0, 50, 2);
int kickvolume = map(kickvolknob, 1023, 0, 50, 1);
int snarevolume = map(snarevolknob, 1023, 0, 50, 1);
int noiselevelhat = map(hatvolknob, 1023, 0, 50, 2);

aSin.setFreq(pitch);
aSin2.setFreq(pitch2);

//KICK

if(digitalRead(4)==1)
{if(lastbutton1==0)
{
kEnvelope.start(10,decaykick);
pitch = lowpitch + 50;
lastbutton1 = 1;
}}
else
{lastbutton1 = 0;}
if(pitch>lowpitch)
{pitch = pitch - 5;}
else
{pitch = lowpitch;}
gain = (int) kEnvelope.next()/kickvolume;


/// HIHAT

if(digitalRead(2)==1)
{if(lastbutton3==0)
{
kEnvelope3.start(10,200);
lastbutton3 = 1;
}}
else
{lastbutton3 = 0;}
noisegain3 = (int) kEnvelope3.next()/noiselevelhat;

///SNARE

if(digitalRead(3)==1)
{if(lastbutton2==0)
{
kEnvelope2.start(10,decaysnare);
pitch2 = lowpitch2 + 50;
lastbutton2 = 1;
}}
else
{lastbutton2 = 0;}
if(pitch2>lowpitch2)
{pitch2 = pitch2 - 5;}
else
{pitch2 = lowpitch2;}
gain2 = (int) kEnvelope2.next()/snarevolume;
noisegain2 = (int) kEnvelope2.next()/noiselevelsnare;

pixels.clear();

if(digitalRead(4)==1)
{myLights[0] = 1;}
else
{myLights[0] = 0;}

if(digitalRead(3)==1)
{myLights[1] = 1;}
else
{myLights[1] = 0;}

if(digitalRead(2)==1)
{myLights[2] = 1;}
else
{myLights[2] = 0;}

pixels.clear();

if(myLights[0]==1)
{pixels.setPixelColor(0, pixels.Color(255, 0, 0));
pixels.setPixelColor(1, pixels.Color(255, 0, 0));}
if(myLights[1]==1)
{pixels.setPixelColor(3, pixels.Color(0, 255, 0));
pixels.setPixelColor(4, pixels.Color(0, 255, 0));}
if(myLights[2]==1)
{pixels.setPixelColor(6, pixels.Color(0, 0, 255));
pixels.setPixelColor(7, pixels.Color(0, 0, 255));}

pixels.show();}
  


AudioOutput_t updateAudio(){
  return MonoOutput::from16Bit((gain*aSin.next()/2)+(gain2*aSin2.next()/2+noisegain2*aNoise2.next()/2)+noisegain3*aNoise3.next()/2);
}


void loop(){
  audioHook(); // required here
}
