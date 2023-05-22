#include <AutoMap.h>
#include <N64Controller.h>
#include "LedControl.h"
#include <MIDI.h>
#include "Simpletimer.h"

Simpletimer timer1;

MIDI_CREATE_DEFAULT_INSTANCE();

N64Controller player1 (5); // this controller for player one is on PIN 5

int x_joystick = 0;
int x_joystick_last = 0;
int y_joystick = 0;
int y_joystick_last = 0;

int beat = 0;
int lastbeat = 0;

int x_possy = 1; //  variable for x-position of cursor
int y_possy = 6; //  variable for y-position of cursor
int x_possy_last = 0; //  variable for x-position of cursor
int y_possy_last = 0; //  variable for y-position of cursor

int beatcount = 0;
int lastclock = 1;

int note = 48;
int note1 = 60;
int note2 = 60;
int note3 = 60;
int key = 0;
bool vibe = 0;
int velocity = 50;
int octave = 0;
int lastnote = 48;
int lastnote1 = 60;
int lastnote2 = 60;
int lastnote3 = 60;
bool click = 0;
bool live1 = 0;
bool live2 = 0;
int channel = 0;
int bar = 0;
int barcount = 0;
int barmax = 0;

#define kickout 10
#define snareout 11
#define hihatout 8
#define triggerout 9

bool lastdup = 0;
bool lastddown = 0;
bool lastdleft = 0;
bool lastdright = 0;

int myChannel1[4][16] = {
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
  
};

int myChannel2[4][16] = {
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
  
};

int myChannel3[4][16] = {
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
  
};

int myChannel4[4][16] = {
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
  
};

const int myPreset1a[4][16] PROGMEM = {
{12, -1, 12, 14, 14, 14, 16, -1, 7, -1, 7, 14, 14, 14, 16, -1},
{9, -1, 9, 14, 14, 14, 16, -1, 5, -1, 5, 16, 16, 16, 14, -1},
{12, -1, 12, 14, 14, 14, 16, -1, 7, -1, 7, 14, 14, 14, 16, -1},
{9, -1, 9, 14, 14, 14, 16, -1, 5, -1, 5, 16, 16, 16, 14, -1},
};

const int myPreset2a[4][16] PROGMEM = {
{0, -1, -1, -1, -1, -1, -1, -1, 7, -1, -1, -1, -1, -1, -1, -1},
{9, -1, -1, -1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, -1, -1},
{0, -1, -1, -1, -1, -1, -1, -1, 7, -1, -1, -1, -1, -1, -1, -1},
{9, -1, -1, -1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, -1, -1},
};

const int myPreset3a[4][16] PROGMEM  = {
{4, -1, -1, -1, -1, -1, -1, -1, 11, -1, -1, -1, -1, -1, -1, -1},
{12, -1, -1, -1, -1, -1, -1, -1, 9, -1, -1, -1, -1, -1, -1, -1},
{4, -1, -1, -1, -1, -1, -1, -1, 11, -1, -1, -1, -1, -1, -1, -1},
{12, -1, -1, -1, -1, -1, -1, -1, 9, -1, -1, -1, -1, -1, -1, -1},
};

const int myPreset4a[4][16] PROGMEM = {
{7, -1, -1, -1, -1, -1, -1, -1, 14, -1, -1, -1, -1, -1, -1, -1},
{16, -1, -1, -1, -1, -1, -1, -1, 12, -1, -1, -1, -1, -1, -1, -1},
{7, -1, -1, -1, -1, -1, -1, -1, 14, -1, -1, -1, -1, -1, -1, -1},
{16, -1, -1, -1, -1, -1, -1, -1, 12, -1, -1, -1, -1, -1, -1, -1},
};

const int myPreset1b[4][16] PROGMEM = {
{-7, -1, 5, -1, -7, -1, 5, -1, -12, -1, 0, -1, -12, -1, 0, -1},
{-10, -1, 2, -1, -10, -1, 2, -1, -10, -1, 2, -1, -10, -1, 2, -1},
{-7, -1, 5, -1, -7, -1, 5, -1, -12, -1, 0, -1, -12, -1, 0, -1},
{-10, -1, 2, -1, -10, -1, 2, -1, -10, -1, 2, -1, -10, -1, 2, -1},
};
const int myPreset2b[4][16] PROGMEM = {
{5, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1},
{2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{5, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1},
{2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
};

const int myPreset3b[4][16] PROGMEM  = {
{9, -1, -1, -1, -1, -1, -1, -1, 4, -1, -1, -1, -1, -1, -1, -1},
{5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, -1, -1, -1, -1, -1, -1, -1, 4, -1, -1, -1, -1, -1, -1, -1},
{5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
};

const int myPreset4b[4][16] PROGMEM = {
{12, -1, -1, -1, -1, -1, -1, -1, 7, -1, -1, -1, -1, -1, -1, -1},
{9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{12, -1, -1, -1, -1, -1, -1, -1, 7, -1, -1, -1, -1, -1, -1, -1},
{9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
};

bool myKick[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
bool mySnare[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
bool myHihat[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
bool myTrigger[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

LedControl lc = LedControl(2, 4, 3, 1);
AutoMap kMapx(0, 1000, 0, 7);
AutoMap kMapy(0, 1000, 0, 7);

void setup() {
  player1.begin();
  Serial.begin(9600);
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 15); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  
  MIDI.begin();   

  pinMode(6, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(7, INPUT);
  pinMode(12, INPUT_PULLUP);
  pinMode(kickout, OUTPUT);
  pinMode(snareout, OUTPUT);
  pinMode(hihatout, OUTPUT);
  pinMode(triggerout, OUTPUT);

lc.setLed(0, (5 - beatcount % 4), 6, true);
lc.setLed(0, 1, (2 + ((beatcount - beatcount % 4) / 4)), true);
lc.setLed(0, 0, 0, true);
lc.setLed(0, 5, 7, true);
lc.setLed(0, 0, 2, true);


digitalWrite(kickout, LOW);
digitalWrite(snareout, LOW);
digitalWrite(hihatout, LOW);



}

void loop() {
  
bool live2 = !digitalRead(6);
bool live1 = !digitalRead(12);

player1.update();
  
  /////////////////////// dpad
  
bool dup = player1.D_up();
bool ddown = player1.D_down();
bool dleft = player1.D_left();
bool dright = player1.D_right();

if(player1.R()==0)
{
if(dup > lastdup)
{if(player1.L())
{key = key + 12;}
else
{key = key + 1;}}

if(ddown > lastddown)
{if(player1.L())
{key = key - 12;}
else
{key = key - 1;}}

if(dright > lastdright)
{if(player1.L()==0)
{bar = bar+1;
if(bar == 4)
{bar = 0;
lc.setLed(0, 0, 4, false);
lc.setLed(0, 0, 5, false);
}
if (bar == 1)
{lc.setLed(0, 0, 4, true);
lc.setLed(0, 0, 5, false);}
if (bar == 2)
{lc.setLed(0, 0, 4, false);
lc.setLed(0, 0, 5, true);}
if (bar == 3)
{lc.setLed(0, 0, 4, true);
lc.setLed(0, 0, 5, true);}}
else
{barmax = barmax+1;
if(barmax == 4)
{barmax = 0;
lc.setLed(0, 0, 6, false);
lc.setLed(0, 0, 7, false);
}
if (barmax == 1)
{lc.setLed(0, 0, 6, true);
lc.setLed(0, 0, 7, false);}
if (barmax == 2)
{lc.setLed(0, 0, 6, false);
lc.setLed(0, 0, 7, true);}
if (barmax == 3)
{lc.setLed(0, 0, 6, true);
lc.setLed(0, 0, 7, true);}}
  
}

if(dleft > lastdleft)
{if(player1.L()==0)
{channel = channel+1;
if(channel > 3)
{channel = 0;}
if(channel == 0)
{lc.setLed(0, 0, 0, true);
lc.setLed(0, 0, 1, false);
}
if(channel == 1)
{lc.setLed(0, 0, 0, false);
lc.setLed(0, 0, 1, true);
}
if(channel == 2)
{lc.setLed(0, 0, 0, true);
lc.setLed(0, 0, 1, true);
}
if(channel == 3)
{lc.setLed(0, 0, 0, false);
lc.setLed(0, 0, 1, false);
lc.setLed(0,2,2,false);
lc.setLed(0,2,3,false);
lc.setLed(0,2,4,false);
lc.setLed(0,2,5,false);
lc.setLed(0,3,2,false);
lc.setLed(0,3,3,false);
lc.setLed(0,3,4,false);
lc.setLed(0,3,5,false);
lc.setLed(0,4,2,false);
lc.setLed(0,4,3,false);
lc.setLed(0,4,4,false);
lc.setLed(0,4,5,false);
lc.setLed(0,5,2,false);
lc.setLed(0,5,3,false);
lc.setLed(0,5,4,false);
lc.setLed(0,5,5,false);

}}
else
{vibe = !vibe;}
if(vibe == 0)
{lc.setLed(0, 0, 2, true);
lc.setLed(0, 0, 3, false);
}
else
{lc.setLed(0, 0, 2, false);
lc.setLed(0, 0, 3, true);
}
}}
else ///PRESETS, BABY!
{if(player1.L()==1)
{
if(dleft > lastdleft)
{
 for (int i = 0; i < 16; i++) {
   for(int j = 0; j < 4; j++)
{myChannel1[j][i] = pgm_read_word_near(&(myPreset1a[j][i]));
}
}
 for (int i = 0; i < 16; i++) {
   for(int j = 0; j < 4; j++)
{myChannel2[j][i] = pgm_read_word_near(&(myPreset2a[j][i]));
}
}
 for (int i = 0; i < 16; i++) {
   for(int j = 0; j < 4; j++)
{myChannel3[j][i] = pgm_read_word_near(&(myPreset3a[j][i]));
}
}
 for (int i = 0; i < 16; i++) {
   for(int j = 0; j < 4; j++)
{myChannel4[j][i] = pgm_read_word_near(&(myPreset4a[j][i]));
}
}

barmax = 3;
barcount = 3;
lc.setLed(0, 0, 6, true);
lc.setLed(0, 0, 7, true);
lc.setLed(0, 5, 7, false);
lc.setLed(0, 4, 7, false);
lc.setLed(0, 3, 7, false);
lc.setLed(0, 2, 7, true);
}

if(dup > lastdup)
{
//PRESET  
}

if(dright > lastdright)
{
for (int i = 0; i < 16; i++) {
   for(int j = 0; j < 4; j++)
{myChannel1[j][i] = pgm_read_word_near(&(myPreset1b[j][i]));
}
}
 for (int i = 0; i < 16; i++) {
   for(int j = 0; j < 4; j++)
{myChannel2[j][i] = pgm_read_word_near(&(myPreset2b[j][i]));
}
}
 for (int i = 0; i < 16; i++) {
   for(int j = 0; j < 4; j++)
{myChannel3[j][i] = pgm_read_word_near(&(myPreset3b[j][i]));
}
}
 for (int i = 0; i < 16; i++) {
   for(int j = 0; j < 4; j++)
{myChannel4[j][i] = pgm_read_word_near(&(myPreset4b[j][i]));
}
}

barmax = 3;
barcount = 3;
lc.setLed(0, 0, 6, true);
lc.setLed(0, 0, 7, true);
lc.setLed(0, 5, 7, false);
lc.setLed(0, 4, 7, false);
lc.setLed(0, 3, 7, false);
lc.setLed(0, 2, 7, true);}
  
if(ddown > lastddown)
{
 for (int i = 0; i < 16; i++) {
   for(int j = 0; j < 4; j++)
{myChannel1[j][i] = -1;
myChannel2[j][i] = -1;
myChannel3[j][i] = -1;
myChannel4[j][i] = -1;
}
}}

}}

lastdup = dup;
lastddown = ddown;
lastdleft = dleft;
lastdright = dright;
  

  ////////////////////////////////////////////joystick


  int x_value = int(player1.axis_x());
  int y_value = int(player1.axis_y());


if(player1.L()==0)
  {if (x_value > 60)
  {
    x_joystick = 1;
  }
  else
  {
    x_joystick = 0;
  }
  if (x_value < -60)
  {
    x_joystick = -1;
  }

  if (x_joystick != x_joystick_last)
  {
    x_possy = x_possy + x_joystick;
  }
  if (x_possy > 6)
  {
    x_possy = 6;
  }
  if (x_possy < 1)
  {
    x_possy = 1;
  }

  x_joystick_last = x_joystick;

  //find y_possy

  if (y_value > 10)
  {
    y_joystick = 1;
  }
  else
  {
    y_joystick = 0;
  }
  if (y_value < -10)
  {
    y_joystick = -1;
  }

  if (y_joystick != y_joystick_last)
  {
    y_possy = y_possy + y_joystick;
  }
  if (y_possy > 6)
  {
    y_possy = 6;
  }
  if (y_possy < 0)
  {
    y_possy = 0;
  }

  y_joystick_last = y_joystick;

  if (y_possy_last != y_possy)
  {
    lc.setLed(0, 7, 7 - y_possy_last, false);
  }
  if (x_possy_last != x_possy)
  {
    lc.setLed(0, 7 - x_possy_last, 0, false);
  }

  lc.setLed(0, 7, 7 - y_possy, true); //HERE
  lc.setLed(0, 7 - x_possy, 0, true);
  x_possy_last = x_possy;
  y_possy_last = y_possy;}
  else
  {int lfospeed = map(x_value, -75, 75, 0, 127);
  MIDI.sendControlChange(41, lfospeed, 1);
  int lfointensity = map(y_value, -75, 75, 0, 127);
  MIDI.sendControlChange(42, lfointensity, 1);
  }	

    int beatnumber = (x_possy-2) + 4*(5-y_possy);

////////////// notes

if(player1.R()==1)
{octave = 1;}
else
{octave = 0;}

if((y_possy>1 && y_possy <6)&&(x_possy>1 && x_possy <6))
 { if (player1.Z() == 1)
  {
    if((channel==0)||(channel==2))
    {myChannel1[bar][beatnumber] = 0+12*octave;}
    if((channel==1)||(channel==2))
    {myChannel2[bar][beatnumber] = 0+12*octave;
      myChannel3[bar][beatnumber] = 4+12*octave-vibe;
      myChannel4[bar][beatnumber] = 7+12*octave;
    }
    click = 1;
  }

  if (player1.B() == 1)
  {
    if((channel==0)||(channel==2))
    {myChannel1[bar][beatnumber] = 2+12*octave;}
    if((channel==1)||(channel==2))
    {myChannel2[bar][beatnumber] = 2+12*octave;
      myChannel3[bar][beatnumber] = 5+12*octave;
      myChannel4[bar][beatnumber] = 9+12*octave-vibe;
    }
    click = 1;
  }
  if (player1.A() == 1)
  {
    if((channel==0)||(channel==2))
    {myChannel1[bar][beatnumber] = 4+12*octave-vibe;}
    if((channel==1)||(channel==2))
    {myChannel2[bar][beatnumber] = 4+12*octave-vibe;
    myChannel3[bar][beatnumber] = 7+12*octave;
    myChannel4[bar][beatnumber] = 11+12*octave-vibe;
    }
    click = 1;
  }
  if(player1.L() == 0){
  if (player1.C_left() == 1)
  {
    if((channel==0)||(channel==2))
    {myChannel1[bar][beatnumber] = 5+12*octave;}
    if((channel==1)||(channel==2))
    {myChannel2[bar][beatnumber] = 5+12*octave;
      myChannel3[bar][beatnumber] = 9+12*octave-vibe;
      myChannel4[bar][beatnumber] = 12+12*octave;
    }
    click = 1;
  }
  if (player1.C_down() == 1)
  {
    if((channel==0)||(channel==2))
    {myChannel1[bar][beatnumber] = 7+12*octave;}
    if((channel==1)||(channel==2))
    {myChannel2[bar][beatnumber] = 7+12*octave;
      myChannel3[bar][beatnumber] = 11+12*octave-vibe;
      myChannel4[bar][beatnumber] = 14+12*octave;
    }
    click = 1;
  }
  if (player1.C_up() == 1)
  {
    if((channel==0)||(channel==2))
    {myChannel1[bar][beatnumber] = 9+12*octave-vibe;}
    if((channel==1)||(channel==2))
    {myChannel2[bar][beatnumber] = 9+12*octave-vibe;
      myChannel3[bar][beatnumber] = 12+12*octave;
      myChannel4[bar][beatnumber] = 16+12*octave-vibe;
    }
    click = 1;
    
    
  }
  if (player1.C_right() == 1)
  {
    if((channel==0)||(channel==2))
    {myChannel1[bar][beatnumber] = 11+12*octave-vibe;}
    if((channel==1)||(channel==2))
    {myChannel2[bar][beatnumber] = 11+12*octave-vibe;
      myChannel3[bar][beatnumber] = 14+12*octave;
      myChannel4[bar][beatnumber] = 17+12*octave;
    }
    click = 1;
  }
  if (player1.Start() == 1)
  {
    if((channel==0)||(channel==2))
    {myChannel1[bar][beatnumber] = -1;}
    if((channel==1)||(channel==2))
    {myChannel2[bar][beatnumber] = -1;
    myChannel3[bar][beatnumber] = -1;
    myChannel4[bar][beatnumber] = -1;
    }
    click = 1;
  }}

///// automove

if(player1.Z()==0 && player1.B()==0 && player1.A()==0 && player1.C_left()==0 && player1.C_up()==0 && player1.C_down()==0 && player1.C_right()==0 && player1.Start()==0 && click == 1)
{click = 0;
x_possy = x_possy + 1;
if(x_possy == 6)
{x_possy = 2;
y_possy = y_possy - 1;}
if(y_possy == 1)
{y_possy = 5;


bar = bar+1;
if(bar>barmax)
{bar = 0;}
if(bar == 4)
{bar = 0;
lc.setLed(0, 0, 4, false);
lc.setLed(0, 0, 5, false);
}
if (bar == 1)
{lc.setLed(0, 0, 4, true);
lc.setLed(0, 0, 5, false);}
if (bar == 2)
{lc.setLed(0, 0, 4, false);
lc.setLed(0, 0, 5, true);}
if (bar == 3)
{lc.setLed(0, 0, 4, true);
lc.setLed(0, 0, 5, true);}
  
}
}

}

if(channel==0)
{for (int x = 0; x < 16; x = x +1) {
if(myChannel1[bar][x] == -1)
{lc.setLed(0,5-(x % 4),2+(x-(x % 4))/4, false);}
else
{lc.setLed(0,5-(x % 4),2+(x-(x % 4))/4, true);}}}   //HERE

if(channel==1)
{for (int x = 0; x < 16; x = x +1) {
if(myChannel2[bar][x] == -1)
{lc.setLed(0,5-(x % 4),2+(x-(x % 4))/4, false);}
else
{lc.setLed(0,5-(x % 4),2+(x-(x % 4))/4, true);}}}   //HERE

if(channel==2)
{for (int x = 0; x < 16; x = x +1) {
if((myChannel1[bar][x] == -1)&&(myChannel2[bar][x] < 0))
{lc.setLed(0,5-(x % 4),2+(x-(x % 4))/4, false);}
else
{lc.setLed(0,5-(x % 4),2+(x-(x % 4))/4, true);}}}   //HERE

  ////////////////////////////////////////////////////////////////////////////////the beat

beat = digitalRead(7);
  
  if (beat > lastbeat)
  {
    if (beatcount < 15)
    {
      beatcount = beatcount + 1;
    }
    else
    {
      beatcount = 0;
      barcount = barcount + 1;
      if(barcount > barmax)
      {barcount = 0;
      lc.setLed(0, 5, 7, true);
      lc.setLed(0, 4, 7, false);
      lc.setLed(0, 3, 7, false);
      lc.setLed(0, 2, 7, false);
      };
      if(barcount == 1)
      {lc.setLed(0, 5, 7, false);
      lc.setLed(0, 4, 7, true);
      lc.setLed(0, 3, 7, false);
      lc.setLed(0, 2, 7, false);}
      if(barcount == 2)
      {lc.setLed(0, 5, 7, false);
      lc.setLed(0, 4, 7, false);
      lc.setLed(0, 3, 7, true);
      lc.setLed(0, 2, 7, false);}
      if(barcount == 3)
      {lc.setLed(0, 5, 7, false);
      lc.setLed(0, 4, 7, false);
      lc.setLed(0, 3, 7, false);
      lc.setLed(0, 2, 7, true);}
    }
    

    lc.setLed(0, 2, 6, false);
    lc.setLed(0, 3, 6, false);
    lc.setLed(0, 4, 6, false);
    lc.setLed(0, 5, 6, false);
    lc.setLed(0, (5 - beatcount % 4), 6, true);

    lc.setLed(0, 1, 2, false);
    lc.setLed(0, 1, 3, false);
    lc.setLed(0, 1, 4, false);
    lc.setLed(0, 1, 5, false);
    lc.setLed(0, 1, (2 + ((beatcount - beatcount % 4) / 4)), true);

//playing midi note on beat

if(live1 == 0)
{
    if (myChannel1[barcount][beatcount] != -1)
    {
note = 48+myChannel1[barcount][beatcount]+key;
MIDI.sendNoteOn(note, 127, 1);	
lastnote = note;
    }}
if(live1 == 1)
{
if(player1.L()==0)
{
note = 0;
if(player1.Z()==1)
{note = 48+key+12*octave;
}
if(player1.B()==1)
{note = 50+key+12*octave;
}
if(player1.A()==1)
{note = 52+key+12*octave-vibe;
}
if(player1.C_left()==1)
{note = 53+key+12*octave;
}
if(player1.C_down()==1)
{note = 55+key+12*octave;
}
if(player1.C_up()==1)
{note = 57+key+12*octave-vibe;
}
if(player1.C_right()==1)
{note = 59+key+12*octave-vibe;
}
if(note>0)
{
MIDI.sendNoteOn(note, 127, 1);
lastnote = note;
}
}}

if(live2 == 0)
{
  
if (myChannel2[barcount][beatcount] != -1)
    {
note1 = 60+myChannel2[barcount][beatcount]+key;
MIDI.sendNoteOn(note1, 127, 2);	
lastnote1 = note1;
    }
if (myChannel3[barcount][beatcount] != -1)
    {
note2 = 60+myChannel3[barcount][beatcount]+key;
MIDI.sendNoteOn(note2, 127, 3);	
lastnote2 = note2;
    }
if (myChannel4[barcount][beatcount] != -1)
    {
note3 = 60+myChannel4[barcount][beatcount]+key;
MIDI.sendNoteOn(note3, 127, 4);	
lastnote3 = note3;
    }
}
else
{
if(player1.L()==0)
{
note1 = 0;
if(player1.Z()==1)
{note1 = 12+48+key+12*octave;
note2 = 12+48+key+4-vibe+12*octave;
note3 = 12+48+key+7+12*octave;
}
if(player1.B()==1)
{note1 = 12+50+key+12*octave;
note2 = 12+50+key+3+12*octave;
note3 = 12+50+key+7-vibe+12*octave;
}
if(player1.A()==1)
{note1 = 12+52+key+12*octave-vibe;
note2 = 12+52+key+3+12*octave;
note3 = 12+52+key+7-vibe+12*octave;
}
if(player1.C_left()==1)
{note1 = 12+53+key+12*octave;
note2 = 12+53+key+4-vibe+12*octave;
note3 = 12+53+key+7+12*octave;
}
if(player1.C_down()==1)
{note1 = 12+55+key+12*octave;
note2 = 12+55+key+4-vibe+12*octave;
note3 = 12+55+key+7+12*octave;
}
if(player1.C_up()==1)
{note1 = 12+57+key+12*octave-vibe;
note2 = 12+57+key+3+12*octave;
note3 = 12+57+key+7-vibe+12*octave;
}
if(player1.C_right()==1)
{note1 = 12+59+key+12*octave-vibe;
note2 = 12+59+key+3+12*octave;
note3 = 12+59+key+6+12*octave;
}
if(note1>0)
{
if(player1.Start()==0)
{MIDI.sendNoteOn(note1, 127, 2);	
MIDI.sendNoteOn(note2, 127, 3);	
MIDI.sendNoteOn(note3, 127, 4);}	
else
{
if(beatcount % 4 == 0)
{MIDI.sendNoteOn(note1, 127, 2);}
if(beatcount % 4 == 1)
{MIDI.sendNoteOn(note2, 127, 3);}
if(beatcount % 4 == 2)
{MIDI.sendNoteOn(note3, 127, 4);}
if(beatcount % 4 == 3)
{MIDI.sendNoteOn(note2, 127, 3);}
}

lastnote1 = note1;
lastnote2 = note2;
lastnote3 = note3;

}}}

//ENTERING IN DRUM BEATS
//MOST RECENT CHANGE IN LINE BELOW
if(player1.Z()==0){
if(player1.L()==1)
{if(player1.Start()==0)
{if(player1.C_left()==1)
{mySnare[beatcount]=1;}
if(player1.C_down()==1)
{myKick[beatcount]=1;}
if(player1.C_up()==1)
{myHihat[beatcount]=1;}
if(player1.C_right()==1)
{myTrigger[beatcount]=1;}
}
else
{if(player1.C_left()==1)
{mySnare[beatcount]=0;}
if(player1.C_down()==1)
{myKick[beatcount]=0;}
if(player1.C_up()==1)
{myHihat[beatcount]=0;}
if(player1.C_right()==1)
{myTrigger[beatcount]=0;}}
  
  
}
  
  
}
else
{if(player1.C_left()==1)
{digitalWrite(snareout, 1);}
if(player1.C_down()==1)
{digitalWrite(kickout, 1);}
if(player1.C_up()==1)
{digitalWrite(hihatout, 1);}
if(player1.C_right()==1)
{digitalWrite(triggerout, 1);}}

//playing the drum    
if(player1.Z()==0 || player1.L() == 0)
{if(myKick[beatcount]==1)
{digitalWrite(kickout, 1);}
if(mySnare[beatcount]==1)
{digitalWrite(snareout, 1);}
if(myHihat[beatcount]==1)
{digitalWrite(hihatout, 1);}
if(myTrigger[beatcount]==1)
{digitalWrite(triggerout, 1);} 
}
  }
  
//turning off midi note on offbeat


  if (beat < lastbeat)
  {
 MIDI.sendControlChange(123,0,1);
   MIDI.sendControlChange(123,0,2);
    MIDI.sendControlChange(123,0,3);
     MIDI.sendControlChange(123,0,4);
      MIDI.sendControlChange(123,0,5);
       MIDI.sendControlChange(123,0,6);
        MIDI.sendControlChange(123,0,7);
 
 //turn off the drums
 
digitalWrite(kickout, 0);
digitalWrite(snareout, 0);
digitalWrite(hihatout, 0);
digitalWrite(triggerout, 0);
 
  }


  lastbeat = beat;

//setting the drums
  
}