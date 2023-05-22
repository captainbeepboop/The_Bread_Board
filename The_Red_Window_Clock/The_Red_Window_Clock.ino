#include "Simpletimer.h"
#include <ArduinoTapTempo.h>
ArduinoTapTempo tapTempo;
Simpletimer timer1;
#define tri_out A3
#define quad_out A4
#define pent_out A5
#define t1 5
#define t2 6
#define t3 7
#define q1 8
#define q2 9
#define q3 10
#define q4 11
#define p1 12
#define p2 13
#define p3 A0
#define p4 A1
#define p5 A2
#define bpmknoba 2
#define bpmknobb 4
#define tapbutton 3

volatile bool tri_gate = true;
volatile bool quad_gate = true;
volatile bool pent_gate = true;

int tcount = 0;
int qcount = 0;
int pcount = 0;

unsigned long       msec;
unsigned long       msecLst1;
unsigned long       msecLst2;
unsigned long       msecLst3;

int bpm = 120;
int tapbpm = 120;
int divisor = 4167;

int count = 0;

void setup() {
  pinMode(tri_out, OUTPUT);
  pinMode(quad_out, OUTPUT);
  pinMode(pent_out, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(bpmknoba, INPUT_PULLUP);
  pinMode(bpmknobb, INPUT_PULLUP);
  pinMode(tapbutton, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(bpmknoba), tempochange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(tapbutton), taptempo, FALLING);

  Serial.begin(9600);
}

void taptempo()
{
bpm = tapbpm;
if(bpm<16)
{bpm = 16;}
if(bpm>300)
{bpm = 300;};
divisor = 500000/bpm;
  
Serial.print(bpm);
}

void tempochange()
{unsigned long msec2 = micros ();
if(msec2 - msecLst2 > 000)
{if(digitalRead(bpmknoba)==(digitalRead(bpmknobb)))
{bpm = bpm+1;}
else
{bpm = bpm - 1;}
if(bpm<16)
{bpm = 16;}
if(bpm>300)
{bpm = 300;};
divisor = 500000/bpm;
}
msecLst2 = msec2;
Serial.print(bpm);

}


void loop() {
  
boolean buttonDown = digitalRead(tapbutton) == LOW;
tapTempo.update(buttonDown);
tapbpm = tapTempo.getBPM();

unsigned long msec = micros ();
if(msec - msecLst1 > divisor)

{msecLst1 = msec;

    count = count + 1;
    if (count == 60)
    {
      count = 0;
    }

    switch (count) {
      case 0:
        tri_gate = !tri_gate;
        digitalWrite(tri_out, tri_gate);

        quad_gate = !quad_gate;
        digitalWrite(quad_out, quad_gate);

        pent_gate = !pent_gate;
        digitalWrite(pent_out, pent_gate);
        
        tcount = tcount + 1;
        if (tcount == 6)
        {tcount = 0;}
        if (tcount == 0)
        {digitalWrite(t1, 1);
        digitalWrite(t3, 0);}
        if (tcount == 2)
        {digitalWrite(t2, 1);
        digitalWrite(t1, 0);}
        if (tcount == 4)
        {digitalWrite(t3, 1);
        digitalWrite(t2, 0);}
        
        qcount = qcount + 1;
        if (qcount == 8)
        {qcount = 0;}
        if (qcount == 0)
        {digitalWrite(q1, 1);
        digitalWrite(q4, 0);}
        if (qcount == 2)
        {digitalWrite(q2, 1);
        digitalWrite(q1, 0);}
        if (qcount == 4)
        {digitalWrite(q3, 1);
        digitalWrite(q2, 0);}
        if (qcount == 6)
        {digitalWrite(q4, 1);
        digitalWrite(q3, 0);}
        
        pcount = pcount + 1;
        if (pcount == 10)
        {pcount = 0;}
        if (pcount == 0)
        {digitalWrite(p1, 1);
        digitalWrite(p5, 0);}
        if (pcount == 2)
        {digitalWrite(p2, 1);
        digitalWrite(p1, 0);}
        if (pcount == 4)
        {digitalWrite(p3, 1);
        digitalWrite(p2, 0);}
        if (pcount == 6)
        {digitalWrite(p4, 1);
        digitalWrite(p3, 0);}
        if (pcount == 8)
        {digitalWrite(p5, 1);
        digitalWrite(p4, 0);}
        
        break;

      case 12:
      case 24:
      case 36:
      case 48:
        pent_gate = !pent_gate;
        digitalWrite(pent_out, pent_gate);
        
        pcount = pcount + 1;
        if (pcount == 10)
        {pcount = 0;}
        if (pcount == 0)
        {digitalWrite(p1, 1);
        digitalWrite(p5, 0);}
        if (pcount == 2)
        {digitalWrite(p2, 1);
        digitalWrite(p1, 0);}
        if (pcount == 4)
        {digitalWrite(p3, 1);
        digitalWrite(p2, 0);}
        if (pcount == 6)
        {digitalWrite(p4, 1);
        digitalWrite(p3, 0);}
        if (pcount == 8)
        {digitalWrite(p5, 1);
        digitalWrite(p4, 0);}
        
        break;

      case 15:
      case 30:
      case 45:
        quad_gate = !quad_gate;
        digitalWrite(quad_out, quad_gate);

        qcount = qcount + 1;
        if (qcount == 8)
        {qcount = 0;}
        if (qcount == 0)
        {digitalWrite(q1, 1);
        digitalWrite(q4, 0);}
        if (qcount == 2)
        {digitalWrite(q2, 1);
        digitalWrite(q1, 0);}
        if (qcount == 4)
        {digitalWrite(q3, 1);
        digitalWrite(q2, 0);}
        if (qcount == 6)
        {digitalWrite(q4, 1);
        digitalWrite(q3, 0);}
        
        break;

      case 20:
      case 40:
        tri_gate = !tri_gate;
        digitalWrite(tri_out, tri_gate);
        
        tcount = tcount + 1;
        if (tcount == 6)
        {tcount = 0;}
        if (tcount == 0)
        {digitalWrite(t1, 1);
        digitalWrite(t3, 0);}
        if (tcount == 2)
        {digitalWrite(t2, 1);
        digitalWrite(t1, 0);}
        if (tcount == 4)
        {digitalWrite(t3, 1);
        digitalWrite(t2, 0);}

        break;
    }
  }
}
