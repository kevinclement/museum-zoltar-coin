#include "Arduino.h"
#include "lights.h"

#define MAIN_LIGHT_PIN PD7

unsigned long light_time = millis();
bool light_on = false;

unsigned long lastTime = 0;
int waitTime = 1500;
bool blink_lights = false;

Lights::Lights()
{  
}

void Lights::setup() {
  pinMode(MAIN_LIGHT_PIN, OUTPUT);
}

void Lights::handle() {
  // if (blink_lights) {
  //   if (millis() - lastTime > waitTime)  // time for a new flash
  //   {
  //       // adjust timing params
  //       flash();
  //       lastTime = millis();
  //   }
  // } else {
  //   analogWrite(MAIN_LIGHT_PIN, 0);
  // }
}

#define TIMES 7
void flash() {
  for (int i=0; i< random(TIMES); i++)
  {
    analogWrite(MAIN_LIGHT_PIN, 255);
    delay(62);
    analogWrite(MAIN_LIGHT_PIN, 0);
    delay(10);
  }
}
