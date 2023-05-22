#include "Timer.h"
Timer timer;
Timer timer2;

bool inputlast = 0;
int speed = 1000;
bool state = 0;

void setup() {

pinMode(9, INPUT_PULLUP);
pinMode(10, OUTPUT);
pinMode(A0, INPUT);
pinMode(A1, OUTPUT);
pinMode(A2, OUTPUT);

Serial.begin(9600);
timer.start();

}

void loop() {
int speed = map(analogRead(A0), 1023, 0, 50, 1000);

if(digitalRead(9)==1)
{if(inputlast == 0)
{
inputlast = 1;
state = 1;
digitalWrite(A2, HIGH);
digitalWrite(10, HIGH);
timer2.start();
timer.start();
}
digitalWrite(A1, HIGH);
}
else
{inputlast = 0;
digitalWrite(A1, LOW);}
 

if(timer.read() > speed) 
{state = 1;
timer2.start();
digitalWrite(A2, state);
timer.start();
digitalWrite(10, state);
}

if(timer2.read()==5)
{state = 0;
digitalWrite(A2, state);
digitalWrite(10, state);
}

  
}
