#pragma once

#include "Arduino.h"

class Lights {
  public:
    Lights();

    void setup();
    void handle();

    void trigger();

  private:

    unsigned int waitTime;
};