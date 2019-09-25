#pragma once

#include "Arduino.h"

class LED {
  public:
    LED();
    void setup();
    void handle();

    bool state[3] = {false};

  private:
};