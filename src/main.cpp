#include <Arduino.h>
#include "version.h"
#include "coin.h"
#include "led.h"
#include "lights.h"

Coin coin;
LED led;
Lights lights;

// functions
void readAnySerialMessage();
void fake();
void real();
void status();
void(* resetFunc) (void) = 0;

// track coin count
uint8_t coin_count = 0;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  
  Serial.println("Museum zoltar coin detector by kevinc...");
  Serial.println(getFullVersion("museum-zoltar-coin"));

  coin.setup();
  led.setup();
  lights.setup();

  attachInterrupt(digitalPinToInterrupt(DONATE_PIN), fake, RISING);
  attachInterrupt(digitalPinToInterrupt(TOKEN_PIN), real, RISING);
}

void loop() {
  readAnySerialMessage();

  coin.handle();
  led.handle();
  lights.handle();

  if (coin.count != coin_count) {

  }
}

void status() {
  // status=version:vb7e8fa7-dirty,buildDate:2009-11-10 11:09,gitDate:2019-08-06 14:51:47
}

void readAnySerialMessage() {
  if (!Serial.available()) {
    return;
  }

  String msg = Serial.readStringUntil('\n');
  Serial.print("got '");
  Serial.print(msg);
  Serial.println("' command");

  if (msg == "lights" || msg == "l") {
    Serial.println("toggling blinking lights...");
    // blink_lights = !blink_lights;
  }
  else if (msg == "solve" || msg == "v") {
  }
  else if (msg == "reset" || msg == "reboot" || msg == "r") {
    resetFunc();
  } else {
    Serial.print("unknown command: ");
    Serial.println(msg);
  }
}

// I dont want to figure out static callers and such to get this working
// in class world with attachinterupt, so just calling instance
void fake() {
  coin.fake();
}
void real() {
  coin.real();
}
