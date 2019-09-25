#include "Arduino.h"
#include "lights.h"

#define MAIN_LIGHT_PIN PD7

bool _enabled = false;

Lights::Lights()
{  
}

void Lights::setup() {
  pinMode(MAIN_LIGHT_PIN, OUTPUT);
}

void Lights::handle() {
  if (_enabled) {
    for (int i=0; i< random(7); i++)
    {
      analogWrite(MAIN_LIGHT_PIN, 255);
      delay(62);
      analogWrite(MAIN_LIGHT_PIN, 0);
      delay(10);
    }
    _enabled = false;
  } else {
    analogWrite(MAIN_LIGHT_PIN, 0);
  }
}

void Lights::trigger() {
  Serial.println("triggering lights.");
  _enabled = true;
}