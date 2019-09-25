#pragma once

#include "Arduino.h"

#define DONATE_PIN PD2
#define TOKEN_PIN PD3

class Coin {
  public:
    Coin();
    void setup();
    void handle();

    void fake();
    void real();

    unsigned int count = 0;
    unsigned int donations = 0;

  private:
    void token();
};