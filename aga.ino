#define CLK 4
#define DIO 5
#define butt 2

#define zoomer 8
//Настройки
#define maxLength 50 // для настройки максимальной высоты при калибровке
#define pogreshnost 2

#include "GyverTM1637.h"
#include <Ultrasonic.h>
void(* resetFunc) (void) = 0;
Ultrasonic ultrasonic(7, 6, 9999); // (Trig PIN,Echo PIN)
GyverTM1637 disp(CLK, DIO);

boolean flag = true;
boolean flagButton = true;
boolean flagStandUp = false;
boolean flagBeep = true;

volatile int minDistance = 0;
volatile int maxDistance = 0;
volatile int sitDowns = 0;
volatile int distance = 0;

long beepTime = millis();

void setup() {
  pinMode(butt, INPUT_PULLUP);
  pinMode(zoomer, OUTPUT);

  disp.clear();
  disp.brightness(7);  // яркость, 0 - 7 (минимум - максимум)

  delay(1000);
  runningTextHello();

  attachInterrupt(0, buttonPress, FALLING);

  digitalWrite(zoomer, LOW);

}

void loop() {
  distance = ultrasonic.distanceRead(CM);
  delay(50);

  if (minDistance == 0)
  {
            Serial.print(minDistance);

            Serial.print(maxDistance);    
            if (maxDistance == 0)
        disp.displayByte(_N, _N, _N, _N);
      else
        disp.displayByte(_n, _n, _n, _n);

  }
  else
  {
    if (flag)
    {
      runningTextGo();
      flag = false;
    }
    else
    {
      if (flagStandUp == true)
      {
        if (distance <= (minDistance + pogreshnost))
        {

          sitDowns += 1;
          if (flagBeep != flagStandUp)
          {
            tone(zoomer, 2000, 100);
            flagBeep = flagStandUp;
          }
          flagStandUp = false;
        }
      }
      else
      {

        if (distance >= (maxDistance - pogreshnost) && distance < maxDistance + maxLength)
        {

          if (flagBeep != flagStandUp)
          {
            tone(zoomer, 1000, 100);
            flagBeep = flagStandUp;
          }
          flagStandUp = true;
        }
      }
      disp.displayInt(sitDowns);

    }

  }

}


void runningTextHello() {
  byte welcome_banner[] = {_H, _E, _L, _L, _O, _empty, _empty };
  disp.runningString(welcome_banner, sizeof(welcome_banner), 150);  // 200 это время в миллисекундах!
}

void runningTextGo() {
  byte welcome_banner[] = {_G, _O, _empty, _empty };
  disp.runningString(welcome_banner, sizeof(welcome_banner), 150);  // 200 это время в миллисекундах!
}

void buttonPress () {
  tone(zoomer, 500, 50);
  if (flagButton)
  {
    if (maxDistance == 0)
      maxDistance = distance;
    else
    {
      minDistance = distance;
      flagButton = false;
    }
  }

}
