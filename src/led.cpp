#include "Arduino.h"
#include "led.h"

#define LED1_PIN 10
#define LED2_PIN 11
#define LED3_PIN 12

LED::LED()
{  
}

void LED::setup() {
}

void LED::handle() {
   analogWrite(LED1_PIN, state[0] ? 255 : 0);
   analogWrite(LED2_PIN, state[1] ? 255 : 0);
   analogWrite(LED3_PIN, state[2] ? 255 : 0);
}
