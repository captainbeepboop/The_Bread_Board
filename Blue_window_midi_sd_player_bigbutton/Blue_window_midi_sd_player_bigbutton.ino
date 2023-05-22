
#include <SdFat.h>
#include <MD_MIDIFile.h>
#include <MIDI.h>

#define KICKOUT 7
#define SNAREOUT 5
#define HATOUT 4
#define TRIGOUT 6

MIDI_CREATE_DEFAULT_INSTANCE();

#define USE_MIDI  1  // set to 1 for MIDI output, 0 for debug output

#if USE_MIDI // set up for direct MIDI serial output

#define DEBUGS(s)
#define DEBUG(s, x)
#define DEBUGX(s, x)
#define SERIAL_RATE 31250

#else // don't use MIDI to allow printing debug statements

#define DEBUGS(s)
#define DEBUG(s, x)
#define DEBUGX(s, x)
#define SERIAL_RATE 9600

#endif // USE_MIDI

int note1 = 0;
int velocity1 = 0;
int note1last = 0;
int err;
bool play = 0;
int song = 0;

int button1last = 1;
int button2last = 1;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long lastDebounceTime2 = 0;
unsigned long debounceDelay = 1000;
unsigned long debounceDelay2 = 1000;
int state = 3;


const uint16_t WAIT_DELAY = 500; // ms


#define BUTTON1 3
#define BUTTON2 2
#define LED 8
#define LED1 A1
#define LED2 A0
#define REDLIGHT 9
#define BLUELIGHT A2

// SD chip select pin for SPI comms.
// Default SD chip select is the SPI SS pin (10 on Uno, 53 on Mega).
const uint8_t SD_SELECT = SS;
uint8_t lastnote1 = 0x00;

#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))
const char *tuneList[] = {"BATOUTOFHELL.midi", "DJSAMMY.midi"};
static uint16_t currTune = ARRAY_SIZE(tuneList);

// The files in the tune list should be located on the SD card
// or an error will occur opening the file and the next in the
// list will be opened (skips errors).

SDFAT  SD;
MD_MIDIFile SMF;

void midiCallback(midi_event *pev)
// Called by the MIDIFile library when a file event needs to be processed
// thru the midi communications interface.
// This callback is set up in the setup() function.
{
#if USE_MIDI

  if (pev->channel == 0x00)
  {
    MIDI.sendControlChange(123, 0, 1);
  }

  if (pev->data[0] == 0x90)
  { if (pev->channel == 0x00)
    {
      MIDI.sendNoteOn(pev->data[1], 127, 1);
    }
    if (pev->channel == 0x01)
    {
      MIDI.sendNoteOn(pev->data[1], 127, 2);
    }
    if (pev->channel == 0x02)
    {
      MIDI.sendNoteOn(pev->data[1], 127, 3);
    }
    if (pev->channel == 0x03)
    {
      MIDI.sendNoteOn(pev->data[1], 127, 4);
    }
    if (pev->channel == 0x04)
    {
      MIDI.sendNoteOn(pev->data[1], 127, 5);
    }
    if (pev->channel == 0x05)
    {
      MIDI.sendNoteOn(pev->data[1], 127, 6);
    }
    if (pev->channel == 0x06)
    {
      MIDI.sendNoteOn(pev->data[1], 127, 7);
    }
    if (pev->channel == 0x09)
    { if (pev->data[1] == 0x24)
      {
        digitalWrite(KICKOUT, HIGH);
      }
      if (pev->data[1] == 0x26)
      {
        digitalWrite(SNAREOUT, HIGH);
      }
      if (pev->data[1] == 0x2a)
      {
        digitalWrite(HATOUT, HIGH);
      }
    }
    if (pev->channel == 0x07)
    {
      digitalWrite(TRIGOUT, HIGH);
    }
  }

  if (pev->data[0] == 0x80)
  { if (pev->channel == 0x00)
    {
      MIDI.sendNoteOff(pev->data[1], 0, 1);
    }
    if (pev->channel == 0x01)
    {
      MIDI.sendNoteOff(pev->data[1], 0, 2);
    }
    if (pev->channel == 0x02)
    {
      MIDI.sendNoteOff(pev->data[1], 0, 3);
    }
    if (pev->channel == 0x03)
    {
      MIDI.sendNoteOff(pev->data[1], 0, 4);
    }
    if (pev->channel == 0x04)
    {
      MIDI.sendNoteOff(pev->data[1], 0, 5);
    }
    if (pev->channel == 0x05)
    {
      MIDI.sendNoteOff(pev->data[1], 0, 6);
    }
    if (pev->channel == 0x06)
    {
      MIDI.sendNoteOff(pev->data[1], 0, 7);
    }
    if (pev->channel == 0x09)
    { if (pev->data[1] == 0x24)
      {
        digitalWrite(KICKOUT, LOW);
      }
      if (pev->data[1] == 0x26)
      {
        digitalWrite(SNAREOUT, LOW);
      }
      if (pev->data[1] == 0x2a)
      {
        digitalWrite(HATOUT, LOW);
      }
    }
    if (pev->channel == 0x07)
    {
      digitalWrite(TRIGOUT, LOW);
    }
  }

#else


  if (pev->data[0] == 0x90)
  { Serial.print(pev->channel);
    if (pev->channel == 0x09)
    {
      //Serial.println(pev->data[1]);
      if (pev->data[1] == 0x24)
      { //Serial.println("kickon");
        digitalWrite(KICKOUT, HIGH);
      }
      if (pev->data[1] == 0x26)
      { //Serial.println("snareon");
        digitalWrite(SNAREOUT, HIGH);
      }
      if (pev->data[1] == 0x2a)
      { //Serial.println("haton");
        digitalWrite(HATOUT, HIGH);
      }
    }
  }

  if (pev->data[0] == 0x80)
  { if (pev->channel == 0x09)
    { if (pev->data[1] == 0x24)
      { digitalWrite(KICKOUT, LOW);
        //Serial.println("kickoff");
      }
      if (pev->data[1] == 0x26)
      { digitalWrite(SNAREOUT, LOW);
        //Serial.println("snareoff");
      }
      if (pev->data[1] == 0x2a)
      { digitalWrite(HATOUT, LOW);
        //Serial.println("hatoff");
      }
    }
  }

#endif

  DEBUG("\nM T", pev->track);
  DEBUG(":  Ch ", pev->channel + 1);
  DEBUGS(" Data");
  for (uint8_t i = 0; i < pev->size; i++)
  {
    DEBUGX(" ", pev->data[i]);
  }
}

void midiSilence(void)
// Turn everything off on every channel.
// Some midi files are badly behaved and leave notes hanging, so between songs turn
// off all the notes and sound
{
  midi_event ev;

  // All sound off
  // When All Sound Off is received all oscillators will turn off, and their volume
  // envelopes are set to zero as soon as possible.
  ev.size = 0;
  ev.data[ev.size++] = 0xb0;
  ev.data[ev.size++] = 120;
  ev.data[ev.size++] = 0;

  for (ev.channel = 0; ev.channel < 16; ev.channel++)
    midiCallback(&ev);
}

void setup(void)
{

  pinMode(KICKOUT, OUTPUT);
  pinMode(SNAREOUT, OUTPUT);
  pinMode(HATOUT, OUTPUT);
  pinMode(TRIGOUT, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(REDLIGHT, OUTPUT);
  pinMode(BLUELIGHT, OUTPUT);
  digitalWrite(KICKOUT, LOW);
  digitalWrite(SNAREOUT, LOW);
  digitalWrite(HATOUT, LOW);
  digitalWrite(TRIGOUT, LOW);
  digitalWrite(LED, LOW);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  SMF.pause(true);

digitalWrite(REDLIGHT, HIGH);
digitalWrite(BLUELIGHT, HIGH);

  attachInterrupt(digitalPinToInterrupt(BUTTON1), PIN1INTERRUPT, LOW);
  attachInterrupt(digitalPinToInterrupt(BUTTON2), PIN2INTERRUPT, LOW);


  MIDI.begin(MIDI_CHANNEL_OMNI);                      // Launch MIDI and listen to channel 4

  Serial.begin(SERIAL_RATE);

  DEBUGS("\n[MidiFile Play List]");

  // Initialize SD
  if (!SD.begin(SD_SELECT, SPI_FULL_SPEED))
  {
    DEBUGS("\nSD init fail!");
    while (true) ;
  }

  // Initialize MIDIFile
  SMF.begin(&SD);
  SMF.setMidiHandler(midiCallback);

}

void PIN1INTERRUPT() 
{if(play == 0)
{
if((millis() - lastDebounceTime) > debounceDelay)
{
detachInterrupt(digitalPinToInterrupt(BUTTON1));
detachInterrupt(digitalPinToInterrupt(BUTTON2));
play = 1;
digitalWrite(REDLIGHT, HIGH);
digitalWrite(BLUELIGHT, LOW);
digitalWrite(LED1, HIGH);
digitalWrite(LED2, LOW);
song = 0;
state = 0;
SMF.pause(false);
digitalWrite(LED, HIGH);}
lastDebounceTime2 = millis();
}}


void PIN2INTERRUPT() 
{if(play == 0)
{
if((millis() - lastDebounceTime2) > debounceDelay)
{
detachInterrupt(digitalPinToInterrupt(BUTTON1));
detachInterrupt(digitalPinToInterrupt(BUTTON2));  
play = 1;
digitalWrite(REDLIGHT, LOW);
digitalWrite(BLUELIGHT, HIGH);
digitalWrite(LED1, LOW);
digitalWrite(LED2, HIGH);
song = 1;
state = 0;
SMF.pause(false);
digitalWrite(LED, HIGH);}
lastDebounceTime2 = millis();
}}

void loop(void)
{
  static uint32_t timeStart;

  switch (state)
  {
    case 0:    // now idle, set up the next tune
      {
        int err;

        DEBUGS("\nS_IDLE");
      SMF.close();
      midiSilence();

DEBUG("\nFile: ", tuneList[song]);
        err = SMF.load(tuneList[song]);
        if (err != MD_MIDIFile::E_OK)
        {
          DEBUG(" - SMF load Error ", err);
          timeStart = millis();
          state = 3;
          DEBUGS("\nWAIT_BETWEEN");
        }
        else
        {
          DEBUGS("\n1");
          state = 1;
        }
      }
      break;

    case 1: // play the file
      DEBUGS("\n1");
      if (!SMF.isEOF())
      {
        if (SMF.getNextEvent())
        {}
      }
      else
        state = 2;
      break;

    case 2:   // done with this one
digitalWrite(REDLIGHT, HIGH);
digitalWrite(BLUELIGHT, HIGH);
digitalWrite(LED, LOW);
digitalWrite(LED1, LOW);
digitalWrite(LED2, LOW);
attachInterrupt(digitalPinToInterrupt(BUTTON1), PIN1INTERRUPT, LOW);
attachInterrupt(digitalPinToInterrupt(BUTTON2), PIN2INTERRUPT, LOW);
play = 0;
      DEBUGS("\n2");
      SMF.close();
      midiSilence();
      timeStart = millis();
      state = 3;
      DEBUGS("\nWAIT_BETWEEN");
      break;

case 3:    // signal finished with a dignified pause


 break;

    default:
      state = 0;
      break;
  }
}

