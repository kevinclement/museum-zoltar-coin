#include "Arduino.h"
#include "lights.h"

#define MAIN_LIGHT_PIN PD7

#define TOTAL_TIME_TO_RUN 12000

#define TIME_BETWEEN_FLASHES_L 600
#define TIME_BETWEEN_FLASHES_H 900

#define TIME_LIGHT_ON_L 20
#define TIME_LIGHT_ON_H 70
#define TIME_LIGHT_OFF_L 20
#define TIME_LIGHT_OFF_H 40

bool _enabled = false;
unsigned long light_timestamp = 0;
unsigned long _triggered_at = 0;
int lastLoop = 0;

Lights::Lights()
{  
}

void Lights::setup() {
  pinMode(MAIN_LIGHT_PIN, OUTPUT);
  waitTime = random(TIME_BETWEEN_FLASHES_L, TIME_BETWEEN_FLASHES_H);
}

void Lights::handle() {
  if (_enabled && _triggered_at > 0 && millis() - _triggered_at > TOTAL_TIME_TO_RUN) {
    Serial.println("turning off lights due to timeout.");
    _enabled = false;
  }

  if (_enabled) {
    if (light_timestamp == 0 || millis() - light_timestamp > waitTime) {
      int loops = random(1,5);

      while(loops == lastLoop) {
        loops = random(1,5);
      }
      lastLoop = loops;

      for (int i=0; i < loops; i++)
      {
        analogWrite(MAIN_LIGHT_PIN, 255);
        delay(random(TIME_LIGHT_ON_L, TIME_LIGHT_ON_H));
        analogWrite(MAIN_LIGHT_PIN, 0);
        delay(random(TIME_LIGHT_OFF_L, TIME_LIGHT_OFF_H));
      }

      light_timestamp = millis();
      waitTime = random(TIME_BETWEEN_FLASHES_L, TIME_BETWEEN_FLASHES_H);
    } else {
      analogWrite(MAIN_LIGHT_PIN, 0);
    }

  } else {
    analogWrite(MAIN_LIGHT_PIN, 0);
  }
}

void Lights::trigger() {
  Serial.println("triggering lights.");
  _enabled = !_enabled;
  _triggered_at = millis();
}